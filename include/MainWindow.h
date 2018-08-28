#ifndef QSPACE_MAINWINDOW_H
#define QSPACE_MAINWINDOW_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <QStyle>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>

#include "GLWidget.h"

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

protected:
    void dragEnterEvent(QDragEnterEvent *e);

    void dropEvent(QDropEvent *e);

    void keyReleaseEvent(QKeyEvent* e);

private:
    GLWidget *gl_renderer;

    void setupSelfState();

    void setWindowToCenter();

    void setupGLWidget();
};

#endif //QSPACE_MAINWINDOW_H
