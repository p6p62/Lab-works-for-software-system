#include "FieldStateTreeNode.h"

void FieldStateTreeNode::create_next_states()
{
	// TODO
}

FieldStateTreeNode::FieldStateTreeNode(FieldStateTreeNode* previous_state, size_t state_number, Field field) : current_field_(field)
{
	previous_state_ = previous_state;
	state_number_ = state_number;
	
	create_next_states();
}

bool FieldStateTreeNode::get_next_field_state_by_index(size_t index, FieldStateTreeNode& resulted_state)
{
	//TODO
	return false;
}
