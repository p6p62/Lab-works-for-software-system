#pragma once
#include "FieldStateTreeNode.h"

class SolutionAlgorithms
{
private:

public:
	static bool get_answer_by_depth_search(const Field& start_field, const int depth_limit, std::vector<Field>& result);
};

