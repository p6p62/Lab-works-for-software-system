#include "FieldStateTreeNode.h"
#include <algorithm>

/// <summary>
/// ПОСЛЕ ИСПОЛЬЗОВАНИЯ РЕЗУЛЬТАТА ФУНКЦИИ ОЧИСТИТЬ ПАМЯТЬ!!!
/// </summary>
/// <param name="field">Поле для представления в таблицу</param>
/// <returns>Указатель на двумерный массив размера [field_row, field_col], в котором 1 стоят на местах блоков, игрока и точки выхода, а 0 стоят на свободных местах</returns>
int** field_to_array(Field& field)
{
	const int OCCUPIED_VALUE = 1;

	int rows = field.get_row_size();
	int cols = field.get_col_size();

	// взятие памяти и получение таблицы элементов поля
	int** result_array = new int* [rows];
	for (int i = 0; i < rows; i++)
		result_array[i] = new int [cols] {0};

	// добавление игрока
	size_t player_row = field.get_player_position().row;
	size_t player_col = field.get_player_position().col;
	result_array[player_row][player_col] = OCCUPIED_VALUE;

	// добавление точки выхода
	// TODO подумать
	/*const FieldCell& end_point{ field.get_endgame_position() };
	result_array[end_point.row][end_point.col] = OCCUPIED_VALUE;*/

	// добавление блоков поля
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
/// Проверка возможности смещения блока moved_block на ОДНУ строку и столбец по горизонтали и вертикали в поле field_mask
/// </summary>
/// <param name="moved_block"></param>
/// <param name="left_to_right_offset">Смещение по горизонтали. +1 - вправо, -1 - влево</param>
/// <param name="up_to_down_offset">Смещение по вертикали. +1 - вниз, -1 - вверх</param>
/// <param name="field_mask">Таблица-маска поля, в котором происходит смещение. В свободных клетках должны быть 0</param>
/// <param name="rows">Число строк field_mask</param>
/// <param name="cols">Число столбцов field_mask</param>
/// <returns>True, если смещение возможно, иначе false</returns>
bool check_block_move(const Block& moved_block, int left_to_right_offset, int up_to_down_offset, int const* const* field_mask, size_t rows, size_t cols)
{
	// проверка направления смещений на соответствие одному из: (-1; 0), (1; 0), (0; -1), (0; 1)
	if (left_to_right_offset * up_to_down_offset != 0 || abs(left_to_right_offset + up_to_down_offset) != 1)
		return false;
	bool result{ true };

	int block_u_l_row = moved_block.upper_left_cell_pos.row;
	int block_u_l_col = moved_block.upper_left_cell_pos.col;

	int block_u_l_row_offset = block_u_l_row + up_to_down_offset;
	int block_u_l_col_offset = block_u_l_col + left_to_right_offset;

	// проверка на вхождение в границы после перемещения
	if (block_u_l_row_offset >= 0
		&& block_u_l_col_offset >= 0
		&& (block_u_l_row_offset + moved_block.row_size) <= rows
		&& (block_u_l_col_offset + moved_block.col_size) <= cols)
	{
		// если есть смещение по горизонтали
		if (abs(left_to_right_offset) == 1)
		{
			// выбор проверяемого столбца
			size_t checked_col_index = (left_to_right_offset == -1) ? block_u_l_col_offset : block_u_l_col + moved_block.col_size;

			for (size_t i = block_u_l_row; i < block_u_l_row + moved_block.row_size; i++)
				if (field_mask[i][checked_col_index] != 0)
				{
					result = false;
					break;
				}
		}
		else // иначе смещение по вертикали
		{
			// выбор проверяемой строки
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
	int** field_grid = field_to_array(current_field_); // таблица-"маска" текущего поля

	// добавление состояний после ходов игрока
	add_next_states_for_block(current_field_.get_player_block(), next_states_in_moves_, field_grid);

	// состояния после всех доступных перемещений блоков
	for (const Block& block : current_field_.get_field_blocks())
	{
		add_next_states_for_block(block, next_states_in_moves_, field_grid);
	}

	// очищение взятой памяти
	for (size_t i = 0; i < current_field_.get_row_size(); i++)
		delete[] field_grid[i];
	delete[] field_grid;
}

/// <summary>
/// Добавляет к collected_states состояния, которые можно получить перемещением блока moved_block по горизонтали или вертикали на 1 позицию. Проверка по field_mask
/// </summary>
/// <param name="moved_block">Перемещаемый блок</param>
/// <param name="collected_states">Следующие состояния</param>
/// <param name="field_mask">Таблица-маска исходного поля, по которой происходит проверка</param>
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
