package drawer;

import java.util.Arrays;

public class DataParcer {
	private static Cell parseCell(String pRow, String pCol) {
		int row = Integer.parseInt(pRow);
		int col = Integer.parseInt(pCol);
		return new Cell(row, col);
	}

	private static Block parseBlock(String pRowSize, String pColSize, String pRow, String pCol) {
		int rowSize = Integer.parseInt(pRowSize);
		int colSize = Integer.parseInt(pColSize);
		Cell cell = parseCell(pRow, pCol);
		return new Block(rowSize, colSize, cell);
	}

	/**
	 * 
	 * @param argsFromCommandLine ������ ����� ����� ���������, ������� 4 (������
	 *                            ������ ������)
	 * @return
	 */
	private static Block[] getBlocksFromArgs(String[] argsFromCommandLine) {
		Block[] blocks = new Block[argsFromCommandLine.length / 4];

		for (int i = 0; i < argsFromCommandLine.length; i += 4) {
			String rowSize = argsFromCommandLine[i + 0];
			String colSize = argsFromCommandLine[i + 1];
			String upLeftCellRow = argsFromCommandLine[i + 2];
			String upLeftCellCol = argsFromCommandLine[i + 3];
			blocks[i / 4] = parseBlock(rowSize, colSize, upLeftCellRow, upLeftCellCol);
		}

		return blocks;
	}

	/**
	 * ������ ������ ���� �� ������, ��������� � ���������� ������ �� ���������
	 * ������
	 * 
	 * @param args ������ � ��������� �����������. ������� � ��������: <����� �����
	 *             ����> <����� �������� ����> <������ ������> <������� ������>
	 *             <������ ����� ������> <������� ����� ������> [<<������ ����� 1 �
	 *             �������> <������ ����� 1 � ��������> <������ ����� ������� ������
	 *             ����� 1> <������� ����� ������� ������ ����� 1>> ... <<������
	 *             ����� n � �������> <������ ����� n � ��������> <������ �����
	 *             ������� ������ ����� n> <������� ����� ������� ������ ����� n>>]
	 * @return ��������� ������ ���� ��� null, ���� ����� ������ ��������
	 * @throws NumberFormatException ����������, ���� ��������� �� ��������
	 */
	public static Field getFieldFromArgs(String[] args) throws NumberFormatException {
		final int ARGS_BEFORE_BLOCKS_COUNT = 6;

		Field resultField = null;
		if (args.length >= ARGS_BEFORE_BLOCKS_COUNT && (args.length - ARGS_BEFORE_BLOCKS_COUNT) % 4 == 0) {
			int fieldHeight = Integer.parseInt(args[0]);
			int fieldWidth = Integer.parseInt(args[1]);
			Cell player = parseCell(args[2], args[3]);
			Cell endPoint = parseCell(args[4], args[5]);
			Block[] blocks = null;

			if ((args.length - ARGS_BEFORE_BLOCKS_COUNT) / 4 >= 1) {
				blocks = getBlocksFromArgs(Arrays.copyOfRange(args, ARGS_BEFORE_BLOCKS_COUNT, args.length));
			}

			resultField = new Field(fieldHeight, fieldWidth, player, endPoint, blocks);
		}

		return resultField;
	}
}
