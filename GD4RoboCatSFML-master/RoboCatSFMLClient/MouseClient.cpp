#include "RoboCatClientPCH.hpp"

MouseClient::MouseClient()
{
	mSpriteComponent.reset(new SpriteComponent(this));
}

void MouseClient::Read(InputMemoryBitStream& inInputStream)
{
	bool stateBit;

	inInputStream.Read(stateBit);
	if (stateBit)
	{
		Vector3 location;
		inInputStream.Read(location.mX);
		inInputStream.Read(location.mY);
		SetLocation(location);
	}

	inInputStream.Read(stateBit);
	if (stateBit)
	{
		bool isHealthType;
		inInputStream.Read(isHealthType);
		mIsHealth = isHealthType;
		UpdateSprite();
	}
}

void MouseClient::UpdateSprite()
{
	if (mIsHealth)
	{
		SetScale(0.85f);
		//SetCollisionRadius(20.f);
		mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture("healthRefill"));
	}
	else
	{
		SetScale(0.85f);
		//SetCollisionRadius(20.f);
		mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture("snowballRefill"));
	}
}
