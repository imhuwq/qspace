#include "GLWidget.h"

#include <QtGui/QGuiApplication>

int main(int argc, char **argv) {
    QGuiApplication app(argc, argv);

    QSurfaceFormat format;
    format.setSamples(16);

    QTriangleWindow window;
    window.setFormat(format);
    window.resize(640, 480);
    window.show();

    window.update();

    return app.exec();
}
