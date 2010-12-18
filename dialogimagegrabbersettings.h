#ifndef DIALOGIMAGEGRABBERSETTINGS_H
#define DIALOGIMAGEGRABBERSETTINGS_H

#include <QDialog>
#include "imagegrabber.h"

namespace Ui {
    class DialogImageGrabberSettings;
}

class GETIMAGESHARED_EXPORT DialogImageGrabberSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DialogImageGrabberSettings(QWidget *parent = 0);
    ~DialogImageGrabberSettings();
    void setGrabber(ImageGrabber *gb);

private:
    Ui::DialogImageGrabberSettings *ui;
    ImageGrabber *grabber;

private slots:
    void on_pushButtonStart_clicked();
    void grabberStateChanged(ImageGrabber::GrabbingState state);
    void grabberErrorHappend();
    void on_checkBoxNoLimit_toggled(bool checked);

};

#endif // DIALOGIMAGEGRABBERSETTINGS_H
