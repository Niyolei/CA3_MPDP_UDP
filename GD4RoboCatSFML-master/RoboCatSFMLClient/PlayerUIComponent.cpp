#include "RoboCatClientPCH.hpp"


PlayerUIComponent::PlayerUIComponent(GameObject* inGameObject)
	
{
	mPlayer = dynamic_cast<RoboCat*>(inGameObject);
	mHealthSprite.setTexture(*TextureManager::sInstance->GetTexture("healthRefill"));
	mSnowballSprite.setTexture(*TextureManager::sInstance->GetTexture("snowball"));
}

void PlayerUIComponent::Render()
{
	
	// Get the position of the player
	auto pos = mPlayer->GetLocation();
	auto health = mPlayer->GetHealth();
	auto ammo = mPlayer->GetAmmo();

	const int healths = mPlayer->GetMaxHealth();
	const int snowballs = mPlayer->GetMaxAmmo();

	const float spacingHealth = 20.f;
	const float spacingSnowball = 15.f;
	const float scale = 0.5f;

	const float lifeRow = pos.mY - 40.f;
	const float snowballRow = pos.mY + 25.f;

	//Render hearts above the player
	float healthStartX = pos.mX - (healths * spacingHealth) / 2.f;
	for (int i = 0; i < health; ++i)
	{
		mHealthSprite.setPosition(healthStartX + (i * spacingHealth), lifeRow);
		mHealthSprite.setScale(scale, scale);

		WindowManager::sInstance->draw(mHealthSprite);
	}


	//Render snowballs above the player
	float snowballStartX = pos.mX - (snowballs * spacingSnowball) / 2.f;
	for (int i = 0; i < ammo; ++i)
	{
		mSnowballSprite.setPosition(snowballStartX + (i * spacingSnowball), snowballRow);
		mSnowballSprite.setScale(0.5f, 0.5f);
		WindowManager::sInstance->draw(mSnowballSprite);
	}

	
}
