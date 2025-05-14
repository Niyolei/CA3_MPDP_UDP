//Dominik Hampejs D00250604
//Gracie Chaudhary D00251769
#include "RoboCatClientPCH.hpp"

bool Client::StaticInit()
{
	// Create the Client pointer first because it initializes SDL
	Client* client = new Client();
	InputManager::StaticInit();

	WindowManager::StaticInit();
	FontManager::StaticInit();
	TextureManager::StaticInit();
	RenderManager::StaticInit();
	TexturedWorld::StaticInit();
	HUD::StaticInit();
	AudioManager::StaticInit();

	AudioManager::sInstance->PlayMusic("game", true);
	AudioManager::sInstance->SetMusicVolume(50.0f);
	AudioManager::sInstance->RemoveStoppedSounds();

	s_instance.reset(client);

	return true;
}

Client::Client()
{
	GameObjectRegistry::sInstance->RegisterCreationFunction('RCAT', RoboCatClient::StaticCreate);
	GameObjectRegistry::sInstance->RegisterCreationFunction('MOUS', MouseClient::StaticCreate);
	GameObjectRegistry::sInstance->RegisterCreationFunction('YARN', YarnClient::StaticCreate);

	string destination = StringUtils::GetCommandLineArg(1);
	string name = StringUtils::GetCommandLineArg(2);

	//limit name to 4
	if (name.length() > 4)
	{
		name = name.substr(0, 4);
	}

	SocketAddressPtr serverAddress = SocketAddressFactory::CreateIPv4FromString(destination);

	NetworkManagerClient::StaticInit(*serverAddress, name);

	//NetworkManagerClient::sInstance->SetSimulatedLatency(0.0f);
}



void Client::DoFrame()
{
	InputManager::sInstance->Update();

	if (NetworkManagerClient::sInstance->HasGameStarted())
	{
		Engine::DoFrame();

	}

	NetworkManagerClient::sInstance->ProcessIncomingPackets();

	RenderManager::sInstance->Render();

	NetworkManagerClient::sInstance->SendOutgoingPackets();

	Engine::SetShouldKeepRunning(!InputManager::sInstance->mWantsToExit);
}

void Client::HandleEvent(sf::Event& p_event)
{
	switch (p_event.type)
	{
	case sf::Event::KeyPressed:
		InputManager::sInstance->HandleInput(EIA_Pressed, p_event.key.code);
		break;
	case sf::Event::KeyReleased:
		InputManager::sInstance->HandleInput(EIA_Released, p_event.key.code);
		break;
	default:
		break;
	}
}

bool Client::PollEvent(sf::Event& p_event)
{
	return WindowManager::sInstance->pollEvent(p_event);
}


