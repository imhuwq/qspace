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
    setAcceptDrops(true);

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

void MainWindow::dragEnterEvent(QDragEnterEvent *e) {
    if (e->mimeData()->hasUrls()) {
        e->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *e) {
    for (const QUrl &url:e->mimeData()->urls()) {
        QString fileName = url.toLocalFile();
        m_glWidget->LoadModelFile(fileName);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent* e) {
    if (e->key() == Qt::Key_Escape) {
        Application::quit();
    }
}
