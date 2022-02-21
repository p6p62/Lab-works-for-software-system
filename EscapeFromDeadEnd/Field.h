#pragma once
#include <vector>

struct FieldCell
{
	size_t row;
	size_t col;
};

struct Block
{
	size_t row_size;
	size_t col_size;
	FieldCell upper_left_cell_pos;
};

using field_blocks_t = std::vector<Block>;

class Field
{
private:
	size_t row_size;
	size_t col_size;
	FieldCell player_posititon;

	bool check_cell_value(FieldCell& cell);
public:
	Field(size_t rows, size_t cols) { row_size = rows; col_size = cols; }

	size_t get_row_size() { return row_size; }
	size_t get_col_size() { return col_size; }
	const FieldCell& get_player_position() { return player_posititon; }

	bool set_player_position(FieldCell cell);
};