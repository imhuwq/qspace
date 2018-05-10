#ifndef QSPACE_TEST_TRIANGLEWINDOW_H
#define QSPACE_TEST_TRIANGLEWINDOW_H

#include <gtest/gtest.h>
#include <string>
#include <QtGui/QGuiApplication>

#include "../playground/OpenGLWindow/TriangleWindow.h"

using namespace std;

class TriangleWindowTest : public ::testing::Test {
protected:
    int argc = 1;
    char *argv[1] = {"test"};
};

TEST_F(TriangleWindowTest, CompileOK) {
    EXPECT_EQ(1, 1);
}

TEST_F(TriangleWindowTest, Constructor) {
    QGuiApplication app(argc, argv);
    TriangleWindow window;
    EXPECT_EQ(1, 1);
}

#endif //QSPACE_TEST_TRIANGLEWINDOW_H
