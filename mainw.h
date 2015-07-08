#ifndef MAINW_H
#define MAINW_H

#include <qobject.h>
#include <qwidget.h>
#include <qmainwindow.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include "pathmgr.h"
#include "grid.h"

class MainW : public QMainWindow, public pathMgr
{
	Q_OBJECT
public:
	MainW(void);

private slots:
	void updateScore(int score);

private:
	Grid *g;
	QLabel *lScore;
	QPushButton *pbQuit;
};

#endif
