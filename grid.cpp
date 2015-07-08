#include "grid.h"
#include "tile.h"
#include <qapplication.h>
#include <qpainter.h>
#include <qmessagebox.h>
#include <qlist.h>
#include <qsizepolicy.h>
#include <ctime>
#include <cstdlib>

#define MINS	202

Grid::Grid(QWidget *parent) : QWidget(parent)
{
	setFocusPolicy(StrongFocus);
	setMinimumSize(MINS, MINS);
	setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
	srand(time(NULL));
	ani = new Animation(30, 5, this);
	connect(qApp, SIGNAL(aboutToQuit()), ani, SLOT(stop()));
	init();
}

void Grid::init(void)
{
	score = 0;
	emit scoreChanged(score);
	QRect rCell(frame, frame, width() - 2 * frame, width() - 2 * frame);
	rCell.setWidth((rCell.width() - frame * 3) / 4);
	rCell.setHeight((rCell.height() - frame * 3) / 4);
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++) {
			cells[x][y] = new Cell(x, y, 0, this);
			cells[x][y]->setGeometry(rCell.left() + x * (rCell.width() + frame), rCell.top() + y * (rCell.height() + frame), rCell.width(), rCell.height());
		}
	generate();
}

void Grid::reinit(void)
{
	ani->stop();
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
			delete cells[x][y];
	init();
}

bool Grid::move(const vect dir)
{
	ani->stop();
	int scoreBak = score;
	bool moved = false;
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++) {
			int xt = dir.x == 1 ? 3 - x : x;
			int yt = dir.y == 1 ? 3 - y : y;
			if (cells[xt][yt]->value() == 0)
				continue;
			Cell **cell = find(&cells[xt][yt], dir);
			if (&cells[xt][yt] != cell)
				moved = true;
			move(&cells[xt][yt], cell);
		}
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
			cells[x][y]->setMerged(false);
	if (score != scoreBak)
		emit scoreChanged(score);
	ani->start();
	return moved;
}

void Grid::move(Cell **from, Cell **to)
{
	if (*from == *to)
		return;
	QRect rCell(frame, frame, width() - frame * 2, height() - frame * 2);
	rCell.setWidth((rCell.width() - frame * 3) / 4);
	rCell.setHeight((rCell.height() - frame * 3) / 4);
	if ((*to)->value() != 0) {
		(*from)->setValue((*from)->value() + (*to)->value());
		(*from)->setMerged(true);
		ani->addRefresh(*from);
		score += (*from)->value();
	}
	int x = (*to)->x(), y = (*to)->y();
	ani->addDelete(*to);
	*to = *from;
	*from = new Cell((*to)->x(), (*to)->y(), 0, this);
	(*from)->setGeometry(rCell.left() + (*to)->x() * (rCell.width() + frame), \
			rCell.top() + (*to)->y() * (rCell.height() + frame), \
			rCell.width(), rCell.height());
	(*to)->setX(x);
	(*to)->setY(y);

	ani->add(*to, QPoint(rCell.left() + x * (rCell.width() + frame), \
			rCell.top() + y * (rCell.height() + frame)));
}

class Cell **Grid::find(Cell **orig, const vect dir)
{
	int x = (*orig)->x(), y = (*orig)->y();
	do {
		x += dir.x;
		y += dir.y;
	} while (x >= 0 && x <= 3 && y >= 0 && y <= 3 && cells[x][y]->value() == 0);
	if (x >= 0 && x <= 3 && y >= 0 && y <= 3 && \
			abs(cells[x][y]->value()) == abs((*orig)->value()) && \
			!cells[x][y]->merged())
		return &cells[x][y];
	x -= dir.x;
	y -= dir.y;
	return &cells[x][y];
}

bool Grid::generate(void)
{
	QList<Cell *> list;
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++) {
			if (cells[x][y]->value() != 0)
				continue;
			list.append(&cells[x][y]);
		}
	if (list.count() == 0)
		return false;
	Cell *cell = *list.at(rand() % list.count());
	cell->setValue((rand() % 100) < 90 ? 2 : 4);
	ani->addNew(cell);
	cell->refresh();
	return true;
}

bool Grid::failed(void)
{
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++) {
			if (cells[x][y]->value() == 0)
				return false;
			if (x != 0 && abs(cells[x][y]->value()) == abs(cells[x - 1][y]->value()))
				return false;
			if (x != 3 && abs(cells[x][y]->value()) == abs(cells[x + 1][y]->value()))
				return false;
			if (y != 0 && abs(cells[x][y]->value()) == abs(cells[x][y - 1]->value()))
				return false;
			if (y != 3 && abs(cells[x][y]->value()) == abs(cells[x][y + 1]->value()))
				return false;
		}
	return true;
}

void Grid::resizeEvent(QResizeEvent *e)
{
	int w = e->size().width(), h = e->size().height();
	int s = w < h ? w : h;
	if (w != h)
		resize(s, s);
	frame = 15 * s / 500;
	QRect rCell(frame, frame, s - 2 * frame, s - 2 * frame);
	rCell.setWidth((rCell.width() - frame * 3) / 4);
	rCell.setHeight((rCell.height() - frame * 3) / 4);
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
			cells[x][y]->setGeometry(rCell.left() + x * (rCell.width() + frame), rCell.top() + y * (rCell.height() + frame), rCell.width(), rCell.height());

	cache.resize(size());
	//cache.fill(backgroundColor());
	QPainter painter(&cache);
	painter.setPen(Tile::foreground(0));
	painter.setBrush(Tile::foreground(0));
	painter.drawRect(rect());
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++) {
			painter.setPen(Tile::background(0));
			painter.setBrush(Tile::background(0));
			painter.drawRect(rCell.left() + x * (rCell.width() + frame) - 1, rCell.top() + y * (rCell.height() + frame) - 1, rCell.width(), rCell.height());
		}
}

void Grid::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.drawPixmap(1, 1, cache);
	return;
}

void Grid::keyPressEvent(QKeyEvent *e)
{
	vect dir = {0, 0};
	switch (e->key()) {
	case 'W':
	case 'w':
	case 'K':
	case 'k':
	case Qt::Key_Up:
		dir.y = -1;
		break;
	case 'S':
	case 's':
	case 'J':
	case 'j':
	case Qt::Key_Down:
		dir.y = 1;
		break;
	case 'A':
	case 'a':
	case 'H':
	case 'h':
	case Qt::Key_Left:
		dir.x = -1;
		break;
	case 'D':
	case 'd':
	case 'L':
	case 'l':
	case Qt::Key_Right:
		dir.x = 1;
		break;
	case 'P':
	case 'p':
		dir.x = rand() % 3 - 1;
		if (dir.x == 0)
			dir.y = rand() % 3 - 1;
		break;
	case 'R':
	case 'r':
		reinit();
		return;
	case 'Q':
	case 'q':
		qApp->quit();
		return;
	default:
		e->ignore();
		qDebug(QString::number(e->key(), 16));
		return;
	}
	if (move(dir)) {
		generate();
		if (failed()) {
			QMessageBox::critical(topLevelWidget(), "Full", "The board is full!");
			reinit();
		}
	}
}

void Grid::mousePressd(QMouseEvent *e) {mousePressEvent(e);}
void Grid::mouseReleased(QMouseEvent *e) {mouseReleaseEvent(e);}

void Grid::mousePressEvent(QMouseEvent *e)
{
	QWidget::mousePressEvent(e);
	mouse = e->globalPos();
}

void Grid::mouseReleaseEvent(QMouseEvent *e)
{
	QWidget::mouseReleaseEvent(e);
	int x = e->globalX() - mouse.x(), y = e->globalY() - mouse.y();
	if (x == y)
		return;
	vect dir = {0, 0};
	if (abs(x) > abs(y))
		dir.x = x > 0 ? 1 : -1;
	else
		dir.y = y > 0 ? 1 : -1;
	if (move(dir)) {
		generate();
		if (failed()) {
			QMessageBox::critical(topLevelWidget(), "Full", "The board is full!");
			reinit();
		}
	}
}
