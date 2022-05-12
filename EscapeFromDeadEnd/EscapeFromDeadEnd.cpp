#include <iostream>
#include "Field.h"
#include "FieldDrawer.h"
#include "FieldStateTreeNode.h"
#include "SolutionAlgorithms.h"
#include <thread>
#include <string>

#define DEPTH_SEARCH
#define WIDTH_SEARCH
#define PERFORMANCE_MEASURES

using std::cout;
using std::endl;
using WorkResult = SolutionAlgorithms::WorkResult;

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

std::string convert_perf_metrics_to_string(WorkResult& metrics)
{
	std::string str = "\tПоказатели алгоритма\n";
	str += "Время работы: " + std::to_string(metrics.seconds) + " с.";
	return str;
}

void draw_answer_states(const std::vector<Field>& escape_way)
{
	for (int i = 0; i < escape_way.size(); i++)
	{
		const Field& f = escape_way[i];
		draw_f(f);
	}
};

int main()
{
	setlocale(LC_ALL, "Russian"); // русский язык, но разделитель чисел - точка
	setlocale(LC_NUMERIC, "English");

#ifdef DEPTH_SEARCH
	std::vector<Field> escape_way;
	bool search_res_depth;
	cout << "---ПОИСК В ГЛУБИНУ---\n";
#ifdef PERFORMANCE_MEASURES
	WorkResult performance_metrics_d;
	search_res_depth = SolutionAlgorithms::get_answer_by_depth_search(get_easy_field(), 15, escape_way, &performance_metrics_d);
	cout << convert_perf_metrics_to_string(performance_metrics_d) << endl << endl;
#else
	search_res_depth = SolutionAlgorithms::get_answer_by_depth_search(get_medium_field(), 15, escape_way);
#endif // PERFORMANCE_MEASURES
	draw_answer_states(escape_way);
#endif // DEPTH_SEARCH

#ifdef WIDTH_SEARCH
	std::vector<Field> escape_way2;
	bool search_res_width;
	cout << "---ПОИСК В ШИРИНУ---\n";
#ifdef PERFORMANCE_MEASURES
	WorkResult performance_metrics;
	search_res_width = SolutionAlgorithms::get_answer_by_width_search(get_medium_field(), escape_way2, &performance_metrics);
	cout << convert_perf_metrics_to_string(performance_metrics) << endl << endl;
#else
	search_res_width = SolutionAlgorithms::get_answer_by_width_search(get_medium_field(), escape_way2);
#endif // PERFORMANCE_MEASURES
	draw_answer_states(escape_way2);
#endif // WIDTH_SEARCH
}