#include "RoboCatClientPCH.hpp"


PlayerUIComponent::PlayerUIComponent(GameObject* inGameObject)
	
{
	mPlayer = dynamic_cast<RoboCat*>(inGameObject);
	mHeartSprite.setTexture(*TextureManager::sInstance->GetTexture("healthRefill"));
	mSnowballSprite.setTexture(*TextureManager::sInstance->GetTexture("snowball"));
}

void PlayerUIComponent::Render()
{
	
	// Get the position of the player
	auto pos = mPlayer->GetLocation();
	auto health = mPlayer->GetHealth();
	auto ammo = mPlayer->GetAmmo();

	//Render hearts above the player
	for (int i = 0; i < mPlayer->GetHealth(); ++i)
	{
		mHeartSprite.setPosition(pos.mX - 50 + (i * 20), pos.mY - 50);
		mHeartSprite.setScale(0.5f, 0.5f);

		WindowManager::sInstance->draw(mHeartSprite);
	}


	//Render snowballs above the player
	for (int i = 0; i < mPlayer->GetAmmo(); ++i)
	{
		mSnowballSprite.setPosition(pos.mX + 50 + (i * 20), pos.mY - 50);
		mSnowballSprite.setScale(0.5f, 0.5f);
		WindowManager::sInstance->draw(mSnowballSprite);
	}

	
}
