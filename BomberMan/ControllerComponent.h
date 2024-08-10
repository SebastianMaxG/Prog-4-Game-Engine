#pragma once
#include "BaseComponent.h"
#include "InputManager.h"
#include "Transform.h"

namespace lsmf {
    class ControllerComponent : public BaseComponent {
    public:
        ControllerComponent(GameObject* gameObject);
        ~ControllerComponent() override;

        //delete copy constructor and assignment operator
        ControllerComponent(const ControllerComponent& other) = delete;
        ControllerComponent(ControllerComponent&& other) = delete;
        ControllerComponent& operator=(const ControllerComponent& other) = delete;
        ControllerComponent& operator=(ControllerComponent&& other) = delete;

        void FixedUpdate(double deltaTime) override;
        virtual void HandleInput(SDL_Event event) = 0;
        virtual bool GetLeft() const { return m_Left; }

        void SetSpeed(double speed) { m_Speed = speed; }

    protected:
        void Move(double deltaTime);
        bool m_Left = true;
        Transform m_MoveTransform;
    private:
        signal::Connection<SDL_Event>* m_Connection;
        double m_Speed = 1;
    };
}

