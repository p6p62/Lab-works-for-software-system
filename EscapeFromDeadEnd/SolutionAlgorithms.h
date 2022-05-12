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
	};

public:
	static bool get_answer_by_depth_search(const Field& start_field, const int depth_limit, std::vector<Field>& result, WorkResult* const out_performance = nullptr);
	static bool get_answer_by_width_search(const Field& start_field, std::vector<Field>& result, WorkResult* const out_performance = nullptr);
};

