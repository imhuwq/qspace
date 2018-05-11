#include <QGuiApplication>
#include "Window.h"

int main(int argc, char **argv) {
    QGuiApplication app(argc, argv);

    Window window;
    window.resize(QSize(800, 800));
    window.show();

    return app.exec();
}
