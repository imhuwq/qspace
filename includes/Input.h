#ifndef QSPACE_INPUT_H
#define QSPACE_INPUT_H

#include <Qt>
#include <QPoint>
#include <QtGui/QCursor>

class Input {
    using Key = Qt::Key;
    using MouseButton = Qt::MouseButton;
public:
    enum InputState {
        InputInvalid,
        InputRegistered,
        InputUnregistered,
        InputTriggered,
        InputPressed,
        InputReleased
    };

    static InputState keyState(Key key);

    static bool keyTriggered(Key key) {
        return keyState(key) == InputTriggered;
    }

    static bool keyPressed(Key key) {
        return keyState(key) == InputPressed;
    }

    static bool keyReleased(Key key) {
        return keyState(key) == InputReleased;
    }

    static InputState buttonState(MouseButton button);

    static bool buttonTriggered(MouseButton button) {
        return buttonState(button) == InputTriggered;
    }

    static bool buttonPressed(MouseButton button) {
        return buttonState(button) == InputPressed;
    }

    static bool buttonRelease(MouseButton button) {
        return buttonState(button) == InputReleased;
    }

    static QPoint mousePosition() {
        return QCursor::pos();
    }

    static QPoint mouseDelta();

private:
    static void update();

    static void registerKeyPress(int key);

    static void registerKeyRelease(int key);

    static void registerMousePress(MouseButton button);

    static void registerMouseRelease(MouseButton button);

    static void reset();

    friend class Window;
};

#endif //QSPACE_INPUT_H
