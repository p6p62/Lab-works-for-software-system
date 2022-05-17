#include "SolutionAlgorithms.h"

bool recursive_depth_search(int current_depth, const int depth_limit, FieldStateTreeNode current_state, std::vector<Field>& result, const std::function<double(const Field&)>& evaluating_function = nullptr)
{
	current_depth++;
	result.push_back(current_state.get_current_field());
	if (current_state.get_current_field().is_endgame_state())
		return true;

	if (current_depth < depth_limit && current_state.get_next_states_count() > 0)
	{
		// сортировка для спуска по градиенту
		if (evaluating_function != nullptr)
			current_state.sort_next_states_by_descend(evaluating_function);

		for (size_t i = 0; i < current_state.get_next_states_count(); i++)
		{
			FieldStateTreeNode next_state;

			current_state.get_next_field_state_by_index(i, next_state);

			bool search_result = recursive_depth_search(current_depth, depth_limit, next_state, result, evaluating_function);
			if (search_result)
			{
				return true;
			}
		}
	}
	result.pop_back();
	return false;
}

bool SolutionAlgorithms::get_answer_by_depth_search(const Field& start_field, const int depth_limit, std::vector<Field>& result, WorkResult* const out_performance)
{
	// для замеров производительности
	bool is_need_performance_measure = out_performance != nullptr;
	clock_t start_clocks;
	if (is_need_performance_measure)
	{
		start_clocks = clock();
	}

	bool search_result = recursive_depth_search(0, depth_limit + 1, FieldStateTreeNode(start_field), result);

	// заполнение данных о производительности
	if (is_need_performance_measure)
	{
		out_performance->seconds = (clock() - start_clocks) / (double)CLOCKS_PER_SEC;
	}

	return search_result;
}

bool SolutionAlgorithms::get_answer_by_width_search(const Field& start_field, std::vector<Field>& result, WorkResult* const out_performance)
{
	// для замеров производительности
	bool is_need_performance_measure = out_performance != nullptr;
	clock_t start_clocks;
	if (is_need_performance_measure)
	{
		start_clocks = clock();
	}

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

	// заполнение данных о производительности
	if (is_need_performance_measure)
	{
		out_performance->seconds = (clock() - start_clocks) / (double)CLOCKS_PER_SEC;
	}

	return search_result;
}

bool SolutionAlgorithms::get_answer_by_gradient_descent(const std::function<double(const Field&)>& evaluating_function, const Field& start_field, const int depth_limit, std::vector<Field>& result, WorkResult* const out_performance)
{
	// TODO
	// для замеров производительности
	bool is_need_performance_measure = out_performance != nullptr;
	clock_t start_clocks;
	if (is_need_performance_measure)
	{
		start_clocks = clock();
	}

	bool search_result = recursive_depth_search(0, depth_limit + 1, FieldStateTreeNode(start_field), result, evaluating_function);

	// заполнение данных о производительности
	if (is_need_performance_measure)
	{
		out_performance->seconds = (clock() - start_clocks) / (double)CLOCKS_PER_SEC;
	}

	return search_result;
}
