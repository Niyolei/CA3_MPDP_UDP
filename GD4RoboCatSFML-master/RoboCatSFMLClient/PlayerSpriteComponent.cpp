//Dominik Hampejs D00250604
//Gracie Chaudhary D00251769
#include "RoboCatClientPCH.hpp"

PlayerSpriteComponent::PlayerSpriteComponent(GameObject* inGameObject) 
	:SpriteComponent(inGameObject), mAnimation()
{
	mAnimation.Init();
	mAnimation.SetFrameSize({ 38, 42 });
	mAnimation.SetTexture(TextureManager::sInstance->GetTexture("character"));
	sf::IntRect rect(0, 0, 38, 42);
	mAnimation.GetSprite().setTextureRect(rect);
	mPlayer = dynamic_cast<RoboCat*>(inGameObject);
	mOriginalColor = mAnimation.GetSprite().getColor();
}

sf::Sprite& PlayerSpriteComponent::GetSprite()
{
	// Update the sprite based on the game object stuff.
	auto pos = mGameObject->GetLocation();
	auto rot = mGameObject->GetRotation();
	mAnimation.GetSprite().setPosition(pos.mX, pos.mY);
	mAnimation.GetSprite().setRotation(rot);
	RoboCat* player = dynamic_cast<RoboCat*>(mGameObject);
	Vector3 playerColor = player->GetColor();

	if (mCurrentAnimationState != CharacterAnimation::CharacterAnimationState::kImpact)
	{
		mAnimation.GetSprite().setColor(sf::Color(playerColor.mX, playerColor.mY, playerColor.mZ, 255));
	}


	mAnimation.GetSprite().setPosition(pos.mX, pos.mY);

	return mAnimation.GetSprite();
}

void PlayerSpriteComponent::SetCurrentAnimationState(CharacterAnimation::CharacterAnimationState state)
{
	if (mCurrentAnimationState != state)
	{
		mCurrentAnimationState = state;

		switch (state)
		{
		case CharacterAnimation::CharacterAnimationState::kWalk:
			mAnimation.SetNumFrames(4);
			mAnimation.SetDuration(sf::seconds(1.f));
			mAnimation.SetRepeating(true);
			break;

		case CharacterAnimation::CharacterAnimationState::kAttack:
			mAnimation.SetNumFrames(4);
			mAnimation.SetDuration(sf::seconds(1.f));
			mAnimation.SetRepeating(false);
			break;
		} 

		mAnimation.Restart(); 
	}
}

CharacterAnimation::CharacterAnimationState PlayerSpriteComponent::GetCurrentAnimationState() const
{
	return mCurrentAnimationState;
}

void PlayerSpriteComponent::Update(sf::Time dt)
{
	

	if (mPlayer->IsShooting()) {
		SetCurrentAnimationState(CharacterAnimation::CharacterAnimationState::kAttack);
	}
	
	else if (mPlayer->IsHit())
	{
		//debug
		LOG("Player is hit", 0);
		SetCurrentAnimationState(CharacterAnimation::CharacterAnimationState::kImpact);
		mPlayer->SetIsHit(false);
	}
	
	else
	{
		Vector3 velocity = mPlayer->GetVelocity();
		bool isMoving = velocity.Length2D() > mPlayer->GetVelocityCutoffValue();

		if (isMoving)
		{
			SetCurrentAnimationState(CharacterAnimation::CharacterAnimationState::kWalk);
		}
		else
		{
			SetCurrentAnimationState(CharacterAnimation::CharacterAnimationState::kIdle);
		}
	}

	UpdateAnimation(dt);
}

void PlayerSpriteComponent::UpdateAnimation(sf::Time dt)
{
	if (mCurrentAnimationState == CharacterAnimation::CharacterAnimationState::kWalk)
	{
		UpdateWalkingAnimation(dt);
	}
	else if (mCurrentAnimationState == CharacterAnimation::CharacterAnimationState::kAttack)
	{
		UpdateAttackingAnimation(dt);
	}
	else if (mCurrentAnimationState == CharacterAnimation::CharacterAnimationState::kImpact)
	{
		UpdateImpactAnimation(dt);
	}
	else if (mCurrentAnimationState == CharacterAnimation::CharacterAnimationState::kIdle)
	{
		UpdateIdleAnimation(dt);
		
	}
}

void PlayerSpriteComponent::UpdateWalkingAnimation(sf::Time dt)
{
	
	Vector3 velocity = mPlayer->GetVelocity();
	bool isMoving = velocity.Length2D() > mPlayer->GetVelocityCutoffValue();

	if (isMoving)
	{
		
		UpdateLastDirection();

		// facing direction to row
		switch (mLastDirection)
		{
		case FacingDirection::kDown:		mAnimation.SetRow(0); break;
		case FacingDirection::kDownLeft:	mAnimation.SetRow(1); break;
		case FacingDirection::kDownRight:	mAnimation.SetRow(2); break;
		case FacingDirection::kLeft:		mAnimation.SetRow(3); break;
		case FacingDirection::kRight:		mAnimation.SetRow(4); break;
		case FacingDirection::kUp:			mAnimation.SetRow(5); break;
		case FacingDirection::kUpLeft:		mAnimation.SetRow(6); break;
		case FacingDirection::kUpRight:		mAnimation.SetRow(7); break;
		}
	}

	if (isMoving)
	{
		mAnimation.Update(dt);
	}
	else
	{
		mAnimation.Restart(); 
	}
}

void PlayerSpriteComponent::UpdateAttackingAnimation(sf::Time dt)
{
	UpdateLastDirection();

	// facing direction to row
	switch (mLastDirection)
	{
	case FacingDirection::kDown:      mAnimation.SetRow(8); break;
	case FacingDirection::kDownLeft:  mAnimation.SetRow(10); break;
	case FacingDirection::kDownRight: mAnimation.SetRow(8); break;
	case FacingDirection::kLeft:      mAnimation.SetRow(10); break;
	case FacingDirection::kRight:     mAnimation.SetRow(9); break;
	case FacingDirection::kUp:        mAnimation.SetRow(11); break;
	case FacingDirection::kUpLeft:    mAnimation.SetRow(11); break;
	case FacingDirection::kUpRight:   mAnimation.SetRow(9); break;
	}



	if (mAnimation.IsFinished())
	{
		SetCurrentAnimationState(CharacterAnimation::CharacterAnimationState::kIdle);
	}
	else
	{
		mAnimation.Update(dt);
	}
}

void PlayerSpriteComponent::UpdateImpactAnimation(sf::Time dt)
{
	UpdateLastDirection();

	switch (mLastDirection)
	{
	case FacingDirection::kDown:		mAnimation.SetRow(0); break;
	case FacingDirection::kDownLeft:	mAnimation.SetRow(1); break;
	case FacingDirection::kDownRight:	mAnimation.SetRow(2); break;
	case FacingDirection::kLeft:		mAnimation.SetRow(3); break;
	case FacingDirection::kRight:		mAnimation.SetRow(4); break;
	case FacingDirection::kUp:			mAnimation.SetRow(5); break;
	case FacingDirection::kUpLeft:		mAnimation.SetRow(6); break;
	case FacingDirection::kUpRight:		mAnimation.SetRow(7); break;
	}

	mAnimation.SetNumFrames(1);
	mAnimation.SetDuration(mImpactDuration);
	mAnimation.SetRepeating(false);
	
	
	mBlinkTimer += dt;
	if (mBlinkTimer >= sf::seconds(0.1f))
	{
		auto& sprite = mAnimation.GetSprite();
		sprite.setColor(sprite.getColor() == sf::Color::Transparent ? mOriginalColor : sf::Color::Transparent);
		mBlinkTimer = sf::Time::Zero;
	}

	mImpactTimer += dt;
	if (mImpactTimer >= mImpactDuration)
	{
		mAnimation.GetSprite().setColor(mOriginalColor);
		mImpactTimer = sf::Time::Zero;
		mBlinkTimer = sf::Time::Zero;

		//mPlayer->SetIsHit(false);

		Vector3 velocity = mPlayer->GetVelocity();
		if (velocity.Length2D() > mPlayer->GetVelocityCutoffValue())
		{
			SetCurrentAnimationState(CharacterAnimation::CharacterAnimationState::kWalk);
		}
		else
		{
			SetCurrentAnimationState(CharacterAnimation::CharacterAnimationState::kIdle);
		}
	}

	mAnimation.Update(dt);
}

void PlayerSpriteComponent::UpdateIdleAnimation(sf::Time dt)
{

	UpdateLastDirection();


	switch (mLastDirection)
	{
	case FacingDirection::kDown:		mAnimation.SetRow(0); break;
	case FacingDirection::kDownLeft:	mAnimation.SetRow(1); break;
	case FacingDirection::kDownRight:	mAnimation.SetRow(2); break;
	case FacingDirection::kLeft:		mAnimation.SetRow(3); break;
	case FacingDirection::kRight:		mAnimation.SetRow(4); break;
	case FacingDirection::kUp:			mAnimation.SetRow(5); break;
	case FacingDirection::kUpLeft:		mAnimation.SetRow(6); break;
	case FacingDirection::kUpRight:		mAnimation.SetRow(7); break;
	}


	mAnimation.SetNumFrames(1);
	mAnimation.SetDuration(sf::seconds(1.f));
	mAnimation.SetRepeating(false);
	mAnimation.Update(dt);
}

void PlayerSpriteComponent::UpdateLastDirection()
{
	const Vector3& dir = mPlayer->GetFacingVector();

	if (dir.mY > 0)
	{
		mLastDirection = dir.mX > 0 ? FacingDirection::kDownRight :
			dir.mX < 0 ? FacingDirection::kDownLeft :
			FacingDirection::kDown;
	}
	else if (dir.mY < 0)
	{
		mLastDirection = dir.mX > 0 ? FacingDirection::kUpRight :
			dir.mX < 0 ? FacingDirection::kUpLeft :
			FacingDirection::kUp;
	}
	else if (dir.mX > 0)
	{
		mLastDirection = FacingDirection::kRight;
	}
	else if (dir.mX < 0)
	{
		mLastDirection = FacingDirection::kLeft;
	}

}

