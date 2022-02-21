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