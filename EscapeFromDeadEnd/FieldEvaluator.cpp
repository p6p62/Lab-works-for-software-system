#include "FieldEvaluator.h"

// оценочная функция обратно пропорциональна манхэттенскому расстоянию между игроком и выходом
// 1) игрок ближе к точке выхода - оценка выше
//#define SIMPLE_MANHATTAN

// функция та же, что и прошлая, но с дополнительным слагаемым в виде суммы расстояний от блоков до точки выхода
// 1) игрок ближе к точке выхода - оценка выше
// 2) блоки дальше от точки выхода - оценка выше
//#define MANHATTAN_WITH_BLOCKS_PUSHED

// в оценочной функции учитывается:
// 1) манхэттенское расстояние от игрока до точки выхода,
// 2) расстояния от блоков поля до точки выхода
// 3) расстояния от блоков до самого игрока
// при этом в функции есть дополнительное слагаемое, добавляющее сильный приоритет тем полям, в которых
// точка выхода более "расчищена" от блоков и одновременно с этим позиция игрока при этом близка к ней
// Влияние ситуации на оценку:
// 1) игрок ближе к точке выхода - оценка выше
// 2) блоки дальше от точки выхода - оценка выше
// 3) блоки дальше от игрока - оценка выше
// 4) игрок ближе к полю, а блоки дальше - оценка сильно(!) выше 
#define MANHATTAN_WITH_CLEAR_WAY

double manhattan_simple(const Field& field)
{
	FieldCell p_pos = field.get_player_position();
	FieldCell e_pos = field.get_endgame_position();
	int manhattan_distance = abs((int)p_pos.col - (int)e_pos.col) + abs((int)p_pos.row - (int)e_pos.row);
	return 1.0 / manhattan_distance;
}

double manhattan_with_block_pushed(const Field& field)
{
	FieldCell p_pos = field.get_player_position();
	FieldCell e_pos = field.get_endgame_position();
	size_t blocks_distance_sum = 0;
	for (const Block& b : field.get_field_blocks())
	{
		blocks_distance_sum += abs((int)b.upper_left_cell_pos.row - (int)e_pos.row) + abs((int)b.upper_left_cell_pos.col - (int)e_pos.col);
	}
	int manhattan_distance = abs((int)p_pos.col - (int)e_pos.col) + abs((int)p_pos.row - (int)e_pos.row);
	return 1.0 / manhattan_distance + blocks_distance_sum;
}

double manhattan_smart(const Field& field)
{
	FieldCell p_pos = field.get_player_position();
	FieldCell e_pos = field.get_endgame_position();

	size_t blocks_distance_to_exit_sum = 0;
	size_t blocks_distance_to_player_sum = 0;
	for (const Block& b : field.get_field_blocks())
	{
		const FieldCell& b_cell = b.upper_left_cell_pos;
		blocks_distance_to_exit_sum += abs((int)b_cell.row - (int)e_pos.row) + abs((int)b_cell.col - (int)e_pos.col);
		blocks_distance_to_player_sum += abs((int)b_cell.row - (int)p_pos.row) + abs((int)b_cell.col - (int)p_pos.col);
	}

	int manhattan_distance_to_exit = abs((int)p_pos.col - (int)e_pos.col) + abs((int)p_pos.row - (int)e_pos.row);
	return 1.0 / manhattan_distance_to_exit + blocks_distance_to_exit_sum / (double)manhattan_distance_to_exit
		+ blocks_distance_to_exit_sum + blocks_distance_to_player_sum;
}

double FieldEvaluator::get_evaluate(const Field& field)
{
#ifdef SIMPLE_MANHATTAN
	return manhattan_simple(field);
#endif // SIMPLE_MANHATTAN

#ifdef MANHATTAN_WITH_BLOCKS_PUSHED
	return manhattan_with_block_pushed(field);
#endif // MANHATTAN_WITH_BLOCKS_PUSHED

#ifdef MANHATTAN_WITH_CLEAR_WAY
	return manhattan_smart(field);
#endif // MANHATTAN_WITH_CLEAR_WAY
}

double FieldEvaluator::get_move_cost(const Field& old_field, const Field& new_field)
{
	return  abs((int)(new_field.get_player_position().row - new_field.get_endgame_position().row)+ 
		(int)(new_field.get_player_position().col - new_field.get_endgame_position().col)) - 
			abs((int)(old_field.get_player_position().row - old_field.get_endgame_position().row)+
				(int)(old_field.get_player_position().col - old_field.get_endgame_position().col));
}
