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

	std::vector<std::unique_ptr<FieldStateTreeNode>> generated_states;
	generated_states.push_back(std::make_unique<FieldStateTreeNode>(FieldStateTreeNode(start_field)));

	// поиск состояния выхода
	size_t current_checked_index = 0;
	while (current_checked_index < generated_states.size()
		&& !generated_states.at(current_checked_index)->get_current_field().is_endgame_state())
	{
		for (size_t i = 0; i < generated_states.at(current_checked_index)->get_next_states_count(); i++)
		{
			generated_states.push_back(std::make_unique<FieldStateTreeNode>(FieldStateTreeNode()));
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
				generated_states.pop_back(); // удаляю состояние, автоматически освобождается память по указателю
			}
		}
		current_checked_index++;
	}

	// возврат по прошлым состояниям и заполнение ответа
	FieldStateTreeNode* state = generated_states.at(current_checked_index).get();
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

	// заполнение данных о производительности
	if (is_need_performance_measure)
	{
		out_performance->seconds = (clock() - start_clocks) / (double)CLOCKS_PER_SEC;
	}

	return search_result;
}

bool SolutionAlgorithms::get_answer_by_gradient_descent(const std::function<double(const Field&)>& evaluating_function, const Field& start_field, const int depth_limit, std::vector<Field>& result, WorkResult* const out_performance)
{
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

bool SolutionAlgorithms::get_answer_by_branchs_and_borders(const std::function<double(const Field&, const Field&)>& cost_function, const Field& start_field, std::vector<Field>& result, WorkResult* const out_performance)
{
	struct StateWithCurrentWayCost
	{
		FieldStateTreeNode state;
		int way_cost;
	};

	// для замеров производительности
	bool is_need_performance_measure = out_performance != nullptr;
	clock_t start_clocks;
	if (is_need_performance_measure)
	{
		start_clocks = clock();
	}

	std::vector<std::unique_ptr<StateWithCurrentWayCost>> generated_states;
	generated_states.push_back(std::make_unique<StateWithCurrentWayCost>(StateWithCurrentWayCost{ FieldStateTreeNode(start_field), 0 }));

	// поиск состояния выхода
	size_t current_checked_index = 0;
	while (current_checked_index < generated_states.size()
		&& !generated_states.at(current_checked_index)->state.get_current_field().is_endgame_state())
	{
		for (size_t i = 0; i < generated_states.at(current_checked_index)->state.get_next_states_count(); i++)
		{
			// создаю новое состояние с начальной стоимостью, равной стоимости родителя, сразу добавляю его последним
			generated_states.push_back(std::make_unique<StateWithCurrentWayCost>(
				StateWithCurrentWayCost{ FieldStateTreeNode(), generated_states.at(current_checked_index)->way_cost }));
			generated_states.at(current_checked_index)->state.get_next_field_state_by_index(i, generated_states.at(generated_states.size() - 1)->state);

			// сразу после вычисления нового состояния обновляю суммарную стоимость ходов
			generated_states.at(generated_states.size() - 1)->way_cost +=
				cost_function(generated_states.at(current_checked_index)->state.get_current_field(),
					generated_states.at(generated_states.size() - 1)->state.get_current_field());

			// проверка на то, были в этом состоянии или нет
			bool is_visited = false;
			for (int j = generated_states.size() - 2; j >= 0; j--)
			{
				if ((Field&)generated_states.at(j)->state.get_current_field() == generated_states.at(generated_states.size() - 1)->state.get_current_field())
				{
					is_visited = true;
					break;
				}
			}
			if (is_visited)
			{
				generated_states.pop_back(); // удаляю добавленное состояние, автоматически освобождается память по указателю
			}
		}

		// перераспределение состояний. На место (current_checked_index + 1) ставится состояние, для которого будет наименьшая итоговая стоимость
		int min_sum_cost_state_index = (current_checked_index + 1 >= generated_states.size()) ? current_checked_index : current_checked_index + 1;
		for (size_t i = current_checked_index + 1; i < generated_states.size(); i++)
		{
			// в цикле ищется следующее состояние с наименьшей итоговой стоимостью
			if (generated_states.at(i)->way_cost < generated_states.at(min_sum_cost_state_index)->way_cost)
				min_sum_cost_state_index = i;
		}

		// перестановка состояний
		auto temp = generated_states.at(current_checked_index + 1).release();
		generated_states.at(current_checked_index + 1).reset(generated_states.at(min_sum_cost_state_index).release());
		generated_states.at(min_sum_cost_state_index).reset(temp);

		current_checked_index++;
	}

	// возврат по прошлым состояниям и заполнение ответа
	FieldStateTreeNode* state = &generated_states.at(current_checked_index)->state;
	bool search_result = state->get_current_field().is_endgame_state();
	if (search_result)
	{
		while (state->get_previous_state() != nullptr)
		{
			result.insert(result.begin(), state->get_current_field());
			state = state->get_previous_state();
		}
		result.insert(result.begin(), state->get_current_field());
	}

	// заполнение данных о производительности
	if (is_need_performance_measure)
	{
		out_performance->seconds = (clock() - start_clocks) / (double)CLOCKS_PER_SEC;
	}

	return search_result;
}

bool SolutionAlgorithms::get_answer_by_equal_costs_strategy(const std::function<double(const Field&, const Field&)>& cost_function, const Field& start_field, std::vector<Field>& result, WorkResult* const out_performance)
{
	struct StateWithCurrentWayCost
	{
		FieldStateTreeNode state;
		int way_cost;
	};

	// для замеров производительности
	bool is_need_performance_measure = out_performance != nullptr;
	clock_t start_clocks;
	if (is_need_performance_measure)
	{
		start_clocks = clock();
	}

	std::vector<std::unique_ptr<StateWithCurrentWayCost>> generated_states;
	generated_states.push_back(std::make_unique<StateWithCurrentWayCost>(StateWithCurrentWayCost{ FieldStateTreeNode(start_field), 0 }));

	// поиск состояния выхода
	size_t current_checked_index = 0;
	while (current_checked_index < generated_states.size()
		&& !generated_states.at(current_checked_index)->state.get_current_field().is_endgame_state())
	{
		for (size_t i = 0; i < generated_states.at(current_checked_index)->state.get_next_states_count(); i++)
		{
			// создаю новое состояние с начальной стоимостью, равной стоимости родителя, сразу добавляю его последним
			generated_states.push_back(std::make_unique<StateWithCurrentWayCost>(
				StateWithCurrentWayCost{ FieldStateTreeNode(), generated_states.at(current_checked_index)->way_cost }));
			generated_states.at(current_checked_index)->state.get_next_field_state_by_index(i, generated_states.at(generated_states.size() - 1)->state);

			// сразу после вычисления нового состояния обновляю суммарную стоимость ходов
			generated_states.at(generated_states.size() - 1)->way_cost +=
				cost_function(generated_states.at(current_checked_index)->state.get_current_field(),
					generated_states.at(generated_states.size() - 1)->state.get_current_field());

			// проверка на то, были в этом состоянии или нет
			bool is_visited = false;
			int index_visited = 0; // для сохранения состояния, уже существующего в наборе
			for (int j = generated_states.size() - 2; j >= 0; j--)
			{
				if ((Field&)generated_states.at(j)->state.get_current_field() == generated_states.at(generated_states.size() - 1)->state.get_current_field())
				{
					is_visited = true;
					index_visited = j;
					break;
				}
			}
			if (is_visited)
			{
				// если обрабатываемое состояние уже встречалось, то происходит замена уже сохранённой структуры (узел, стоимость)
				// на ту, которая обрабатывается сейчас, в том случае, если она имеет лучшую оценку
				// добавленное в конец состояние в любом случае будет удалено, так как 2 разных пути до одного места хранить нельзя
				if (generated_states.at(index_visited)->way_cost > generated_states.at(generated_states.size() - 1)->way_cost)
				{
					generated_states.at(index_visited).reset(generated_states.at(generated_states.size() - 1).release());
				}
				generated_states.pop_back(); // удаляю добавленное состояние, автоматически освобождается память по указателю

			}
		}

		// перераспределение состояний. На место (current_checked_index + 1) ставится состояние, для которого будет наименьшая итоговая стоимость
		int min_sum_cost_state_index = (current_checked_index + 1 >= generated_states.size()) ? current_checked_index : current_checked_index + 1;
		for (size_t i = current_checked_index + 1; i < generated_states.size(); i++)
		{
			// в цикле ищется следующее состояние с наименьшей итоговой стоимостью
			if (generated_states.at(i)->way_cost < generated_states.at(min_sum_cost_state_index)->way_cost)
				min_sum_cost_state_index = i;
		}

		// перестановка состояний
		auto temp = generated_states.at(current_checked_index + 1).release();
		generated_states.at(current_checked_index + 1).reset(generated_states.at(min_sum_cost_state_index).release());
		generated_states.at(min_sum_cost_state_index).reset(temp);

		current_checked_index++;
	}

	// возврат по прошлым состояниям и заполнение ответа
	FieldStateTreeNode* state = &generated_states.at(current_checked_index)->state;
	bool search_result = state->get_current_field().is_endgame_state();
	if (search_result)
	{
		while (state->get_previous_state() != nullptr)
		{
			result.insert(result.begin(), state->get_current_field());
			state = state->get_previous_state();
		}
		result.insert(result.begin(), state->get_current_field());
	}

	// заполнение данных о производительности
	if (is_need_performance_measure)
	{
		out_performance->seconds = (clock() - start_clocks) / (double)CLOCKS_PER_SEC;
	}

	return search_result;
}
