#include "cell.h"
#include "tile.h"
#include <qpainter.h>
#include <qfont.h>

Cell::Cell(const int x, const int y, const int v, QWidget *parent) : QWidget(parent)
{
	setX(x);
	setY(y);
	setValue(v);
	setMerged(false);
	refresh();
	show();
	connect(this, SIGNAL(mousePress(QMouseEvent *)), parent, SLOT(mousePressd(QMouseEvent *)));
	connect(this, SIGNAL(mouseRelease(QMouseEvent *)), parent, SLOT(mouseReleased(QMouseEvent *)));
}

void Cell::resizeEvent(QResizeEvent *)
{
	refresh();
}

void Cell::refresh(void)
{
	cache.resize(size());
	QPainter painter(&cache);
	painter.setPen(Tile::background(data.v));
	painter.setBrush(Tile::background(data.v));
	painter.drawRect(rect());
	if (data.v == 0)
		return;
	QFont f(Tile::font(data.v));
	f.setPixelSize(f.pixelSize() * width() / 90);
	painter.setFont(f);
	painter.setPen(Tile::foreground(data.v));
	painter.drawText(rect(), Qt::AlignCenter, QString::number(data.v));
}

void Cell::setValue(const int v)
{
	data.v = v;
	if (v == 0)
		lower();
	else
		raise();
	//refresh();
}

void Cell::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);
	painter.drawPixmap(0, 0, cache);
}

void Cell::mousePressEvent(QMouseEvent *e)
{
	emit mousePress(e);
}

void Cell::mouseReleaseEvent(QMouseEvent *e)
{
	emit mouseRelease(e);
}
