#ifndef QSPACE_APPLICATION_H
#define QSPACE_APPLICATION_H

#include <QApplication>

#include "MainWindow.h"

class Application : public QApplication {
public:
    Application(int &argc, char **argv) : QApplication(argc, argv), m_mainWindow(new MainWindow()) {}

    ~Application() { delete m_mainWindow; }

    int run() {
        m_mainWindow->show();
        return QApplication::exec();
    }

private:
    MainWindow *m_mainWindow;
};

#endif //QSPACE_APPLICATION_H
