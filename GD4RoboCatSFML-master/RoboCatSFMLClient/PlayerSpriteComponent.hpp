//Dominik Hampejs D00250604
//Gracie Chaudhary D00251769
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

		void SetCurrentAnimationState(CharacterAnimation::CharacterAnimationState state);
		CharacterAnimation::CharacterAnimationState GetCurrentAnimationState() const;

		void Update(sf::Time dt);

		void UpdateAnimation(sf::Time dt);
		void UpdateWalkingAnimation(sf::Time dt);
		void UpdateAttackingAnimation(sf::Time dt);
		void UpdateImpactAnimation(sf::Time dt);
		void UpdateIdleAnimation(sf::Time dt);

		void UpdateLastDirection();

	private: 
		CharacterAnimation mAnimation;
		RoboCat* mPlayer;

		bool mActive;

		CharacterAnimation::CharacterAnimationState mCurrentAnimationState;

		FacingDirection mLastDirection;

		sf::Time mBlinkTimer = sf::Time::Zero;
		sf::Time mImpactTimer = sf::Time::Zero;
		const sf::Time mImpactDuration = sf::seconds(1.f); 
		sf::Color mOriginalColor;
				
};
typedef shared_ptr<PlayerSpriteComponent>	PlayerSpriteComponentPtr;

