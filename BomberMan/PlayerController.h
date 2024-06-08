#pragma once
#include "ControllerComponent.h"

namespace lsmf {
    class PlayerController : public ControllerComponent {
    public:
        PlayerController(GameObject* gameObject, int joystickId = -1);
        void HandleInput(SDL_Event event) override;

    private:
        int joystickId;
    };
}

