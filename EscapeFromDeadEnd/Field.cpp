#include "Field.h"

bool Field::check_cell_value(FieldCell& cell)
{
	return (cell.row < row_size) && (cell.col < col_size);
}

bool Field::set_player_position(FieldCell cell)
{
	bool result{ check_cell_value(cell) };
	if (result)
	{
		player_posititon = cell;
	}
	return result;
}

bool Field::set_endgame_position(FieldCell cell)
{
	bool result{ check_cell_value(cell) };
	if (result)
	{
		endgame_position = cell;
	}
	return result;
}

bool Field::is_endgame_state()
{
	return player_posititon.row == endgame_position.row && player_posititon.col == endgame_position.col;
}

bool is_blocks_intersected(Block& b1, Block& b2)
{
	const FieldCell& c1{ b1.upper_left_cell_pos };
	const FieldCell& c2{ b2.upper_left_cell_pos };

	auto left_col_1{ c1.col };
	auto right_col_1{ c1.col + b1.col_size - 1 };
	auto left_col_2{ c2.col };
	auto right_col_2{ c2.col + b2.col_size - 1 };

	auto up_row_1{ c1.row };
	auto down_row1{ c1.row + b1.row_size - 1 };
	auto up_row_2{ c2.row };
	auto down_row2{ c2.row + b2.row_size - 1 };

	return !((left_col_1 > right_col_2) || (right_col_1 < left_col_2) || (up_row_1 > down_row2) || (down_row1 < up_row_2));
}

bool Field::add_field_block(Block block)
{
	bool result{ false };

	FieldCell up_left_cell{ block.upper_left_cell_pos };
	FieldCell down_right_cell{ up_left_cell.row + block.row_size - 1, up_left_cell.col + block.col_size - 1 };

	if (check_cell_value(up_left_cell) && check_cell_value(down_right_cell))
	{
		bool intersected{ false };
		for (Block& b : blocks)
		{
			if (is_blocks_intersected(b, block))
			{
				intersected = true;
				break;
			}
		}
		if (!intersected)
		{
			blocks.push_back(block);
			result = true;
		}
	}
	return result;
}

void Field::print_field(std::ostream& out, const Field& field, const field_blocks_t& blocks)
{
	std::vector<std::vector<size_t>> temp;
	temp.assign(field.row_size, std::vector<size_t>(field.col_size));

	int n{ 1 };
	for (auto block : blocks)
	{
		for (size_t i = 0; i < block.row_size; i++)
		{
			for (size_t j = 0; j < block.col_size; j++)
			{
				temp.at(block.upper_left_cell_pos.row + i).at(block.upper_left_cell_pos.col + j) = n;
			}
		}
		n++;
	}

	for (auto row : temp)
	{
		for (auto col : row)
		{
			if (col == 0)
			{
				out << '*';
			}
			else
			{
				out << col;
			}
			out << ' ';
		}
		out << '\n';
	}
}
