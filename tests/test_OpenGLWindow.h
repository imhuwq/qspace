#ifndef QSPACE_TEST_OPENGLWINDOW_H
#define QSPACE_TEST_OPENGLWINDOW_H

#include <gtest/gtest.h>
#include <string>
#include <QtGui/QGuiApplication>

#include "OpenGLWindow.h"

using namespace std;

class OpenGLWindowTest : public ::testing::Test {
protected:
    int argc = 1;
    char *argv[1] = {"test"};
};

TEST_F(OpenGLWindowTest, CompileOK) {
    EXPECT_EQ(1, 1);
}

TEST_F(OpenGLWindowTest, Constructor) {
    QGuiApplication app(argc, argv);
    OpenGLWindow window;
    EXPECT_EQ(1, 1);
}

#endif //QSPACE_TEST_OPENGLWINDOW_H
