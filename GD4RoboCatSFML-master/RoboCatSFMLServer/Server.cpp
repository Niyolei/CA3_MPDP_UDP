
#include "RoboCatServerPCH.hpp"
#include <iostream>



//uncomment this when you begin working on the server

bool Server::StaticInit()
{
	s_instance.reset(new Server());

	return true;
}

Server::Server()
{

	GameObjectRegistry::sInstance->RegisterCreationFunction('RCAT', RoboCatServer::StaticCreate);
	GameObjectRegistry::sInstance->RegisterCreationFunction('MOUS', MouseServer::StaticCreate);
	GameObjectRegistry::sInstance->RegisterCreationFunction('YARN', YarnServer::StaticCreate);

	InitNetworkManager();

	// Setup latency
	float latency = 0.0f;
	string latencyString = StringUtils::GetCommandLineArg(2);
	if (!latencyString.empty())
	{
		latency = stof(latencyString);
	}
	NetworkManagerServer::sInstance->SetSimulatedLatency(latency);
}


int Server::Run()
{
	SetupWorld();

	return Engine::Run();
}

bool Server::InitNetworkManager()
{
	string portString = StringUtils::GetCommandLineArg(1);
	uint16_t port = stoi(portString);

	return NetworkManagerServer::StaticInit(port);
}


namespace
{
	int LastQuadrant = -1;
	void CreateRandomMice(int inMouseCount)
	{
		//Vector3 mouseMin(100.f, 100.f, 0.f);
		//Vector3 mouseMax(1180.f, 620.f, 0.f);
		//GameObjectPtr go;

		////make a mouse somewhere- where will these come from?
		//for (int i = 0; i < inMouseCount; ++i)
		//{
		//	go = GameObjectRegistry::sInstance->CreateGameObject('MOUS');
		//	Vector3 mouseLocation = RoboMath::GetRandomVector(mouseMin, mouseMax);
		//	go->SetLocation(mouseLocation);
		//}

		float border_distance = 65.f;

		//auto type = static_cast<PickupType>(Utility::RandomInt(static_cast<int>(PickupType::kPickupCount)));
		//sf::Int16 pickup_id = m_pickup_counter++;


		//std::unique_ptr<Pickup> pickup = std::make_unique<Pickup>(pickup_id, type, m_textures);

		GameObjectPtr go;
		go = GameObjectRegistry::sInstance->CreateGameObject('MOUS');

		
		int quadrant = RoboMath::GetRandomFloat() * 4;
		while (quadrant == LastQuadrant) {
			quadrant = RoboMath::GetRandomFloat() * 4;
		}
		LastQuadrant = quadrant;

		Vector3 position;

		float mid_x = kWindowSize.x / 2;
		float mid_y = kWindowSize.y / 2;

		Vector3 minOne = Vector3(0.f, 0.f, 0.f);
		Vector3 maxOne = Vector3(mid_x, mid_y, 0.f);

		Vector3 minTwo = Vector3(mid_x, 0.f, 0.f);
		Vector3 maxTwo = Vector3(kWindowSize.x, mid_y, 0.f);

		Vector3 minThree = Vector3(0.f, mid_y, 0.f);
		Vector3 maxThree = Vector3(mid_x, kWindowSize.y, 0.f);

		Vector3 minFour = Vector3(mid_x, mid_y, 0.f);
		Vector3 maxFour = Vector3(kWindowSize.x, kWindowSize.y, 0.f);

		switch (quadrant) {
		case 0: // Top-left quadrant
			position = RoboMath::GetRandomVector(minOne, maxOne);
			break;
		case 1: // Top-right quadrant
			position = RoboMath::GetRandomVector(minTwo, maxTwo);
			break;
		case 2: // Bottom-left quadrant
			position = RoboMath::GetRandomVector(minThree, maxThree);
			break;
		case 3: // Bottom-right quadrant
			position = RoboMath::GetRandomVector(minFour, maxFour);
			break;
		default:
			return;
		}

		go->SetLocation(position);
	}


}


void Server::SetupWorld()
{
	//spawn some random mice
	CreateRandomMice(10);

	//spawn more random mice!
	//CreateRandomMice(10);
}

void Server::DoFrame()
{
	NetworkManagerServer::sInstance->ProcessIncomingPackets();

	NetworkManagerServer::sInstance->CheckForDisconnects();

	NetworkManagerServer::sInstance->RespawnCats();

	Engine::DoFrame();

	NetworkManagerServer::sInstance->SendOutgoingPackets();

}

void Server::HandleNewClient(ClientProxyPtr inClientProxy)
{

	int playerId = inClientProxy->GetPlayerId();

	ScoreBoardManager::sInstance->AddEntry(playerId, inClientProxy->GetName());
	SpawnCatForPlayer(playerId);
}

void Server::SpawnCatForPlayer(int inPlayerId)
{
	RoboCatPtr cat = std::static_pointer_cast<RoboCat>(GameObjectRegistry::sInstance->CreateGameObject('RCAT'));
	cat->SetColor(ScoreBoardManager::sInstance->GetEntry(inPlayerId)->GetColor());
	cat->SetPlayerId(inPlayerId);
	//gotta pick a better spawn location than this...
	cat->SetLocation(Vector3(600.f - static_cast<float>(inPlayerId), 400.f, 0.f));
}

void Server::HandleLostClient(ClientProxyPtr inClientProxy)
{
	//kill client's cat
	//remove client from scoreboard
	int playerId = inClientProxy->GetPlayerId();

	ScoreBoardManager::sInstance->RemoveEntry(playerId);
	RoboCatPtr cat = GetCatForPlayer(playerId);
	if (cat)
	{
		cat->SetDoesWantToDie(true);
	}
}

RoboCatPtr Server::GetCatForPlayer(int inPlayerId)
{
	//run through the objects till we find the cat...
	//it would be nice if we kept a pointer to the cat on the clientproxy
	//but then we'd have to clean it up when the cat died, etc.
	//this will work for now until it's a perf issue
	const auto& gameObjects = World::sInstance->GetGameObjects();
	for (int i = 0, c = gameObjects.size(); i < c; ++i)
	{
		GameObjectPtr go = gameObjects[i];
		RoboCat* cat = go->GetAsCat();
		if (cat && cat->GetPlayerId() == inPlayerId)
		{
			return std::static_pointer_cast<RoboCat>(go);
		}
	}

	return nullptr;

}
