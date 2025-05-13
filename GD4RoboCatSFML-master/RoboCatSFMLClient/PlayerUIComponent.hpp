#pragma once
class PlayerUIComponent
{

public:
    PlayerUIComponent(GameObject* inOwner);

    void Render(); 

private:
 
    sf::Sprite mHealthSprite;
    sf::Sprite mSnowballSprite;
	RoboCat* mPlayer;

    bool active;
};

typedef shared_ptr<PlayerUIComponent> PlayerUIComponentPtr; 

