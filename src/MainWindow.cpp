#include "MainWindow.h"
#include "Application.h"

MainWindow::MainWindow(QWidget *parent) : m_glWidget(new GLWidget()) {
    setupSelfState();
    setupGLWidget();
}

MainWindow::~MainWindow() { delete m_glWidget; }


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

void MainWindow::setupGLWidget() {
    if (!m_glWidget) m_glWidget = new GLWidget();
    if (centralWidget()) centralWidget()->setParent(nullptr);
    setCentralWidget(m_glWidget);
}