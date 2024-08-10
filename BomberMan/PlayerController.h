#pragma once
#include "ControllerComponent.h"

namespace lsmf {
	class Player;
    class Enemy;

	class PlayerController : public ControllerComponent {
    public:
        PlayerController(GameObject* gameObject, Player* player, int joystickId = -1, bool keyboardInput = false);
        PlayerController(GameObject* gameObject, Enemy* player, int joystickId = -1, bool keyboardInput = false);
        ~PlayerController() override;

        void HandleInput(SDL_Event event) override;
        void PlayerMove(SDL_Event event);
        void Action1(SDL_Event event);
        void Action2(SDL_Event event);
        void NextLevel(SDL_Event event);

        void InitCommands();
        
        void Update(double deltaTime) override;
        bool IsMoving();
    private:
        const int JOYSTICK_DEADZONE = 8000;
        int m_ControllerID;

        bool m_UseKeyboard;

        float yDir{};
        float xDir{};

        Player* m_Player;
        Enemy* m_Enemy{nullptr};
    };
}

