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
		const Block& moved_block;
		FieldCell new_moved_block_upper_left_cell;
	};
private:
	FieldStateTreeNode* previous_state_;

	/// <summary>
	/// ����� ����� ��������� ����� ������ ���������, ��������� �� previous_state_ 
	/// </summary>
	size_t state_number_;

	Field current_field_;
	std::vector<MoveOnField> next_states_in_moves_;
private:
	/// <summary>
	/// ��������� ���� ������ ���������, �� ������� ����� ��������� �� ����� ����
	/// </summary>
	void create_next_states();
	void add_next_states_for_block(const Block& moved_block, std::vector<MoveOnField>& collected_states, int const* const* field_mask);
public:
	FieldStateTreeNode();
	FieldStateTreeNode(Field field);
	FieldStateTreeNode(FieldStateTreeNode* previous_state, size_t state_number, Field field);

	size_t get_next_states_count() { return next_states_in_moves_.size(); }
	bool get_next_field_state_by_index(size_t index, FieldStateTreeNode& resulted_state);
	const Field& get_current_field() const { return current_field_; }
};

