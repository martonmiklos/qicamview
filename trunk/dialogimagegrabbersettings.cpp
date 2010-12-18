#include "dialogimagegrabbersettings.h"
#include "ui_dialogimagegrabbersettings.h"

DialogImageGrabberSettings::DialogImageGrabberSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogImageGrabberSettings),
    grabber(NULL)
{
    ui->setupUi(this);
}

DialogImageGrabberSettings::~DialogImageGrabberSettings()
{
    delete ui;
}

void DialogImageGrabberSettings::setGrabber(ImageGrabber *gb)
{
    grabber = gb;
    ui->sourceLineEdit->setText(grabber->getSource());
    ui->fPSSpinBox->setValue(grabber->getFps());
    if (grabber->getFps() == 0) {
        ui->fPSSpinBox->setEnabled(false);
        ui->checkBoxNoLimit->setChecked(true);
    }
    connect(grabber, SIGNAL(stateChanged(ImageGrabber::GrabbingState)), this ,SLOT(grabberStateChanged(ImageGrabber::GrabbingState)));
    connect(grabber, SIGNAL(errorHappend()), this, SLOT(grabberErrorHappend()));
}

void DialogImageGrabberSettings::on_checkBoxNoLimit_toggled(bool checked)
{
    ui->fPSSpinBox->setEnabled(!checked);
    if (grabber != NULL) {
        if (checked)  // no limit -> FPS 0
            grabber->setFps(0);
        else
            grabber->setFps(ui->fPSSpinBox->value());
    }
}

void DialogImageGrabberSettings::on_pushButtonStart_clicked()
{
    ui->pushButtonStart->setEnabled(false);
    if (grabber != NULL) {
        if (grabber->isGrabbing()) {
            grabber->stopGrabbing();
        } else {
            grabber->startGrabbing();
        }
    }
}

void DialogImageGrabberSettings::grabberStateChanged(ImageGrabber::GrabbingState state)
{
    switch (state) {
    case ImageGrabber::GrabbingOn:
        ui->pushButtonStart->setText("Stop"); break;
    default:
        ui->pushButtonStart->setText("Start");
    }
    ui->pushButtonStart->setEnabled(true);
}

void DialogImageGrabberSettings::grabberErrorHappend()
{
    ui->labelError->setText(grabber->errorString());
}
