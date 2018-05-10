#include <QGuiApplication>
#include "Window.h"

int main(int argc, char **argv) {
    QGuiApplication app(argc, argv);

    Window window;
    window.resize(QSize(800, 600));
    window.show();

    return app.exec();
}
