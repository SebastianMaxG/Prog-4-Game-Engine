#pragma once
#include "ControllerComponent.h"

namespace lsmf {
	class Player;

	class PlayerController : public ControllerComponent {
    public:
        PlayerController(GameObject* gameObject, Player* player, int joystickId = -1, bool keyboardInput = false);
        void HandleInput(SDL_Event event) override;
        void Update(double deltaTime) override;
        bool IsMoving();
    private:
        const int JOYSTICK_DEADZONE = 8000;
        int m_ControllerID;

        bool m_UseKeyboard;

        Player* m_Player;
        SDL_KeyCode m_LastKey{};
    };
}

