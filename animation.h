#ifndef ANIMATION_H
#define ANIMATION_H

#include <qobject.h>
#include <qwidget.h>
#include <qtimer.h>
#include <qlist.h>
#include "cell.h"

class Animation : public QObject
{
	Q_OBJECT
public:
	Animation(const int msec, const int max, QObject *parent = 0);
	void changeInterval(const int msec) {interval = msec;}
	void changeMaxProgress(const int max) {maxProgress = max;}
	void add(QWidget *w, QPoint to);
	void addNew(QWidget *w);
	void addDelete(QWidget *w);
	void addRefresh(Cell *w);
	int find(QWidget *w) {return widgets.find(w);}
	void remove(QWidget *w);

public slots:
	void start(void);
	void stop(void);

private slots:
	void update(void);

private:
	void clear(void);

	struct pos {
		QPoint from;
		QPoint d;
	};

	int interval, progress, maxProgress;
	QTimer *timer;
	QList<QWidget> widgets, news, deletes;
	QList<Cell> refresh;
	QList<pos> positions;
};

#endif
