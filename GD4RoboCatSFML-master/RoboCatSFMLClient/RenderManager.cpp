//Dominik Hampejs D00250604
//Gracie Chaudhary D00251769
#include "RoboCatClientPCH.hpp"

std::unique_ptr< RenderManager >	RenderManager::sInstance;

RenderManager::RenderManager()
{
	view.reset(sf::FloatRect(0, 0, kWindowSize.x, kWindowSize.y));
	WindowManager::sInstance->setView(view);
}


void RenderManager::StaticInit()
{
	sInstance.reset(new RenderManager());
}


void RenderManager::AddComponent(SpriteComponent* inComponent)
{
	mComponents.emplace_back(inComponent);
}

void RenderManager::RemoveComponent(SpriteComponent* inComponent)
{
	int index = GetComponentIndex(inComponent);

	if (index != -1)
	{
		int lastIndex = mComponents.size() - 1;
		if (index != lastIndex)
		{
			mComponents[index] = mComponents[lastIndex];
		}
		mComponents.pop_back();
	}
}

int RenderManager::GetComponentIndex(SpriteComponent* inComponent) const
{
	for (int i = 0, c = mComponents.size(); i < c; ++i)
	{
		if (mComponents[i] == inComponent)
		{
			return i;
		}
	}

	return -1;
}

void RenderManager::AddPlayerUIComponent(PlayerUIComponent* inComponent)
{
	mPlayerUIComponents.emplace_back(inComponent);
}

void RenderManager::RemovePlayerUIComponent(PlayerUIComponent* inComponent)
{
	int index = GetPlayerUIComponentIndex(inComponent);
	if (index != -1)
	{
		int lastIndex = mPlayerUIComponents.size() - 1;
		if (index != lastIndex)
		{
			mPlayerUIComponents[index] = mPlayerUIComponents[lastIndex];
		}
		mPlayerUIComponents.pop_back();
	}
}

int RenderManager::GetPlayerUIComponentIndex(PlayerUIComponent* inComponent) const
{
	for (int i = 0, c = mPlayerUIComponents.size(); i < c; ++i)
	{
		if (mPlayerUIComponents[i] == inComponent)
		{
			return i;
		}
	}
	return -1;
}


//this part that renders the world is really a camera-
//in a more detailed engine, we'd have a list of cameras, and then render manager would
//render the cameras in order
void RenderManager::RenderComponents()
{

	sf::Time dt = sf::seconds(Timing::sInstance.GetDeltaTime());
	for (SpriteComponent* c : mComponents)
	{
		c->Update(dt);
	}
	//Get the logical viewport so we can pass this to the SpriteComponents when it's draw time
	for (SpriteComponent* c : mComponents)
	{	
		WindowManager::sInstance->draw(c->GetSprite());	
	}
}

void RenderManager::RenderTexturedWorld()
{
	for (auto spr : TexturedWorld::sInstance->getTexturedWorld())
	{
		WindowManager::sInstance->draw(spr);
	}
}

void RenderManager::RenderPlayerUI()
{
	for (PlayerUIComponent* c : mPlayerUIComponents)
	{
		c->Render();
	}
}

void RenderManager::Render()
{
	// Clear the back buffer
	WindowManager::sInstance->clear(sf::Color(100, 149, 237, 255));	

	RenderManager::sInstance->RenderTexturedWorld();
	RenderManager::sInstance->RenderComponents();
	HUD::sInstance->Render();	
	RenderPlayerUI();
	WindowManager::sInstance->display();	
	
}
