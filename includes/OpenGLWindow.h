#ifndef QSPACE_OPENGLWINDOW_H
#define QSPACE_OPENGLWINDOW_H

#include <QtGui/QWindow>
#include <QtGui/QOpenGLFunctions>
#include <QtCore/QCoreApplication>
#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>


class OpenGLWindow : public QWindow, protected QOpenGLFunctions {
Q_OBJECT
public:
    explicit OpenGLWindow(QWindow *parent = nullptr);

    ~OpenGLWindow();

    virtual void render(QPainter *painter);

    virtual void render();

    virtual void initialize();

    void setAnimating(bool animating);

public slots:

    void renderLater();

    void renderNow();

protected:
    bool event(QEvent *event) override;

    void exposeEvent(QExposeEvent *event) override;

private:
    bool m_animating;
    QOpenGLContext *m_context;
    QOpenGLPaintDevice *m_device;
};

#endif //QSPACE_OPENGLWINDOW_H
