#include "icamviewgrabber.h"
#include <QDebug>
#include <QFile>

IcamViewGrabber::IcamViewGrabber(QObject *)
{
    connect(&socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));
    connect(&socket, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
    cameraNumber = 1;
    requestId = 0;
    connect(&timeOutTimer, SIGNAL(timeout()), this, SLOT(timeOutSlot()));
    timeOutTimer.setSingleShot(true);
    enumeratorSupported = false;
}

void IcamViewGrabber::connectToHost(QHostAddress address, quint16 port)
{
    socket.connectToHost(address, port, QUdpSocket::ReadWrite);
}


void IcamViewGrabber::socketStateChanged(QAbstractSocket::SocketState newState)
{
    switch(newState) {
    case QAbstractSocket::ConnectedState:
        //sendLogin();
        qWarning() << "connected";
        break;
    }
}

void IcamViewGrabber::socketReadyRead()
{

    receieveArray.append(socket.readAll());
   if (receieveArray.size() > 25) { // check for enough bytes to read the header
        unsigned char recId = ((unsigned char)receieveArray[0]) - 48;
        if (requestId == recId) {
            if (imageBuffer->pos() == 0) { // first packet after request have a 31 byte length header
                bool ok = false;
                if (firstAfterLogin) {
                    currentImageSize = receieveArray.mid(16, 6).toInt(&ok);
                    if (ok == false) {
                        qWarning() << QString(tr("Unable to convert the %1 string to number")
                                              .arg(QString(receieveArray.mid(16, 6))));

                        timeOutTimer.stop();
                        receieveArray.clear();
                        sendImageRequest();
                        return;
                    } else {
                        imageBuffer->open(QBuffer::ReadWrite | QBuffer::Truncate);
                        imageBuffer->write(receieveArray.mid(31));
                        qWarning() << "written :" << receieveArray.mid(31).size();
                        firstAfterLogin = false;
                    }
                } else {
                    currentImageSize = receieveArray.mid(10, 6).toInt(&ok);
                    if (ok == false) {
                        qWarning() << QString(tr("Unable to convert the %1 string to number")
                                              .arg(QString(receieveArray.mid(10, 6))));
                        timeOutTimer.stop();
                        receieveArray.clear();
                        sendImageRequest();
                        return;
                    } else {
                        imageBuffer->open(QBuffer::ReadWrite | QBuffer::Truncate);
                        imageBuffer->write(receieveArray.mid(25));
                    }
                }

            } else{ // the second and the following packets has 8 byte header
                imageBuffer->write(receieveArray.mid(8));
            }

            if (imageBuffer->pos() >= currentImageSize) {
                int currentImageDelay = reqTime.msecsTo(QTime::currentTime());
                calcFPS(currentImageDelay);
                timeOutTimer.stop();;
                imageBuffer->seek(0);
                imageReader->read(currentImage);
                imageBuffer->close();
                emit newImageGrabbed(currentImage);
                int waitDelay = 0;
                if (fps != 0)
                    waitDelay = (1000.0f/fps) - currentImageDelay;
                if ((fps == 0) || (waitDelay < 20)) {
                    sendImageRequest();
                } else {
                    newImageTimer.start(waitDelay);
                }
            }
            receieveArray.clear();
        } else { // have no header clear the buffer
            receieveArray.clear();
            timeOutTimer.stop();
            sendImageRequest();
        }
    }
}

void IcamViewGrabber::startGrabbing()
{
    if (socket.isOpen()) {
        sendLogin();
    }
}

void IcamViewGrabber::debugArray(QByteArray & inArr, bool limit)
{
    int top = inArr.size();
    if (limit)
        top = 40;
    for (int i = 0; i<top; i++) {
        QChar chr = ' ';
        if (QChar(inArr[i]).isLetterOrNumber())
            chr = QChar(inArr[i]);
        qDebug()  << i
                << (unsigned char)inArr[i] << "\t\t"
                << QString::number((unsigned char)inArr[i]) << "\t\t"
                << QString::number((unsigned char)inArr[i], 16).rightJustified(2, '0')
                << chr;
    }
    qWarning() << "............";
}

void IcamViewGrabber::sendLogin()
{
    QByteArray loginArray;

    // message header
    loginArray.append((char)0x38);
    loginArray.append((char)0x30);
    loginArray.append((char)0x34);
    loginArray.append((char)0x33);
    loginArray.append((char)0x32);
    loginArray.append((char)0x31);

    // camera ID
    loginArray.append((char)0x30);
    loginArray.append((char)0x30 + cameraNumber);

    loginArray.append(userName);
    for (int i = 0; i<(32-userName.length()); i++) {
        loginArray.append((char)0);
    }
    loginArray.append(password);
    int left = 72-loginArray.size();
    for (int i = 0; i<left; i++) {
        loginArray.append((char)0);
    }
    socket.write(loginArray);
    firstAfterLogin = true;
}

void IcamViewGrabber::sendImageRequest()
{

    reqTime = QTime::currentTime();
    requestId=8;
    if (requestId == 9)
        requestId = 1;
    QByteArray imgReqArray;
    // message header
    imgReqArray.append((char)0x30+requestId);
    imgReqArray.append((char)0x30);
    imgReqArray.append((char)0x33);
    imgReqArray.append((char)0x30);
    imgReqArray.append((char)0x30);
    imgReqArray.append((char)0x32);
    socket.write(imgReqArray);
    saveFile.setFileName("out.jpg");
    currentImageSize = 0;
    imageBuffer->close();
    timeOutTimer.start(1000);
}

void IcamViewGrabber::timeOutSlot()
{
    qWarning() << "TOUT";
    imageBuffer->close();
    sendImageRequest();
}

QStringList IcamViewGrabber::enumerateSources()
{
    return QStringList();
}

void IcamViewGrabber::newImageTimeOut()
{
    //FIXME
    sendImageRequest();
}
