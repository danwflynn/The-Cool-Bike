#pragma once
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstring>

class Controller
{
public:
    Controller(int joystickID = GLFW_JOYSTICK_1)
        : jid(joystickID)
    {
    }

    bool isConnected() const
    {
        return glfwJoystickPresent(jid) &&
               glfwJoystickIsGamepad(jid);
    }

    const char* getName() const
    {
        if (!isConnected()) return "No controller";
        return glfwGetGamepadName(jid);
    }

    bool update()
    {
        if (!isConnected())
            return false;

        return glfwGetGamepadState(jid, &state);
    }


    bool buttonPressed(int button) const
    {
        return state.buttons[button] == GLFW_PRESS;
    }

    bool cross()  const { return buttonPressed(GLFW_GAMEPAD_BUTTON_CROSS); }
    bool circle() const { return buttonPressed(GLFW_GAMEPAD_BUTTON_CIRCLE); }
    bool square() const { return buttonPressed(GLFW_GAMEPAD_BUTTON_SQUARE); }
    bool triangle() const { return buttonPressed(GLFW_GAMEPAD_BUTTON_TRIANGLE); }

    bool l1() const { return buttonPressed(GLFW_GAMEPAD_BUTTON_LEFT_BUMPER); }
    bool r1() const { return buttonPressed(GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER); }

    bool options() const { return buttonPressed(GLFW_GAMEPAD_BUTTON_START); }
    bool share()   const { return buttonPressed(GLFW_GAMEPAD_BUTTON_BACK); }


    float leftStickX() const { return state.axes[GLFW_GAMEPAD_AXIS_LEFT_X]; }
    float leftStickY() const { return state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y]; }

    float rightStickX() const { return state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X]; }
    float rightStickY() const { return state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y]; }

    float leftTrigger() const
    {
        return (state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] + 1.0f) * 0.5f;
    }

    float rightTrigger() const
    {
        return (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] + 1.0f) * 0.5f;
    }


    static float applyDeadzone(float value, float threshold = 0.1f) {
        // 10% to 20% (0.1f to 0.2f) is the industry standard
        if (std::abs(value) < threshold) return 0.0f;
        
        // Optional: Rescale the remaining range so 0.1 becomes the new 0.0
        // and 1.0 remains 1.0. This prevents a "snap" in movement.
        return (value - (value > 0 ? threshold : -threshold)) / (1.0f - threshold);
    }

private:
    int jid;
    GLFWgamepadstate state{};
};
