#ifndef QSPACE_TRIANGLEWINDOW_H
#define QSPACE_TRIANGLEWINDOW_H

#include <QtGui/QScreen>
#include <QtGui/QOpenGLShaderProgram>
#include "OpenGLWindow.h"

class TriangleWindow : public OpenGLWindow {
public:
    TriangleWindow();

    void initialize() override;

    void render() override;

private:
    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_matrixUniform;

    QOpenGLShaderProgram *m_program;
    int m_frame;
};

#endif //QSPACE_TRIANGLEWINDOW_H
