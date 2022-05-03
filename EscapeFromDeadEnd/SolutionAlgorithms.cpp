#include "SolutionAlgorithms.h"

bool recursive_depth_search(int current_depth, const int depth_limit, FieldStateTreeNode current_state, std::vector<Field>& result)
{
	current_depth++;
	result.push_back(current_state.get_current_field());
	if (current_state.get_current_field().is_endgame_state())
		return true;

	if (current_depth < depth_limit && current_state.get_next_states_count() > 0)
	{
		for (size_t i = 0; i < current_state.get_next_states_count(); i++)
		{
			FieldStateTreeNode next_state;
			current_state.get_next_field_state_by_index(i, next_state);

			bool search_result = recursive_depth_search(current_depth, depth_limit, next_state, result);
			if (search_result)
			{
				return true;
			}
		}
	}
	result.pop_back();
	return false;
}

bool SolutionAlgorithms::get_answer_by_depth_search(const Field& start_field, const int depth_limit, std::vector<Field>& result)
{
	return recursive_depth_search(0, depth_limit + 1, FieldStateTreeNode(start_field), result);
}

bool SolutionAlgorithms::get_answer_by_width_search(const Field& start_field, std::vector<Field>& result)
{
	std::vector<FieldStateTreeNode*> generated_states;
	generated_states.push_back(new FieldStateTreeNode(start_field));

	// поиск состояния выхода
	size_t current_checked_index = 0;
	while (current_checked_index < generated_states.size()
		&& !generated_states.at(current_checked_index)->get_current_field().is_endgame_state())
	{
		for (size_t i = 0; i < generated_states.at(current_checked_index)->get_next_states_count(); i++)
		{
			generated_states.push_back(new FieldStateTreeNode());
			generated_states.at(current_checked_index)->get_next_field_state_by_index(i, *generated_states.at(generated_states.size() - 1));

			// проверка на то, были в этом состоянии или нет
			bool is_visited = false;
			for (int j = generated_states.size() - 2; j >= 0; j--)
			{
				if ((Field&)generated_states.at(j)->get_current_field() == generated_states.at(generated_states.size() - 1)->get_current_field())
				{
					is_visited = true;
					break;
				}
			}
			if (is_visited)
			{
				delete generated_states.at(generated_states.size() - 1); // освобождаю память по указателю
				generated_states.pop_back(); // и удаляю состояние
			}
		}
		current_checked_index++;
	}

	// возврат по прошлым состояниям и заполнение ответа
	FieldStateTreeNode* state = generated_states.at(current_checked_index);
	bool search_result = state->get_current_field().is_endgame_state();
	if (state->get_current_field().is_endgame_state())
	{
		while (state->get_previous_state() != nullptr)
		{
			result.insert(result.begin(), state->get_current_field());
			state = state->get_previous_state();
		}
		result.insert(result.begin(), state->get_current_field());
	}

	// освобождение памяти
	for (size_t i = 0; i < generated_states.size(); i++)
	{
		delete generated_states.at(i);
	}

	return search_result;
}
