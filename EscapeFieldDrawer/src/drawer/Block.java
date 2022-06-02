package drawer;

public class Block {
	public final int rowSize;
	public final int colSize;
	public final Cell upperLeftCell;

	public Block(int rowSize, int colSize, Cell upperLeftCell) {
		super();
		this.rowSize = rowSize;
		this.colSize = colSize;
		this.upperLeftCell = upperLeftCell;
	}

	@Override
	public String toString() {
		return String.format("r_size = %d, c_size = %d, row_up_left = %d, col_up_left = %d", rowSize, colSize,
				upperLeftCell.row, upperLeftCell.col);
	}
}
