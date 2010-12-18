#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

#include "icamviewgrabber.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    IcamViewGrabber *grabber;

    QLabel *statusLabel;

private slots:
    void on_doubleSpinBox_valueChanged(double );
    void on_pushButtonGrab_clicked();
    void on_spinBox_valueChanged(int );
    void on_pushButton_clicked();
    void on_lineEditPassword_textEdited(QString );
    void on_lineEditUsername_textEdited(QString );
    void on_pushButtonConnect_clicked();
    void newImageReceieved(QImage *);
    void updateDelayLabel(double, double);
};

#endif // MAINWINDOW_H
