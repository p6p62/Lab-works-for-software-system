#include "FieldStateTreeNode.h"
#include <algorithm>

/// <summary>
/// ����� ������������� ���������� ������� �������� ������!!!
/// </summary>
/// <param name="field">���� ��� ������������� � �������</param>
/// <returns>��������� �� ��������� ������ ������� [field_row, field_col], � ������� 1 ����� �� ������ ������, ������ � ����� ������, � 0 ����� �� ��������� ������</returns>
int** field_to_array(Field& field)
{
	const int OCCUPIED_VALUE = 1;

	int rows = field.get_row_size();
	int cols = field.get_col_size();

	// ������ ������ � ��������� ������� ��������� ����
	int** result_array = new int* [rows];
	for (int i = 0; i < rows; i++)
		result_array[i] = new int [cols] {0};

	// ���������� ������
	size_t player_row = field.get_player_position().row;
	size_t player_col = field.get_player_position().col;
	result_array[player_row][player_col] = OCCUPIED_VALUE;

	// ���������� ����� ������
	// TODO ��������
	/*const FieldCell& end_point{ field.get_endgame_position() };
	result_array[end_point.row][end_point.col] = OCCUPIED_VALUE;*/

	// ���������� ������ ����
	for (const Block& block : field.get_field_blocks())
	{
		auto up_left_row = block.upper_left_cell_pos.row;
		auto up_left_col = block.upper_left_cell_pos.col;
		for (size_t i = 0; i < block.row_size; i++)
			for (size_t j = 0; j < block.col_size; j++)
				result_array[i + up_left_row][j + up_left_col] = OCCUPIED_VALUE;
	}

	return result_array;
}

/// <summary>
/// �������� ����������� �������� ����� moved_block �� ���� ������ � ������� �� ����������� � ��������� � ���� field_mask
/// </summary>
/// <param name="moved_block"></param>
/// <param name="left_to_right_offset">�������� �� �����������. +1 - ������, -1 - �����</param>
/// <param name="up_to_down_offset">�������� �� ���������. +1 - ����, -1 - �����</param>
/// <param name="field_mask">�������-����� ����, � ������� ���������� ��������. � ��������� ������� ������ ���� 0</param>
/// <param name="rows">����� ����� field_mask</param>
/// <param name="cols">����� �������� field_mask</param>
/// <returns>True, ���� �������� ��������, ����� false</returns>
bool check_block_move(const Block& moved_block, int left_to_right_offset, int up_to_down_offset, int const* const* field_mask, size_t rows, size_t cols)
{
	// �������� ����������� �������� �� ������������ ������ ��: (-1; 0), (1; 0), (0; -1), (0; 1)
	if (left_to_right_offset * up_to_down_offset != 0 || abs(left_to_right_offset + up_to_down_offset) != 1)
		return false;
	bool result{ true };

	int block_u_l_row = moved_block.upper_left_cell_pos.row;
	int block_u_l_col = moved_block.upper_left_cell_pos.col;

	int block_u_l_row_offset = block_u_l_row + up_to_down_offset;
	int block_u_l_col_offset = block_u_l_col + left_to_right_offset;

	// �������� �� ��������� � ������� ����� �����������
	if (block_u_l_row_offset >= 0
		&& block_u_l_col_offset >= 0
		&& (block_u_l_row_offset + moved_block.row_size) <= rows
		&& (block_u_l_col_offset + moved_block.col_size) <= cols)
	{
		// ���� ���� �������� �� �����������
		if (abs(left_to_right_offset) == 1)
		{
			// ����� ������������ �������
			size_t checked_col_index = (left_to_right_offset == -1) ? block_u_l_col_offset : block_u_l_col + moved_block.col_size;

			for (size_t i = block_u_l_row; i < block_u_l_row + moved_block.row_size; i++)
				if (field_mask[i][checked_col_index] != 0)
				{
					result = false;
					break;
				}
		}
		else // ����� �������� �� ���������
		{
			// ����� ����������� ������
			size_t checked_row_index = (up_to_down_offset == -1) ? block_u_l_row_offset : block_u_l_row + moved_block.row_size;

			for (size_t j = block_u_l_col; j < block_u_l_col + moved_block.col_size; j++)
				if (field_mask[checked_row_index][j] != 0)
				{
					result = false;
					break;
				}
		}
	}
	else
	{
		result = false;
	}
	return result;
}

void FieldStateTreeNode::create_next_states()
{
	next_states_in_moves_.clear();
	int** field_grid = field_to_array(current_field_); // �������-"�����" �������� ����

	// ���������� ��������� ����� ����� ������
	add_next_states_for_block(current_field_.get_player_block(), next_states_in_moves_, field_grid);

	// ��������� ����� ���� ��������� ����������� ������
	for (const Block& block : current_field_.get_field_blocks())
	{
		add_next_states_for_block(block, next_states_in_moves_, field_grid);
	}

	// �������� ������ ������
	for (size_t i = 0; i < current_field_.get_row_size(); i++)
		delete[] field_grid[i];
	delete[] field_grid;
}

/// <summary>
/// ��������� � collected_states ���������, ������� ����� �������� ������������ ����� moved_block �� ����������� ��� ��������� �� 1 �������. �������� �� field_mask
/// </summary>
/// <param name="moved_block">������������ ����</param>
/// <param name="collected_states">��������� ���������</param>
/// <param name="field_mask">�������-����� ��������� ����, �� ������� ���������� ��������</param>
void FieldStateTreeNode::add_next_states_for_block(const Block& moved_block, std::vector<MoveOnField>& collected_states, int const* const* field_mask)
{
	auto f = [&](int l_r, int u_d)
	{
		if (check_block_move(moved_block, l_r, u_d, field_mask, current_field_.get_row_size(), current_field_.get_col_size()))
		{
			FieldCell new_up_left_cell{ moved_block.upper_left_cell_pos.row + u_d, moved_block.upper_left_cell_pos.col + l_r };
			MoveOnField move_to_next_state = { moved_block, new_up_left_cell };
			collected_states.push_back(move_to_next_state);
		}
	};

	f(-1, 0);
	f(1, 0);
	f(0, -1);
	f(0, 1);
}

bool FieldStateTreeNode::get_next_field_state(const MoveOnField& move, FieldStateTreeNode& resulted_state, int index)
{
	if (index == -1)
	{
		for (int i = 0; i < next_states_in_moves_.size(); i++)
		{
			if (next_states_in_moves_[i] == move)
			{
				index = i;
				break;
			}
		}
		if (index == -1)
			return false;
	}

	resulted_state.state_number_ = index;
	resulted_state.previous_state_ = this;
	resulted_state.current_field_ = current_field_;
	resulted_state.current_field_.replace_block(move.moved_block, move.new_moved_block_upper_left_cell);
	resulted_state.create_next_states();

	return true;
}

FieldStateTreeNode::FieldStateTreeNode() : FieldStateTreeNode(Field(1, 1))
{
	next_states_in_moves_.clear();
}

FieldStateTreeNode::FieldStateTreeNode(Field field) : FieldStateTreeNode(nullptr, 0, field)
{}

FieldStateTreeNode::FieldStateTreeNode(FieldStateTreeNode* previous_state, size_t state_number, Field field) : current_field_(field)
{
	previous_state_ = previous_state;
	state_number_ = state_number;

	create_next_states();
}

bool FieldStateTreeNode::get_next_field_state_by_index(size_t index, FieldStateTreeNode& resulted_state)
{
	return this->get_next_field_state(next_states_in_moves_.at(index), resulted_state, index);
}

void FieldStateTreeNode::sort_next_states_by_descend(const std::function<double(const Field&)>& evaluatingFunction)
{
	std::sort(next_states_in_moves_.begin(), next_states_in_moves_.end(),
		[&](const MoveOnField& m1, const MoveOnField& m2) -> bool {
			FieldStateTreeNode s1, s2;
			get_next_field_state(m1, s1);
			get_next_field_state(m2, s2);

			return evaluatingFunction(s1.get_current_field()) > evaluatingFunction(s2.get_current_field());
		});
}
