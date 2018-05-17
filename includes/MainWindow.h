#ifndef QSPACE_MAINWINDOW_H
#define QSPACE_MAINWINDOW_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <QStyle>


class MainWindow : public QMainWindow {
Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr) { setupSelfState(); }

    ~MainWindow() {}

private:
    void setupSelfState();
    void setWindowToCenter();
};

#endif //QSPACE_MAINWINDOW_H
