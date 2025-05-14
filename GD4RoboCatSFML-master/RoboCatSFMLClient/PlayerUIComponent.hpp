//Dominik Hampejs D00250604
//Gracie Chaudhary D00251769
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

