#include "myqt.h"
#include <QtWidgets/QApplication>
#include "LoginDlg.h"
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	LoginDlg L;
	L.start();
	MyQT w;
	w.show();
	return a.exec();
}
