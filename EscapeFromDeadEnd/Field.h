#pragma once
#include <vector>
#include <ostream>

struct FieldCell
{
	size_t row = 0;
	size_t col = 0;

	bool operator== (const FieldCell& cell)
	{
		return row == cell.row && col == cell.col;
	}
};

struct Block
{
	size_t row_size = 1;
	size_t col_size = 1;
	FieldCell upper_left_cell_pos;

	bool operator== (const Block& block)
	{
		return row_size == block.row_size && col_size == block.col_size && upper_left_cell_pos == block.upper_left_cell_pos;
	}
};

using field_blocks_t = std::vector<Block>;

class Field
{
private:
	size_t row_size;
	size_t col_size;
	Block player_posititon;
	FieldCell endgame_position;
	field_blocks_t blocks;

	bool check_cell_value(FieldCell& cell);
public:
	Field(size_t rows, size_t cols) { row_size = rows; col_size = cols; }

	bool operator== (const Field& f);

	size_t get_row_size() const { return row_size; }
	size_t get_col_size() const { return col_size; }
	const Block& get_player_block() const { return player_posititon; }
	const FieldCell& get_player_position() const { return player_posititon.upper_left_cell_pos; }
	const FieldCell& get_endgame_position() const { return endgame_position; }
	const field_blocks_t& get_field_blocks() const { return blocks; }
	bool is_endgame_state() const;

	bool set_player_position(FieldCell cell);
	bool set_endgame_position(FieldCell cell);
	bool add_field_block(Block block);
	bool replace_block(const Block& replaced_block, const FieldCell& new_left_up__cell_position);

	static void print_field(std::ostream& out, const Field& field, const field_blocks_t& blocks);
};