#include <iostream>
#include "Field.h"
#include "FieldDrawer.h"
#include "FieldStateTreeNode.h"
#include "SolutionAlgorithms.h"

using std::cout;
using std::endl;

int main()
{
	//cout << "escape success))" << endl;
	//Field f(20, 30);
	//f.set_player_position({ 5, 10 });
	//f.set_endgame_position({ 19, 4 });
	//f.add_field_block({ 5, 5, {15, 21} });
	//f.add_field_block({ 2, 1, {0, 0} });
	//f.add_field_block({ 1, 2, {10, 24} });
	//f.add_field_block({ 1, 2, {4, 12} });
	//f.add_field_block({ 1, 2, {14, 12} });
	//f.add_field_block({ 1, 2, {3, 14} });
	//f.add_field_block({ 1, 2, {5, 17} });
	//f.add_field_block({ 1, 2, {8, 1} });
	//f.add_field_block({ 1, 2, {14, 2} });
	//f.add_field_block({ 1, 2, {2, 12} });
	//f.add_field_block({ 1, 2, {18, 12} });

	//Field::print_field(cout, f, f.get_field_blocks());

	//// рисование поля
	//FieldDrawer drawer(&f);
	//drawer.draw_field();

	Field f1(5, 5);
	f1.set_player_position({ 2, 3 });
	f1.set_endgame_position({ 0, 0 });
	f1.add_field_block({ 2, 2, {2, 0} });
	f1.add_field_block({ 3, 1, {0, 2} });
	Field::print_field(cout, f1, f1.get_field_blocks());
	FieldDrawer drawer2(&f1);
	drawer2.draw_field();

	std::vector<Field> escape_way;
	bool search_res{ SolutionAlgorithms::get_answer_by_depth_search(f1, 10, escape_way) };

	Field& f_finish = escape_way.at(escape_way.size() - 1);
	Field::print_field(cout, f_finish, f_finish.get_field_blocks());
	FieldDrawer drawer3(&f_finish);
	drawer3.draw_field();
}