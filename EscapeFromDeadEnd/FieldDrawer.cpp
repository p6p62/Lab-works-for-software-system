#include "FieldDrawer.h"
#include <string>
#include <iostream>

FieldDrawer::FieldDrawer(const Field* field)
{
	this->drawed_field = field;
}

std::string convert_field_to_string(const Field& field)
{
	std::string string_field{ "" };

	string_field.append(std::to_string(field.get_row_size())).push_back(' ');
	string_field.append(std::to_string(field.get_col_size())).push_back(' ');
	string_field.append(std::to_string(field.get_player_position().row)).push_back(' ');
	string_field.append(std::to_string(field.get_player_position().col)).push_back(' ');
	string_field.append(std::to_string(field.get_endgame_position().row)).push_back(' ');
	string_field.append(std::to_string(field.get_endgame_position().col)).push_back(' ');

	for (auto& block : field.get_field_blocks())
	{
		string_field.append(std::to_string(block.row_size)).push_back(' ');
		string_field.append(std::to_string(block.col_size)).push_back(' ');
		string_field.append(std::to_string(block.upper_left_cell_pos.row)).push_back(' ');
		string_field.append(std::to_string(block.upper_left_cell_pos.col)).push_back(' ');
	}

	return string_field;
}

void FieldDrawer::draw_field()
{
	std::string call_string_base("java -classpath \"C:\\Users\\Boris\\Desktop\\РаDICK\\Проектирование программных систем 2\\Выход из тупика\\EscapeFromDeadEnd\\EscapeFieldDrawer\\bin\" drawer.Drawer ");

	if (this->drawed_field != nullptr)
	{
		auto field_to_str{ convert_field_to_string(*this->drawed_field) };
		system(call_string_base.append(field_to_str).c_str());
	}
}
