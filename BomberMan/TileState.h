#pragma once

namespace lsmf
{
	class GameObject;
	class Tile;


    enum class TileState
    {
        Exit = 0,
        Wall = 1,
        Empty = 2,
        Crate = 3,
        Bomb = 4,
        PowerUp = 5,
        Explosion = 6
    };

    class TileStateBase
    {
    public:
        virtual ~TileStateBase() = default;
        virtual TileState Enter(Tile* tile) = 0;
        virtual void Update(Tile* tile, double deltaTime) = 0;
        virtual void OnCollision(Tile* tile, GameObject* collider, GameObject* other) = 0;
    };

    class ExitState : public TileStateBase
    {
    public:
        TileState Enter(Tile* tile) override;
        void Update(Tile* tile, double deltaTime) override;
        void OnCollision(Tile* tile, GameObject* collider, GameObject* other) override;
    };

    class CrateState : public TileStateBase
    {
    public:
        TileState Enter(Tile* tile) override;
        void Update(Tile* tile, double deltaTime) override;
        void OnCollision(Tile* tile, GameObject* collider, GameObject* other) override;
    };

    class WallState : public TileStateBase
    {
    public:
        TileState Enter(Tile* tile) override;
        void Update(Tile* tile, double deltaTime) override;
        void OnCollision(Tile* tile, GameObject* collider, GameObject* other) override;
    };

    class EmptyState : public TileStateBase
    {
    public:
        TileState Enter(Tile* tile) override;
        void Update(Tile* tile, double deltaTime) override;
        void OnCollision(Tile* tile, GameObject* collider, GameObject* other) override;
    };

    class BombState : public TileStateBase
    {
    public:
        TileState Enter(Tile* tile) override;
        void Update(Tile* tile, double deltaTime) override;
        void OnCollision(Tile* tile, GameObject* collider, GameObject* other) override;
    };

    class PowerUpState : public TileStateBase
    {
    public:
        TileState Enter(Tile* tile) override;
        void Update(Tile* tile, double deltaTime) override;
        void OnCollision(Tile* tile, GameObject* collider, GameObject* other) override;
    };

    class ExplosionState : public TileStateBase
    {
    public:
        TileState Enter(Tile* tile) override;
        void Update(Tile* tile, double deltaTime) override;
        void OnCollision(Tile* tile, GameObject* collider, GameObject* other) override;
    };
}
