package drawer;

public class Cell {
	public final int row;
	public final int col;

	public Cell(int row, int col) {
		super();
		this.row = row;
		this.col = col;
	}

	@Override
	public String toString() {
		return String.format("row = %d, col = %d", row, col);
	}
}
