#include <iostream>
#include "Field.h"
#include "FieldDrawer.h"
#include "FieldStateTreeNode.h"
#include "SolutionAlgorithms.h"
#include "FieldEvaluator.h"
#include "FieldGenerator.h"
#include <thread>
#include <string>
#include <functional>
#include <chrono>

using std::cout;
using std::endl;
using WorkResult = SolutionAlgorithms::WorkResult;

void draw_f(const Field& f)
{
	FieldDrawer d(&f);
	d.draw_field();
}

std::string convert_perf_metrics_to_string(WorkResult& metrics)
{
	std::string str = "\tПоказатели алгоритма\n";
	str += "Время работы: " + std::to_string(metrics.seconds) + " с.";
	str += "\nМаксимальная глубина поиска: " + std::to_string(metrics.max_depth);
	str += "\nЧисло сгенерированных состояний: " + std::to_string(metrics.all_nodes_count);
	str += "\nДлина решения: " + std::to_string(metrics.answer_length);
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
	//draw_answer_states(escape_way);
}

int main()
{
	using FG = FieldGenerator;
	using FT = FieldGenerator::FieldType;

	const int DEPTH_LIMIT = 23; // ограничение глубины поиска для алгоритма поиска в глубину и подобных

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

	auto branchs_and_borders = [](const Field& f, std::vector<Field>& out, WorkResult* perf_m) -> bool
	{
		return SolutionAlgorithms::get_answer_by_branchs_and_borders(FieldEvaluator::get_move_cost, f, out, perf_m);
	};

	auto equal_costs_strategy = [](const Field& f, std::vector<Field>& out, WorkResult* perf_m) -> bool
	{
		return SolutionAlgorithms::get_answer_by_equal_costs_strategy(FieldEvaluator::get_move_cost, f, out, perf_m);
	};

	//call_search(depth_search, FG::get_field(FT::Medium), "---ПОИСК В ГЛУБИНУ---", true);
	//call_search(width_search, FG::get_field(FT::Hard), "---ПОИСК В ШИРИНУ---", true);
	//call_search(gradient_search, FG::get_field(FT::Easy), "---ГРАДИЕНТНЫЙ СПУСК---", true);
	//call_search(gradient_search, FG::get_field(FT::Medium), "---ГРАДИЕНТНЫЙ СПУСК---", true);
	call_search(gradient_search, FG::get_field(FT::Hard), "---ГРАДИЕНТНЫЙ СПУСК---", true);
	//call_search(gradient_search, FG::get_field(FT::BigEmpty), "---ГРАДИЕНТНЫЙ СПУСК---", true);
	//call_search(branchs_and_borders, FG::get_field(FT::Medium), "---СТРАТЕГИЯ ВЕТВЕЙ И ГРАНИЦ---", true);
	call_search(branchs_and_borders, FG::get_field(FT::Hard), "---СТРАТЕГИЯ ВЕТВЕЙ И ГРАНИЦ---", true);
	call_search(equal_costs_strategy, FG::get_field(FT::Hard), "---СТРАТЕГИЯ РАВНЫХ ЦЕН---", true);
}