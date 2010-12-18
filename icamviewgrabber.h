#ifndef ICAMVIEWGRABBER_H
#define ICAMVIEWGRABBER_H

#include <QObject>
#include <QUdpSocket>
#include <QTimer>
#include <QFile>
#include <QTime>
#include "imagegrabber.h"

class IcamViewGrabber : public ImageGrabber
{
    enum {

    } IcamViewState;

    Q_OBJECT
public:
    IcamViewGrabber(QObject *parent = 0);
    ~IcamViewGrabber() {saveFile.close();}
    void connectToHost(QHostAddress address, quint16 port);

    QString errorString() const {return error;}
    void setUserName(QString username) {userName = username;}
    void setPassword(QString pwd) {password = pwd;}

    void setCameraNo(int number) {cameraNumber = number;}

    void startGrabbing();
    void stopGrabbing(){}

    void setFps(double framePerSec) {fps = framePerSec;}
    void setSource(QString str) {}

    void sendImageRequest();
    void save();

    QStringList enumerateSources();

private:
    QUdpSocket socket;

    void setError(QString errorString);
    QString error;
    QString userName;
    QString password;

    int requestId;

    void sendLogin();

    void debugArray(QByteArray & array, bool limited = false);
    int cameraNumber;
    QFile saveFile;

    QByteArray receieveArray;
    bool firstAfterLogin;

    int currentImageSize;

    QTimer timeOutTimer;

    QTime reqTime;
signals:
    void errorHappend();

public slots:
private slots:
    void socketStateChanged(QAbstractSocket::SocketState);
    void socketReadyRead();
    void timeOutSlot();
    void newImageTimeOut();
};

#endif // ICAMVIEWGRABBER_H
