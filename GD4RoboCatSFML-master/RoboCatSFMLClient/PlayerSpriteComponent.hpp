#pragma once
class PlayerSpriteComponent : public SpriteComponent
{

	enum class FacingDirection
	{
		kDown = 0,
		kDownLeft = 1,
		kDownRight = 2,
		kLeft = 3,
		kRight = 4,
		kUp = 5,
		kUpLeft = 6,
		kUpRight = 7
	};

	public:
		PlayerSpriteComponent(GameObject* inGameObject);
		virtual sf::Sprite& GetSprite() override;

		void SetAnimation(CharacterAnimation* animation);
		CharacterAnimation& GetAnimation();
		void Update(sf::Time dt);
		void UpdateWalkingAnimation(sf::Time dt);

	private: 
		CharacterAnimation mAnimation;
		RoboCat* mPlayer;
		FacingDirection mLastDirection;
};
typedef shared_ptr<PlayerSpriteComponent>	PlayerSpriteComponentPtr;

