#include "Field.h"

bool Field::check_cell_value(FieldCell& cell)
{
	return (cell.row < row_size && cell.col < col_size);
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
