#include "animation.h"

//#define qDebug(...)

Animation::Animation(const int msec, const int max, QObject *parent) : QObject(parent)
{
	progress = 0;
	changeMaxProgress(max);
	timer = new QTimer(this);
	changeInterval(msec);
	widgets.setAutoDelete(false);
	positions.setAutoDelete(true);
	news.setAutoDelete(false);
	deletes.setAutoDelete(true);
	refresh.setAutoDelete(false);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
}

void Animation::add(QWidget *w, QPoint to)
{
	widgets.append(w);
	pos *p = new pos;
	p->from = w->pos();
	p->d = to - p->from;
	positions.append(p);
}

void Animation::remove(QWidget *w)
{
	int i = find(w);
	if (i < 0)
		return;
	widgets.remove(i);
	positions.remove(i);
}

void Animation::clear(void)
{
	widgets.clear();
	positions.clear();
	news.clear();
	deletes.clear();
	refresh.clear();
}

void Animation::start(void)
{
	if (widgets.count() != 0)
		timer->start(interval);
}

void Animation::stop(void)
{
	timer->stop();
	for (uint i = 0; i < widgets.count(); i++)
		widgets.at(i)->move(positions.at(i)->from + positions.at(i)->d);
	for (uint i = 0; i < news.count(); i++)
		news.at(i)->show();
	for (uint i = 0; i < refresh.count(); i++)
		refresh.at(i)->refresh();
	progress = 0;
	clear();
}

void Animation::update(void)
{
	if (progress == maxProgress) {
		stop();
		return;
	}
	progress++;
	QPoint from, to;
	for (uint i = 0; i < widgets.count(); i++) {
		from = positions.at(i)->from;
		to = from + positions.at(i)->d * progress / maxProgress;
		widgets.at(i)->move(to);
	}
}

void Animation::addNew(QWidget *w)
{
	if (!timer->isActive())
		return;
	w->hide();
	news.append(w);
}

void Animation::addDelete(QWidget *w)
{
	if (deletes.find(w) == -1)
		deletes.append(w);
}

void Animation::addRefresh(Cell *w)
{
	refresh.append(w);
}
