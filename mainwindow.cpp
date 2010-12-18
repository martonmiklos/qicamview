#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    grabber = new IcamViewGrabber(this);
    grabber->setUserName(ui->lineEditUsername->text());
    grabber->setPassword(ui->lineEditPassword->text());
    connect(grabber, SIGNAL(newImageGrabbed(QImage*)), this, SLOT(newImageReceieved(QImage*)));
    connect(grabber, SIGNAL(newFPSCalculated(double,double)), this, SLOT(updateDelayLabel(double,double)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonConnect_clicked()
{
    grabber->connectToHost(QHostAddress(ui->lineEditIPAddress->text()), ui->spinBoxPort->value());
}

void MainWindow::on_lineEditUsername_textEdited(QString text)
{
    grabber->setUserName(text);
}

void MainWindow::on_lineEditPassword_textEdited(QString text)
{
    grabber->setPassword(text);
}

void MainWindow::on_pushButton_clicked()
{
    grabber->startGrabbing();
}

void MainWindow::on_spinBox_valueChanged(int value)
{
    grabber->setCameraNo(value);
}

void MainWindow::on_pushButtonGrab_clicked()
{
    grabber->sendImageRequest();
}


void MainWindow::newImageReceieved(QImage *img)
{
    ui->labelImage->setPixmap(QPixmap::fromImage(*img));
}

void MainWindow::updateDelayLabel(double now, double avg)
{
    ui->labelFPS->setText(QString("%1 FPS").arg(now));
    ui->labelFPSAvg->setText(QString("%1 FPS").arg(avg));
}

void MainWindow::on_doubleSpinBox_valueChanged(double value)
{
    grabber->setFps(value);
}
