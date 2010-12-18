#ifndef IMAGEGRABBER_H
#define IMAGEGRABBER_H

#include <QObject>
#include <QImage>
#include <QBuffer>
#include <QtGui/QImage>
#include <QtGui/QImageReader>
#include <QSize>
#include <QTimer>
#include <QVariant>

#include "getimage_global.h"
#define FPS_ARRAY_SIZE 50

class ImageGrabberParameter
{
public:
    enum ParameterFlag {
         NoFlags = 0x0,
         ReadOnly = 0x1,
         WriteOnly = 0x2,
     };
    Q_DECLARE_FLAGS(ParameterFlags, ParameterFlag)

    QString name;
    QVariant value;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(ImageGrabberParameter::ParameterFlags)


class GETIMAGESHARED_EXPORT ImageGrabber : public QObject
{
    Q_OBJECT
public:
    enum GrabbingState {
        GrabbingOff = 0,
        GrabbingTurnOn,
        GrabbingOn,
        GrabbingTurnOff,
        GrabbingError,
    };


    ImageGrabber(QObject *parent = 0);

    QString errorString() const {return errorStr;}

    virtual void startGrabbing() = 0;
    virtual void stopGrabbing() = 0;
    virtual bool isGrabbing() const {return (currentState == GrabbingOn);}

    virtual void setFps(double framePerSec) = 0;
    double getFps() const {return fps;}

    virtual void setSource(QString str) = 0;
    QString getSource() const {return source;}

    QList<ImageGrabberParameter> getParameterList() const {return parameters;}
    bool setParameter(QString name, QVariant value);
    QVariant getParameter(QString name);

    bool isSourceEnumeratorSupported() const {return enumeratorSupported;}
    virtual QStringList enumerateSources() = 0;

protected:
    QString errorStr;
    QBuffer *imageBuffer;
    QImageReader *imageReader;
    QImage *currentImage;
    double fps;
    QString source;
    GrabbingState currentState;

    void setError(QString str);

    quint16 delays[FPS_ARRAY_SIZE];
    int currentDelayPointer;
    quint64 sum;

    void calcFPS(quint16 newDelay);

    QList<ImageGrabberParameter> parameters;

    bool enumeratorSupported;

    QTimer newImageTimer;
signals:
    void newImageGrabbed(QImage *img);
    void errorHappend();
    void stateChanged(ImageGrabber::GrabbingState newState);
    void newFPSCalculated(double current, double avg);

public slots:
protected slots:
    virtual void newImageTimeOut() = 0;
};

#endif // IMAGEGRABBER_H
