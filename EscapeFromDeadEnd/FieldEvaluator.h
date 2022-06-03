#pragma once
#include "Field.h"

class FieldEvaluator
{
public:
	static double get_evaluate(const Field& field);
	static double get_move_cost(const Field& old_field, const Field& new_field);
};

