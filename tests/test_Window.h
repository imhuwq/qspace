#ifndef QSPACE_TEST_QTRIANGLEWINDOW_H
#define QSPACE_TEST_QTRIANGLEWINDOW_H

#include <gtest/gtest.h>
#include <string>
#include <QtGui/QGuiApplication>

#include "GLWidget.h"

using namespace std;

class QTriangleWindowTest : public ::testing::Test {
protected:
    int argc = 1;
    char *argv[1] = {"test"};
};

TEST_F(QTriangleWindowTest, CompileOK) {
    EXPECT_EQ(1, 1);
}

TEST_F(QTriangleWindowTest, Constructor) {
    QGuiApplication app(argc, argv);
    GLWidget window;
    EXPECT_EQ(1, 1);
}


#endif //QSPACE_TEST_QTRIANGLEWINDOW_H
