package drawer;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Toolkit;
import java.awt.geom.Ellipse2D;
import java.awt.geom.Line2D;
import java.awt.geom.Rectangle2D;

import javax.swing.JFrame;
import javax.swing.JPanel;

public class Drawer extends JFrame {
	private static final long serialVersionUID = 1L;

	public static final int ADDITIONAL_WIDTH_SIZE = 20;
	public static final int ADDITIONAL_HEIGHT_SIZE = 46;
	
	public static final int MAX_SCREEN_WIDTH = 1300;
	public static final int MAX_SCREEN_HEIGHT = 700;
	public static final int DEFAULT_SCREEN_WIDTH = 700;
	public static final int DEFAULT_SCREEN_HEIGHT = 500;

	private int screenWidth = DEFAULT_SCREEN_WIDTH;
	private int screenHeight = DEFAULT_SCREEN_HEIGHT;
	private float cellSize; // размер €чейки пол€ (без учета толщины разделительной линии)
	private float blockPadding = 3; // отступ от контура блока до разделительной линии между €чейками
	private float lineWidth = 3; // толщина разделительной линии между €чейками
	private Line2D[] rowGridLines = null;
	private Line2D[] colGridLines = null;

	private Field drawedField = null;

	public Drawer() {
		Drawer drawer = this;
		drawer.configureWindow();

		drawer.add(new JPanel() {
			private static final long serialVersionUID = 1L;

			@Override
			protected void paintComponent(Graphics g) {
				super.paintComponent(g);
				Graphics2D graphics2d = (Graphics2D) g;

				if (drawer.drawedField != null) {
					drawer.drawFieldGrid(graphics2d);
					drawer.drawFieldBlocks(graphics2d);
					drawer.drawPlayer(graphics2d);
					drawer.drawEndgamePoint(graphics2d);
				}
			}
		});
	}

	public Drawer(Field field) {
		this();
		this.setField(field);
	}

	public void setField(Field field) {
		this.drawedField = field;

		if (drawedField != null) {
			recalculateWindowParameters();
			configureWindow();
		}
	}

	private void recalculateWindowParameters() {
		if (this.drawedField == null)
			return;

		float scaleCoefficient = Math.min(MAX_SCREEN_WIDTH / this.drawedField.fieldWidth,
				MAX_SCREEN_HEIGHT / this.drawedField.fieldHeight);
		cellSize = scaleCoefficient;

		if (cellSize <= 3) {
			lineWidth = 1;
			blockPadding = 0;
		}

		screenWidth = (int) (drawedField.fieldWidth * scaleCoefficient);
		screenHeight = (int) (drawedField.fieldHeight * scaleCoefficient);
	}

	private void configureWindow() {
		this.setSize(this.screenWidth + ADDITIONAL_WIDTH_SIZE, this.screenHeight + ADDITIONAL_HEIGHT_SIZE);
		this.setTitle("¬ыход из тупика");
		this.setLocation((Toolkit.getDefaultToolkit().getScreenSize().width - this.screenWidth) / 2,
				(Toolkit.getDefaultToolkit().getScreenSize().height - this.screenHeight) / 2);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}

	private void drawFieldGrid(Graphics2D graphics2d) {
		final Color GRID_COLOR = Color.BLACK;

		if (rowGridLines == null || colGridLines == null) {
			final int ROW_LINES_COUNT = this.drawedField.fieldHeight + 1;
			final int COL_LINES_COUNT = this.drawedField.fieldWidth + 1;

			rowGridLines = new Line2D.Float[ROW_LINES_COUNT];
			colGridLines = new Line2D.Float[COL_LINES_COUNT];
			for (int i = 0; i < ROW_LINES_COUNT; i++) {
				rowGridLines[i] = new Line2D.Float(0, i * cellSize, this.getWidth(), i * cellSize);
			}
			for (int i = 0; i < COL_LINES_COUNT; i++) {
				colGridLines[i] = new Line2D.Float(i * cellSize, 0, i * cellSize, this.getHeight());
			}
		}

		graphics2d.setStroke(new BasicStroke(lineWidth));
		graphics2d.setColor(GRID_COLOR);
		for (Line2D line : this.rowGridLines) {
			graphics2d.draw(line);
		}
		for (Line2D line : this.colGridLines) {
			graphics2d.draw(line);
		}
	}

	private void drawFieldBlocks(Graphics2D graphics2d) {
		final Color BLOCK_COLOR = Color.GREEN;

		if (drawedField.blocks != null) {
			graphics2d.setColor(BLOCK_COLOR);
			for (Block block : drawedField.blocks) {
				float x = block.upperLeftCell.col * cellSize + blockPadding;
				float y = block.upperLeftCell.row * cellSize + blockPadding;
				float w = block.colSize * cellSize - 1.5F * blockPadding;
				float h = block.rowSize * cellSize - 1.5F * blockPadding;
				graphics2d.fill(new Rectangle2D.Float(x, y, w, h));
			}
		}
	}

	private void drawPlayer(Graphics2D graphics2d) {
		final Color PLAYER_COLOR = Color.GRAY;

		float x = drawedField.playerPosition.col * cellSize;
		float y = drawedField.playerPosition.row * cellSize;

		graphics2d.setColor(PLAYER_COLOR);
		graphics2d.fill(new Ellipse2D.Float(x, y, cellSize, cellSize));
	}

	private void drawEndgamePoint(Graphics2D graphics2d) {
		final Color END_POINT_COLOR = Color.RED;
		final int LINE_OFFSET = (int) ((cellSize > 7) ? 7 : 0); // отступ от углов €чейки

		Cell cell = drawedField.endGamePoint;

		Line2D line1 = new Line2D.Float(cell.col * cellSize + LINE_OFFSET, cell.row * cellSize + LINE_OFFSET,
				(cell.col + 1) * cellSize - LINE_OFFSET, (cell.row + 1) * cellSize - LINE_OFFSET);
		Line2D line2 = new Line2D.Float(cell.col * cellSize + LINE_OFFSET, (cell.row + 1) * cellSize - LINE_OFFSET,
				(cell.col + 1) * cellSize - LINE_OFFSET, cell.row * cellSize + LINE_OFFSET);
		graphics2d.setColor(END_POINT_COLOR);
		graphics2d.setStroke(new BasicStroke(lineWidth));
		graphics2d.draw(line1);
		graphics2d.draw(line2);
	}

	public static void main(String[] args) {
		Field field;
		field = DataParcer.getFieldFromArgs(args);

		if (field != null) {
			System.out.println(field);
		} else {
			System.out.println("Error: Field not readed");
		}

		Drawer drawer = new Drawer(field);
		drawer.setVisible(true);
	}

}