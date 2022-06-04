#pragma once
#include <time.h>
#include "FieldStateTreeNode.h"

class SolutionAlgorithms
{
private:

public:
	struct WorkResult
	{
		double seconds = 0;
		size_t max_depth = 0;
		size_t answer_length = 0;
		size_t all_nodes_count = 0;
	};

public:
	static bool get_answer_by_depth_search(const Field& start_field, const int depth_limit, std::vector<Field>& result, WorkResult* const out_performance = nullptr);
	static bool get_answer_by_width_search(const Field& start_field, std::vector<Field>& result, WorkResult* const out_performance = nullptr);
	static bool get_answer_by_gradient_descent(const std::function<double(const Field&)>& evaluating_function, const Field& start_field, const int depth_limit, std::vector<Field>& result, WorkResult* const out_performance = nullptr);
	static bool get_answer_by_branchs_and_borders(const std::function<double(const Field&, const Field&)>& cost_function, const Field& start_field, std::vector<Field>& result, WorkResult* const out_performance = nullptr);
	static bool get_answer_by_equal_costs_strategy(const std::function<double(const Field&, const Field&)>& cost_function, const Field& start_field, std::vector<Field>& result, WorkResult* const out_performance = nullptr);
};

