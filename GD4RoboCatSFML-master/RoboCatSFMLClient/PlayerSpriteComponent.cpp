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
}

//setting up 8 directions of movement in update



sf::Sprite& PlayerSpriteComponent::GetSprite()
{
	// Update the sprite based on the game object stuff.
	auto pos = mGameObject->GetLocation();
	auto rot = mGameObject->GetRotation();
	mAnimation.GetSprite().setPosition(pos.mX, pos.mY);
	mAnimation.GetSprite().setRotation(rot);
	RoboCat* player = dynamic_cast<RoboCat*>(mGameObject);
	Vector3 playerColor = player->GetColor();
	mAnimation.GetSprite().setColor(sf::Color(playerColor.mX, playerColor.mY, playerColor.mZ, 255));
	mAnimation.GetSprite().setPosition(pos.mX, pos.mY);

	return mAnimation.GetSprite();
}

void PlayerSpriteComponent::Update(sf::Time dt)
{
	UpdateWalkingAnimation(dt);
	//mAnimation.Update(dt);
}

void PlayerSpriteComponent::UpdateWalkingAnimation(sf::Time dt)
{
	const Vector3& dir = mPlayer->GetFacingVector();
	Vector3 velocity = mPlayer->GetVelocity();
	bool isMoving = velocity.Length2D() > mPlayer->GetVelocityCutoffValue();

	if (isMoving)
	{
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

