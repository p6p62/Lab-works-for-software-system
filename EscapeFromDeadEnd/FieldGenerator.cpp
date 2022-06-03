#include "FieldGenerator.h"

Field FieldGenerator::get_field(FieldType type)
{
	Field f(5, 5);
	switch (type)
	{
	case FieldGenerator::FieldType::Easy:
		f.set_endgame_position({ 4, 4 });
		f.set_player_position({ 0, 1 });
		f.add_field_block({ 1, 2, { 4, 0 } });
		break;
	case FieldGenerator::FieldType::Medium:
		f.set_player_position({ 0, 0 });
		f.set_endgame_position({ 4, 4 });
		f.add_field_block({ 2, 2, {2, 0} });
		f.add_field_block({ 3, 1, {0, 2} });
		break;
	case FieldGenerator::FieldType::Hard:
		f.set_player_position({ 3, 0 });
		f.set_endgame_position({ 4, 4 });
		f.add_field_block({ 1,1,{4,3} });
		f.add_field_block({ 2, 1, {2,2} });
		f.add_field_block({ 1,3,{0,2} });
		f.add_field_block({ 1, 1, {1, 2} });
		f.add_field_block({ 1, 2, {3, 3} });
		f.add_field_block({ 4, 1, {0, 1} });
		f.add_field_block({ 1, 2, {4, 1} });
		break;
	case FieldGenerator::FieldType::BigEmpty:
		f = Field(15, 15);
		f.set_player_position({ 1, 3 });
		f.set_endgame_position({ 14, 13 });
		break;
	default:
		break;
	}
	return f;
}
