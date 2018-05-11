#include <QCursor>
#include <vector>
#include <algorithm>

#include "Input.h"

template<typename T>
struct InputInstance : std::pair<T, Input::InputState> {
    typedef std::pair<T, Input::InputState> base_class;

    inline InputInstance(T value) : base_class(value, Input::InputInvalid) {}

    inline InputInstance(T value, Input::InputState state) : base_class(value, state) {}

    inline bool operator==(const InputInstance &rhs) const {
        return this->first == rhs.first;
    }
};

typedef InputInstance<Qt::Key> KeyInstance;
typedef InputInstance<Qt::MouseButton> ButtonInstance;

typedef std::vector<KeyInstance> KeyContainer;
typedef std::vector<ButtonInstance> ButtonContainer;

static KeyContainer sg_keyInstances;
static ButtonContainer sg_buttonInstances;
static QPoint sg_mouseCurrPosition;
static QPoint sg_mousePrevPosition;
static QPoint sg_mouseDelta;

static inline KeyContainer::iterator FindKey(Qt::Key value) {
    return std::find(sg_keyInstances.begin(), sg_keyInstances.end(), value);
}

static inline ButtonContainer::iterator FindButton(Qt::MouseButton value) {
    return std::find(sg_buttonInstances.begin(), sg_buttonInstances.end(), value);
}

template<typename TPair>
static inline void UpdateStates(TPair &instance) {
    switch (instance.second) {
        case Input::InputRegistered:
            instance.second = Input::InputTriggered;
            break;
        case Input::InputTriggered:
            instance.second = Input::InputPressed;
            break;
        case Input::InputUnregistered:
            instance.second = Input::InputReleased;
            break;
        default:
            break;
    }
}

template<typename TPair>
static inline bool CheckReleased(const TPair &instance) {
    return instance.second == Input::InputReleased;
}

template<typename Container>
static inline void Update(Container &container) {
    typedef typename Container::iterator Iter;
    typedef typename Container::value_type TPair;

    Iter remove = std::remove_if(container.begin(), container.end(), &CheckReleased<TPair>);
    container.erase(remove, container.end());
    std::for_each(container.begin(), container.end(), &UpdateStates<TPair>);
}

Input::InputState Input::keyState(Key key) {
    KeyContainer::iterator it = FindKey((key));
    return it == sg_keyInstances.end() ? InputInvalid : it->second;
}

Input::InputState Input::buttonState(MouseButton button) {
    ButtonContainer::iterator it = FindButton(button);
    return it == sg_buttonInstances.end() ? InputInvalid : it->second;
}

QPoint Input::mouseDelta() {
    return sg_mouseDelta;
}

void Input::update() {
    sg_mousePrevPosition = sg_mouseCurrPosition;
    sg_mouseCurrPosition = QCursor::pos();
    sg_mouseDelta = sg_mouseCurrPosition - sg_mousePrevPosition;

    Update(sg_buttonInstances);
    Update(sg_keyInstances);
}

void Input::registerKeyPress(int key) {
    KeyContainer::iterator it = FindKey((Key) key);
    if (it == sg_keyInstances.end()) {
        sg_keyInstances.emplace_back((Key) key, InputRegistered);
    }
}

void Input::registerKeyRelease(int key) {
    KeyContainer::iterator it = FindKey((Key) key);
    if (it != sg_keyInstances.end()) {
        it->second = InputUnregistered;
    }
}

void Input::registerMousePress(MouseButton button) {
    ButtonContainer::iterator it = FindButton(button);
    if (it == sg_buttonInstances.end()) {
        sg_buttonInstances.emplace_back(button, InputRegistered);
    }
}

void Input::registerMouseRelease(MouseButton button) {
    ButtonContainer::iterator it = FindButton(button);
    if (it != sg_buttonInstances.end()) {
        it->second = InputUnregistered;
    }
}

void Input::reset() {
    sg_keyInstances.clear();
    sg_buttonInstances.clear();
}
