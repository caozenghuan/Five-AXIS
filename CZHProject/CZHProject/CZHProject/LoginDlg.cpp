#include "LoginDlg.h"


LoginDlg::LoginDlg(QWidget *parent /* = 0 */):
	ui(new Ui::LoginDlg)
{ 
	ui->setupUi(this);
	this->setAttribute( Qt::WA_TranslucentBackground,true );
	this->setWindowFlags( Qt::FramelessWindowHint ); 
	timer = new QTimer(this);  
	connect(timer, SIGNAL(timeout()),this, SLOT(update()));
	backLevel=15;
	setWindowOpacity(1) ;
}

LoginDlg::~LoginDlg()
{

}



void LoginDlg::start()
{
	 timer->start(100); 
	 exec();
}


void LoginDlg::paintEvent(QPaintEvent *  event )
{
	backLevel--;
	if (backLevel!=0)
	{
		double j=backLevel*0.1;
		if (backLevel<10)
		{
			setWindowOpacity(j) ;
		}
		timer->stop();
		timer->start(100); 
	}
	else
	{
		timer->stop();
		close();
	}
}

