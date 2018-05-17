#ifndef QSPACE_MAINWINDOW_H
#define QSPACE_MAINWINDOW_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <QStyle>

#include "GLWidget.h"


class MainWindow : public QMainWindow {
Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private:
    GLWidget *m_glWidget;

    void setupSelfState();

    void setWindowToCenter();

    void setupGLWidget();
};

#endif //QSPACE_MAINWINDOW_H
