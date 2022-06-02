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
	 * @param argsFromCommandLine Должен иметь число элементов, кратное 4 (только
	 *                            данные блоков)
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
	 * Создаёт объект поля из данных, пришедших в аргументах вызова из командной
	 * строки
	 * 
	 * @param args Строка с числовыми аргументами. Порядок и описание: <число рядов
	 *             поля> <число столбцов поля> <строка игрока> <столбец игрока>
	 *             <строка точки выхода> <столбец точки выхода> [<<размер блока 1 в
	 *             строках> <размер блока 1 в столбцах> <строка левой верхней ячейки
	 *             блока 1> <столбец левой верхней ячейки блока 1>> ... <<размер
	 *             блока n в строках> <размер блока n в столбцах> <строка левой
	 *             верхней ячейки блока n> <столбец левой верхней ячейки блока n>>]
	 * @return Созданный объект поля или null, если число данных неверное
	 * @throws NumberFormatException Выбросится, если аргументы не числовые
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
