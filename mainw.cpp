#include <qapplication.h>
#include "mainw.h"
#include "qlayout.h"
#include "qgrid.h"
#include "qvbox.h"

MainW::MainW(void) : QMainWindow()
{
	QGrid *w = new QGrid(2, this);
	w->setSpacing(2);
	setCentralWidget(w);

	g = new Grid(w);
	QVBox *v = new QVBox(w);

	lScore = new QLabel(v);
	pbQuit = new QPushButton("Quit", v);

	lScore->setText("Score:		\n0");
	setCaption("2048 - by zhiyb");

	connect(g, SIGNAL(scoreChanged(int)), this, SLOT(updateScore(int)));
	connect(pbQuit, SIGNAL(clicked()), qApp, SLOT(quit()));
}

void MainW::updateScore(int score)
{
	lScore->setText(QString(tr("Score:		\n%1")).arg(score));
}
