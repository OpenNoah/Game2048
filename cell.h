#ifndef CELL_H
#define CELL_H

#include <qwidget.h>
#include <qobject.h>
#include <qpixmap.h>

class Cell : public QWidget
{
	Q_OBJECT
public:
	Cell(const int x, const int y, const int v, QWidget *parent);
	int x(void) const {return data.x;}
	void setX(const int x) {data.x = x;}
	int y(void) const {return data.y;}
	void setY(const int y) {data.y = y;}
	int value(void) const {return data.v;}
	void setValue(const int v);
	bool merged(void) const {return data.merged;}
	void setMerged(bool merged) {data.merged = merged;}
	void refresh(void);

signals:
	void mousePress(QMouseEvent *);
	void mouseRelease(QMouseEvent *);

protected:
	void resizeEvent(QResizeEvent *);
	void paintEvent(QPaintEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);

private:
	struct {
		bool merged;
		int x, y, v;
	} data;
	QPixmap cache;
};

#endif
