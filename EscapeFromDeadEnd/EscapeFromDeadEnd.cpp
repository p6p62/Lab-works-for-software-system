#include <iostream>
#include "Field.h"
#include "FieldDrawer.h"
#include "FieldStateTreeNode.h"

using std::cout;
using std::endl;

int main()
{
	cout << "escape success))" << endl;
	Field f(20, 30);
	f.set_player_position({ 5, 10 });
	f.set_endgame_position({ 19, 4 });
	f.add_field_block({ 5, 5, {15, 21} });
	f.add_field_block({ 2, 1, {0, 0} });
	f.add_field_block({ 1, 2, {10, 24} });
	f.add_field_block({ 1, 2, {4, 12} });
	f.add_field_block({ 1, 2, {14, 12} });
	f.add_field_block({ 1, 2, {3, 14} });
	f.add_field_block({ 1, 2, {5, 17} });
	f.add_field_block({ 1, 2, {8, 1} });
	f.add_field_block({ 1, 2, {14, 2} });
	f.add_field_block({ 1, 2, {2, 12} });
	f.add_field_block({ 1, 2, {18, 12} });

	Field::print_field(cout, f, f.get_field_blocks());

	// рисование поля
	FieldDrawer drawer(&f);
	drawer.draw_field();

	FieldStateTreeNode n{ nullptr, 0, f };
}