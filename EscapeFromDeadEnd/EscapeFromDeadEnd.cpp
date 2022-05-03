#include <iostream>
#include "Field.h"
#include "FieldDrawer.h"
#include "FieldStateTreeNode.h"
#include "SolutionAlgorithms.h"
#include <thread>

//#define DEPTH_SEARCH
#define WIDTH_SEARCH

using std::cout;
using std::endl;

Field get_easy_field()
{
	Field f(5, 5);
	f.set_endgame_position({ 4, 4 });
	f.set_player_position({ 0, 1 });
	f.add_field_block({ 1, 2, { 4, 0 } });
	return f;
}

Field get_medium_field()
{
	Field f(5, 5);
	f.set_player_position({ 0, 0 });
	f.set_endgame_position({ 4, 4 });
	f.add_field_block({ 2, 2, {2, 0} });
	f.add_field_block({ 3, 1, {0, 2} });
	return f;
}

Field get_hard_field()
{
	Field f(5, 5);
	f.set_player_position({ 3, 0 });
	f.set_endgame_position({ 4, 4 });
	f.add_field_block({ 1,1,{4,3} });
	f.add_field_block({ 2, 1, {2,2} });
	f.add_field_block({ 1,3,{0,2} });
	f.add_field_block({ 1, 1, {1, 2} });
	f.add_field_block({ 1, 2, {3, 3} });
	f.add_field_block({ 4, 1, {0, 1} });
	f.add_field_block({ 1, 2, {4, 1} });
	return f;
}

void draw_f(const Field& f)
{
	FieldDrawer d(&f);
	d.draw_field();
}

int main()
{
	auto draw_answer_states = [](const std::vector<Field>& escape_way)
	{
		for (int i = 0; i < escape_way.size() - 1; i++)
		{
			const Field& f = escape_way[i];
			std::thread t(draw_f, f);
			t.join();
		}
		std::thread t(draw_f, escape_way.at(escape_way.size() - 1));
		t.join();
	};

#ifdef DEPTH_SEARCH
	std::vector<Field> escape_way;
	bool search_res{ SolutionAlgorithms::get_answer_by_depth_search(get_medium_field(), 15, escape_way) };
	draw_answer_states(escape_way);
#endif // DEPTH_SEARCH

#ifdef WIDTH_SEARCH
	std::vector<Field> escape_way2;
	bool search_res2{ SolutionAlgorithms::get_answer_by_width_search(get_hard_field(), escape_way2) };
	draw_answer_states(escape_way2);
#endif // WIDTH_SEARCH
}