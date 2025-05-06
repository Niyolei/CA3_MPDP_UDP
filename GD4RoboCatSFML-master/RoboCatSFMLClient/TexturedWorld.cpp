#include "RoboCatClientPCH.hpp"

std::unique_ptr<TexturedWorld> TexturedWorld::sInstance;

void TexturedWorld::StaticInit()
{
    sInstance.reset(new TexturedWorld());
    sInstance->Load();
}

bool TexturedWorld::Load()
{
    if (!BuildSnowLandscape())
        return false;

	if (!BuildLakeLandscape())	
		return false;

	if (!BuildTreeLandscape())
		return false;

    return true;
}

std::vector<sf::Sprite>& TexturedWorld::getTexturedWorld()
{
    return m_backgroundSpriteTiles;
}

//build snowy landscape

bool TexturedWorld::BuildSnowLandscape()
{
	//load the snow tile texture
	TexturePtr snowTileTexture = TextureManager::sInstance->GetTexture("snowTile");
	
	if (!snowTileTexture)
	{
		return false;
	}

	snowTileTexture->setRepeated(true);
	
	sf::IntRect snowTextureRect(0, 0, kWindowSize.x, kWindowSize.y);

	sf::Sprite snowTileSprite;

	for (int i = 0; i < kWindowSize.x / 64; ++i)
	{
		for (int j = 0; j < kWindowSize.y / 64; ++j)
		{
			snowTileSprite.setTexture(*snowTileTexture);
			snowTileSprite.setTextureRect(snowTextureRect);
			snowTileSprite.setPosition(i * 64, j * 64);
			m_backgroundSpriteTiles.push_back(snowTileSprite);
		}
	}

	return true;
}

//build lake landscape
bool TexturedWorld::BuildLakeLandscape()
{	
	const float borderThickness = 64.f;

	// Load texture
	TexturePtr lakeTileTexture = TextureManager::sInstance->GetTexture("lakeTile");
	if (!lakeTileTexture)
		return false;

	lakeTileTexture->setRepeated(true);

	// Compute bounds where lake tiles should be placed
	int lakeStartX = borderThickness;
	int lakeEndX = kWindowSize.x - borderThickness;
	int lakeStartY = borderThickness;
	int lakeEndY = kWindowSize.y - borderThickness;

	// Use a 64x64 texture rect
	sf::IntRect lakeTextureRect(0, 0, 64, 64);
	sf::Sprite lakeTileSprite;
	lakeTileSprite.setTexture(*lakeTileTexture);
	lakeTileSprite.setTextureRect(lakeTextureRect);

	// Fill only the central region, leaving a border
	for (int x = lakeStartX; x < lakeEndX; x += 64)
	{
		for (int y = lakeStartY; y < lakeEndY; y += 64)
		{
			lakeTileSprite.setPosition(x, y);
			m_backgroundSpriteTiles.push_back(lakeTileSprite);
		}
	}

	return true;
}

//build tree landscape
bool TexturedWorld::BuildTreeLandscape()
{
	// Load the tree texture
	TexturePtr treeTexture = TextureManager::sInstance->GetTexture("greenTree");
	if (!treeTexture)
		return false;

	const float borderThickness = 64.f;

	// Tree size and spacing ranges
	const float minTreeSize = 32.f;  // Minimum tree size
	const float maxTreeSize = 72.f;  // Maximum tree size
	const float minTreeSpacing = -16.f;  // Minimum spacing between trees
	const float maxTreeSpacing = 4.f;  // Maximum spacing between trees
	const float maxOffset = 10.f;  // Max offset range for natural placement

	// Lake boundaries
	float lakeLeft = borderThickness;
	float lakeTop = borderThickness;
	float lakeRight = kWindowSize.x - borderThickness;
	float lakeBottom = kWindowSize.y - borderThickness;

	// Generate random offset between -maxOffset and +maxOffset
	auto randomOffset = [maxOffset]() {
		return (std::rand() % (static_cast<int>(maxOffset * 2 + 1))) - maxOffset;
		};

	// Generate random tree size between minTreeSize and maxTreeSize
	auto randomTreeSize = [minTreeSize, maxTreeSize]() {
		return minTreeSize + std::rand() % static_cast<int>(maxTreeSize - minTreeSize + 1);
		};

	// Generate random tree spacing between minTreeSpacing and maxTreeSpacing
	auto randomTreeSpacing = [minTreeSpacing, maxTreeSpacing]() {
		return minTreeSpacing + std::rand() % static_cast<int>(maxTreeSpacing - minTreeSpacing + 1);
		};

	// Generate trees along the borders of the lake
	for (float x = lakeLeft + randomTreeSpacing(); x <= lakeRight; x += (randomTreeSize() + randomTreeSpacing())) {
		float offsetY = randomOffset();
		float treeSize = randomTreeSize();
		
		sf::Sprite* treeSprite = new sf::Sprite(*treeTexture);
		treeSprite->setPosition(x, lakeTop - borderThickness + offsetY);
		treeSprite->setScale(treeSize / 64.f, treeSize / 64.f);

		m_backgroundSpriteTiles.push_back(*treeSprite);		
	}

	for (float x = lakeLeft + randomTreeSpacing(); x <= lakeRight; x += (randomTreeSize() + randomTreeSpacing())) {
		float offsetY = randomOffset();
		float treeSize = randomTreeSize();
		sf::Sprite* treeSprite = new sf::Sprite(*treeTexture);
		treeSprite->setPosition(x, lakeBottom + offsetY);
		treeSprite->setScale(treeSize / 64.f, treeSize / 64.f);
		m_backgroundSpriteTiles.push_back(*treeSprite);
	}

	for (float y = lakeTop - borderThickness; y <= lakeBottom + borderThickness; y += (randomTreeSize() + randomTreeSpacing())) {
		float offsetX = randomOffset();
		float treeSize = randomTreeSize();
		sf::Sprite* treeSprite = new sf::Sprite(*treeTexture);
		treeSprite->setPosition(lakeLeft - borderThickness + offsetX, y);
		treeSprite->setScale(treeSize / 64.f, treeSize / 64.f);
		m_backgroundSpriteTiles.push_back(*treeSprite);
	}

	for (float y = lakeTop - borderThickness; y <= lakeBottom + borderThickness; y += (randomTreeSize() + randomTreeSpacing())) {
		float offsetX = randomOffset();
		float treeSize = randomTreeSize();
		sf::Sprite* treeSprite = new sf::Sprite(*treeTexture);
		treeSprite->setPosition(lakeRight + offsetX, y);
		treeSprite->setScale(treeSize / 64.f, treeSize / 64.f);
		m_backgroundSpriteTiles.push_back(*treeSprite);
	}

	/*
	sf::Texture& green_tree_texture = m_textures.Get(TextureID::kGreenTree);
	const float border_thickness = 64.f;
	// Tree size and spacing ranges
	const float min_tree_size = 32.f;  // Minimum tree size
	const float max_tree_size = 72.f;  // Maximum tree size
	const float min_tree_spacing = -16.f;  // Minimum spacing between trees
	const float max_tree_spacing = 4.f;  // Maximum spacing between trees
	const float max_offset = 10.f;  // Max offset range for natural placement

	std::srand(static_cast<unsigned int>(std::time(0)));

	// Lake boundaries
	float lake_left = lake_bounds.left - 10;
	float lake_top = lake_bounds.top - 10;
	float lake_right = lake_bounds.left + lake_bounds.width + 10;
	float lake_bottom = lake_bounds.top + lake_bounds.height + 10;

	//  to generate random offset between -max_offset and +max_offset
	auto randomOffset = [max_offset]() {
		return (std::rand() % (static_cast<int>(max_offset * 2 + 1))) - max_offset;
		};

	// to generate random tree size between min_tree_size and max_tree_size
	auto randomTreeSize = [min_tree_size, max_tree_size]() {
		return min_tree_size + std::rand() % static_cast<int>(max_tree_size - min_tree_size + 1);
		};

	// to generate random tree spacing between min_tree_spacing and max_tree_spacing
	auto randomTreeSpacing = [min_tree_spacing, max_tree_spacing]() {
		return min_tree_spacing + std::rand() % static_cast<int>(max_tree_spacing - min_tree_spacing + 1);
		};

	for (float x = lake_left + randomTreeSpacing(); x <= lake_right; x += (randomTreeSize() + randomTreeSpacing())) {
		float offsetY = randomOffset();
		float tree_size = randomTreeSize();

		std::unique_ptr<SpriteNode> tree_sprite(new SpriteNode(green_tree_texture));
		tree_sprite->setPosition(x, lake_top - border_thickness + offsetY);
		tree_sprite->setScale(tree_size / 64.f, tree_size / 64.f);
		m_scene_layers[static_cast<int>(SceneLayers::kBackground)]->AttachChild(std::move(tree_sprite));
	}


	for (float x = lake_left + randomTreeSpacing(); x <= lake_right; x += (randomTreeSize() + randomTreeSpacing())) {
		float offsetY = randomOffset();
		float tree_size = randomTreeSize();

		std::unique_ptr<SpriteNode> tree_sprite(new SpriteNode(green_tree_texture));
		tree_sprite->setPosition(x, lake_bottom + offsetY);
		tree_sprite->setScale(tree_size / 64.f, tree_size / 64.f);
		m_scene_layers[static_cast<int>(SceneLayers::kBackground)]->AttachChild(std::move(tree_sprite));
	}


	for (float y = lake_top - border_thickness; y <= lake_bottom + border_thickness; y += (randomTreeSize() + randomTreeSpacing())) {
		float offsetX = randomOffset();
		float tree_size = randomTreeSize();

		std::unique_ptr<SpriteNode> tree_sprite(new SpriteNode(green_tree_texture));
		tree_sprite->setPosition(lake_left - border_thickness + offsetX, y);
		tree_sprite->setScale(tree_size / 64.f, tree_size / 64.f);
		m_scene_layers[static_cast<int>(SceneLayers::kBackground)]->AttachChild(std::move(tree_sprite));
	}


	for (float y = lake_top - border_thickness; y <= lake_bottom + border_thickness; y += (randomTreeSize() + randomTreeSpacing())) {
		float offsetX = randomOffset();
		float tree_size = randomTreeSize();

		std::unique_ptr<SpriteNode> tree_sprite(new SpriteNode(green_tree_texture));
		tree_sprite->setPosition(lake_right + offsetX, y);
		tree_sprite->setScale(tree_size / 64.f, tree_size / 64.f);
		m_scene_layers[static_cast<int>(SceneLayers::kBackground)]->AttachChild(std::move(tree_sprite));
	}
	
	*/
	return true;
}
