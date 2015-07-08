#ifndef GRID_H
#define GRID_H

#include <qwidget.h>
#include <qevent.h>
#include <qpoint.h>
#include "cell.h"
#include "animation.h"

class Grid : public QWidget
{
	Q_OBJECT
public:
	Grid(QWidget *parent);

public slots:
	void mousePressd(QMouseEvent *);
	void mouseReleased(QMouseEvent *);

signals:
	void scoreChanged(int);

protected:
	void resizeEvent(QResizeEvent *e);
	void paintEvent(QPaintEvent *);
	void keyPressEvent(QKeyEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);

private:
	struct vect {
		int x, y;
	};

	void init(void);
	void reinit(void);
	void move(void);
	bool move(const vect dir);
	void move(class Cell **from, class Cell **to);
	class Cell **find(class Cell **orig, const vect dir);
	bool generate(void);
	bool failed(void);

	int frame, score;
	QPixmap cache;
	QPoint mouse;
	Cell *cells[4][4];
	Animation *ani;
};

#endif
