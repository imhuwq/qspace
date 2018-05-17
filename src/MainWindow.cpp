#include "MainWindow.h"
#include "Application.h"

void MainWindow::setupSelfState() {
    setFixedSize(800, 800);
    setWindowTitle("QSpace");
    setObjectName("mainWindow");

    setWindowToCenter();
}

void MainWindow::setWindowToCenter() {
    setGeometry(
            QStyle::alignedRect(
                    Qt::LeftToRight,
                    Qt::AlignCenter,
                    size(),
                    Application::desktop()->availableGeometry()
            )
    );
}