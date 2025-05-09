#include "RoboCatClientPCH.hpp"

MouseClient::MouseClient()
{
	mSpriteComponent.reset(new SpriteComponent(this));

	//check if its  health or snowball
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
