#include "RoboCatClientPCH.hpp"


PlayerUIComponent::PlayerUIComponent(GameObject* inGameObject)
	
{
	mPlayer = dynamic_cast<RoboCat*>(inGameObject);
	mHealthSprite.setTexture(*TextureManager::sInstance->GetTexture("healthRefill"));
	mSnowballSprite.setTexture(*TextureManager::sInstance->GetTexture("snowball"));
}

void PlayerUIComponent::Render()
{
	if (mPlayer == nullptr)
	{
		return;
	}

	// Get the position of the player
	auto pos = mPlayer->GetLocation();
	auto health = mPlayer->GetHealth();
	auto ammo = mPlayer->GetAmmo();

	const int healths = mPlayer->GetMaxHealth();
	const int snowballs = mPlayer->GetMaxAmmo();

	const float spacingHealth = 15.f;
	const float spacingSnowball = 15.f;
	const float scaleSnow = 0.5f;
	const float scaleHealth = 0.3f;

	/*const float lifeRow = pos.mY - 40.f;
	const float snowballRow = pos.mY + 25.f;*/

	const float lifeRow = pos.mY + 23.f;
	const float snowballRow = pos.mY + 35.f;
	const float nameRow = pos.mY - 45.f;

	// player name from scoreboard manager
	uint32_t playerId = mPlayer->GetPlayerId();
	auto entry = ScoreBoardManager::sInstance->GetEntry(playerId);

	if (entry)
	{
		sf::Text nameText;
		nameText.setFont(*FontManager::sInstance->GetFont("carlito"));
		nameText.setString(entry->GetPlayerName());
		nameText.setCharacterSize(20);
		nameText.setFillColor(sf::Color::Black);
		//bold
		nameText.setStyle(sf::Text::Bold);
		nameText.setPosition(pos.mX - (nameText.getGlobalBounds().width / 2), nameRow);
		WindowManager::sInstance->draw(nameText);
	}
	else {
		LOG("Player name not found in scoreboard manager", string());
	}

	//Render hearts above the player
	float healthStartX = pos.mX - (healths * spacingHealth) / 2.f;
	for (int i = 0; i < health; ++i)
	{
		mHealthSprite.setPosition(healthStartX + (i * spacingHealth), lifeRow);
		mHealthSprite.setScale(scaleHealth, scaleHealth);
		mHealthSprite.setColor(sf::Color(255,255,255, 191));
		WindowManager::sInstance->draw(mHealthSprite);
	}


	//Render snowballs above the player
	float snowballStartX = pos.mX - (snowballs * spacingSnowball) / 2.f;
	for (int i = 0; i < ammo; ++i)
	{
		mSnowballSprite.setPosition(snowballStartX + (i * spacingSnowball), snowballRow);
		mSnowballSprite.setScale(scaleSnow, scaleSnow);
		mSnowballSprite.setColor(sf::Color(255, 255, 255, 191));
		WindowManager::sInstance->draw(mSnowballSprite);
	}	
}
