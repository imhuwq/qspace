#include "MainWindow.h"
#include "Application.h"

MainWindow::MainWindow(QWidget *parent) : gl_renderer(new GLWidget()) {
    setupSelfState();
    setupGLWidget();
}

MainWindow::~MainWindow() { delete gl_renderer; }

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
    if (!gl_renderer) gl_renderer = new GLWidget();
    if (centralWidget()) centralWidget()->setParent(nullptr);
    setCentralWidget(gl_renderer);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e) {
    if (e->mimeData()->hasUrls()) {
        e->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *e) {
    for (const QUrl &url:e->mimeData()->urls()) {
        QString fileName = url.toLocalFile();
        gl_renderer->LoadModelFile(fileName);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent* e) {
    if (e->key() == Qt::Key_Escape) {
        Application::quit();
    }
}
