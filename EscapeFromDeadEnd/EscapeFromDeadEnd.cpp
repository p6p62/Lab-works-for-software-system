#include <iostream>
#include "Field.h"
#include "FieldDrawer.h"
#include "FieldStateTreeNode.h"
#include "SolutionAlgorithms.h"
#include "FieldEvaluator.h"
#include <thread>
#include <string>
#include <functional>
#include <chrono>

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

Field get_big_empty_field()
{
	Field f(15, 15);
	f.set_player_position({ 1, 3 });
	f.set_endgame_position({ 14, 13 });
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

void call_search
(
	std::function<bool(const Field&, std::vector<Field>&, WorkResult*)> const& search_function, // вызываемый алгоритм
	const Field& start_field, // начальное состояние
	const std::string& algorithm_name, // имя алгоритма для отображения
	bool need_performance_measures // управление расчетом показателями производительности
)
{
	std::vector<Field> escape_way;
	WorkResult perf_stats;

	// вызов отображения сообщения в отдельном потоке
	size_t notification_counter = 0;
	bool isFinish{ false };
	std::thread thr_message([&notification_counter, &isFinish]()
		{
			using namespace std::chrono_literals;
			while (!isFinish)
			{
				cout << "Всё ещё выполняется... Прошло " << notification_counter << " с.\n";
				const size_t message_delay_seconds = 3;
				notification_counter += message_delay_seconds;
				std::this_thread::sleep_for(std::chrono::seconds(message_delay_seconds));
			}
		});
	thr_message.detach();

	// вызов алгоритма
	search_function(start_field, escape_way, (need_performance_measures) ? &perf_stats : nullptr);

	// остановка потока сообщений
	isFinish = true;

	// вывод результатов
	cout << endl << algorithm_name << endl;
	if (need_performance_measures)
		cout << convert_perf_metrics_to_string(perf_stats) << endl << endl;
	draw_answer_states(escape_way);
}

int main()
{
	const int DEPTH_LIMIT = 40; // ограничение глубины поиска для алгоритма поиска в глубину и подобных

	setlocale(LC_ALL, "Russian"); // чтобы был русский язык, но разделитель чисел - точка
	setlocale(LC_NUMERIC, "English");

	auto depth_search = [DEPTH_LIMIT](const Field& f, std::vector<Field>& out, WorkResult* perf_m) -> bool
	{
		return SolutionAlgorithms::get_answer_by_depth_search(f, DEPTH_LIMIT, out, perf_m);
	};

	auto width_search = [](const Field& f, std::vector<Field>& out, WorkResult* perf_m) -> bool
	{
		return SolutionAlgorithms::get_answer_by_width_search(f, out, perf_m);
	};

	auto gradient_search = [DEPTH_LIMIT](const Field& f, std::vector<Field>& out, WorkResult* perf_m) -> bool
	{
		return SolutionAlgorithms::get_answer_by_gradient_descent(FieldEvaluator::get_evaluate, f, DEPTH_LIMIT, out, perf_m);
	};

	//call_search(depth_search, get_medium_field(), "---ПОИСК В ГЛУБИНУ---", true);
	//call_search(width_search, get_medium_field(), "---ПОИСК В ШИРИНУ---", true);
	call_search(gradient_search, get_easy_field(), "---ГРАДИЕНТНЫЙ СПУСК---", true);
	call_search(gradient_search, get_medium_field(), "---ГРАДИЕНТНЫЙ СПУСК---", true);
	call_search(gradient_search, get_hard_field(), "---ГРАДИЕНТНЫЙ СПУСК---", true);
	call_search(gradient_search, get_big_empty_field(), "---ГРАДИЕНТНЫЙ СПУСК---", true);
}