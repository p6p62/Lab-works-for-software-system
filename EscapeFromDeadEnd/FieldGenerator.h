#pragma once
#include "Field.h"

class FieldGenerator
{
public:
	enum class FieldType
	{
		Easy, Medium, Hard, BigEmpty
	};

public:
	static Field get_field(FieldType type);
};

