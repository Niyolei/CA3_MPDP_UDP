#pragma once
class TexturedWorld
{

public:
	static void StaticInit();
	static std::unique_ptr<TexturedWorld> sInstance;

	bool Load();
	std::vector<sf::Sprite>& getTexturedWorld();

private:
	std::vector<sf::Sprite> m_backgroundSpriteTiles;


	bool loadSprites();
	bool BuildSnowLandscape();
	bool BuildLakeLandscape();
	bool BuildTreeLandscape();

};

