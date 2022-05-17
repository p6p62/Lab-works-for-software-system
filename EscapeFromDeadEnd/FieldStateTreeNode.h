#pragma once
#include "Field.h"
#include <functional>

class FieldStateTreeNode
{
private:
	/*
	* Описывает отличие между некоторыми двумя полями.
	* Представляет собой 1 шаг, на который отличаются поля
	* Это может быть или ход игрока, или перестановка какого-либо блока
	* Тип "хода" определяется is_player_move
	*/
	struct MoveOnField
	{
		Block moved_block;
		FieldCell new_moved_block_upper_left_cell;

		bool operator== (const MoveOnField& m)
		{
			return this->moved_block == m.moved_block && this->new_moved_block_upper_left_cell == m.new_moved_block_upper_left_cell;
		}
	};
private:
	FieldStateTreeNode* previous_state_;

	/// <summary>
	/// Номер этого состояния среди других состояний, доступных из previous_state_ 
	/// </summary>
	size_t state_number_;

	Field current_field_;
	std::vector<MoveOnField> next_states_in_moves_;
private:
	/// <summary>
	/// Вычисляет узлы дерева состояний, до которых можно добраться из этого узла
	/// </summary>
	void create_next_states();
	void add_next_states_for_block(const Block& moved_block, std::vector<MoveOnField>& collected_states, int const* const* field_mask);
	bool get_next_field_state(const MoveOnField& move, FieldStateTreeNode& resulted_state, int index = -1);
public:
	FieldStateTreeNode();
	FieldStateTreeNode(Field field);
	FieldStateTreeNode(FieldStateTreeNode* previous_state, size_t state_number, Field field);

	size_t get_next_states_count() { return next_states_in_moves_.size(); }
	bool get_next_field_state_by_index(size_t index, FieldStateTreeNode& resulted_state);
	const Field& get_current_field() const { return current_field_; }
	FieldStateTreeNode* get_previous_state() { return previous_state_; }
	size_t get_state_number() const { return state_number_; }
	void sort_next_states_by_descend(const std::function<double(const Field&)>& evaluatingFunction);
};

