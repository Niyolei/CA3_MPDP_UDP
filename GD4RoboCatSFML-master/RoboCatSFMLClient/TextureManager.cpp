//Dominik Hampejs D00250604
//Gracie Chaudhary D00251769
#include "RoboCatClientPCH.hpp"

std::unique_ptr< TextureManager >		TextureManager::sInstance;

void TextureManager::StaticInit()
{
	sInstance.reset(new TextureManager());
}

TextureManager::TextureManager()
{
	CacheTexture("cat", "../Assets/cat.png");
	CacheTexture("mouse", "../Assets/mouse.png");
	CacheTexture("yarn", "../Assets/yarn.png");

	//environment textures
	CacheTexture("snowTile", "../MediaFiles/Textures/Environment/SnowTile_64x64.png");
	CacheTexture("lakeTile", "../MediaFiles/Textures/Environment/LakeTile_64x64.png");
	CacheTexture("greenTree", "../MediaFiles/Textures/Tree/GreenTree_64x64.png");	

	//character textures
	CacheTexture("character", "../MediaFiles/Textures/Character/CharacterMovementSheet.png");

	//snowball textures
	CacheTexture("snowball", "../MediaFiles/Textures/Weapon/Snowball.png");

	//pickups textures
	CacheTexture("healthRefill", "../MediaFiles/Textures/UI/HealthPickupV2.png");
	CacheTexture("snowballRefill", "../MediaFiles/Textures/UI/SnowballPickup.png");


	/*m_textures.Load(TextureID::kCharacterMovement, "MediaFiles/Textures/Character/CharacterMovementSheet.png");
	m_textures.Load(TextureID::kSnowball, "MediaFiles/Textures/Weapon/Snowball.png");
	m_textures.Load(TextureID::kSnowTile, "MediaFiles/Textures/Environment/SnowTile_64x64.png");
	m_textures.Load(TextureID::kLakeTile, "MediaFiles/Textures/Environment/LakeTile_64x64.png");
	m_textures.Load(TextureID::kPurpleTree, "MediaFiles/Textures/Tree/PurpleTree_64x64.png");
	m_textures.Load(TextureID::kGreenTree, "MediaFiles/Textures/Tree/GreenTree_64x64.png");
	m_textures.Load(TextureID::kDeadTree, "MediaFiles/Textures/Tree/DeadTree_64x64.png");*/

}

TexturePtr	TextureManager::GetTexture(const string& inTextureName)
{
	return mNameToTextureMap[inTextureName];
}

bool TextureManager::CacheTexture(string inTextureName, const char* inFileName)
{
	TexturePtr newTexture(new sf::Texture());
	if (!newTexture->loadFromFile(inFileName))
	{
		return false;
	}

	mNameToTextureMap[inTextureName] = newTexture;

	return true;

}
