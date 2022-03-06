#pragma once
#include "Field.h"

class FieldDrawer
{
private:
	const Field* drawed_field = nullptr;
public:
	FieldDrawer(const Field* field);

	void draw_field();
};

