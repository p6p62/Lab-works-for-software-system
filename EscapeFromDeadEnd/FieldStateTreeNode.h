#pragma once
#include "Field.h"

class FieldStateTreeNode
{
private:
	/*
	* ��������� ������� ����� ���������� ����� ������.
	* ������������ ����� 1 ���, �� ������� ���������� ����
	* ��� ����� ���� ��� ��� ������, ��� ������������ ������-���� �����
	* ��� "����" ������������ is_player_move
	*/
	struct MoveOnField
	{
		bool is_player_move;
		FieldCell new_player_position;
		Block& moved_block;
		FieldCell new_moved_block_upper_left_cell;
	};
private:
	FieldStateTreeNode* previous_state_;

	// ����� ����� ��������� ����� ������ ��������� �� previous_state ���������
	size_t state_number_;

	Field current_field_;
	std::vector<MoveOnField> next_states_in_moves_;
private:
	void create_next_states();
public:
	FieldStateTreeNode(FieldStateTreeNode* previous_state, size_t state_number, Field field);

	size_t get_next_states_count() { return next_states_in_moves_.size(); }
	bool get_next_field_state_by_index(size_t index, FieldStateTreeNode& resulted_state);
};

