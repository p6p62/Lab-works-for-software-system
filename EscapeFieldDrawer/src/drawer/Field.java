package drawer;

public class Field {
	public final int fieldHeight;
	public final int fieldWidth;
	public final Cell playerPosition;
	public final Cell endGamePoint;
	public final Block[] blocks;

	public Field(int fieldHeight, int fieldWidth, Cell playerPosition, Cell endGamePoint, Block[] blocks) {
		super();
		this.fieldHeight = fieldHeight;
		this.fieldWidth = fieldWidth;
		this.playerPosition = playerPosition;
		this.endGamePoint = endGamePoint;
		this.blocks = blocks;
	}

	@Override
	public String toString() {
		StringBuilder result = new StringBuilder();
		result.append(String.format("Height: %d\nWidth: %d\nPlayer (row, col): %d %d\nEnd point (row, col): %d %d\n",
				this.fieldHeight, this.fieldWidth, this.playerPosition.row, this.playerPosition.col,
				this.endGamePoint.row, this.endGamePoint.col));
		if (blocks != null) {
			for (Block b : blocks) {
				result.append(b).append('\n');
			}
		}
		return result.toString();
	}
}
