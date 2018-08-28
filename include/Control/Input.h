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

    static bool KeyTriggered(Key key) {
        return keyState(key) == InputTriggered;
    }

    static bool KeyPressed(Key key) {
        return keyState(key) == InputPressed;
    }

    static bool KeyReleased(Key key) {
        return keyState(key) == InputReleased;
    }

    static InputState ButtonState(MouseButton button);

    static bool ButtonTriggered(MouseButton button) {
        return ButtonState(button) == InputTriggered;
    }

    static bool ButtonPressed(MouseButton button) {
        return ButtonState(button) == InputPressed;
    }

    static bool ButtonRelease(MouseButton button) {
        return ButtonState(button) == InputReleased;
    }

    static QPoint MousePosition() {
        return QCursor::pos();
    }

    static QPoint MouseDelta();

private:
    static void Update();

    static void RegisterKeyPress(int key);

    static void RegisterKeyRelease(int key);

    static void RegisterMousePress(MouseButton button);

    static void RegisterMouseRelease(MouseButton button);

    static void Reset();

    friend class GLWidget;
};

#endif //QSPACE_INPUT_H
