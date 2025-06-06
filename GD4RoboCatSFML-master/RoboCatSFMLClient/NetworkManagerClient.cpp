//Dominik Hampejs D00250604
//Gracie Chaudhary D00251769
#include "RoboCatClientPCH.hpp"

NetworkManagerClient* NetworkManagerClient::sInstance;

namespace
{
	const float kTimeBetweenHellos = 1.f;
	const float kTimeBetweenInputPackets = 0.033f;
}

NetworkManagerClient::NetworkManagerClient() :
	mState(NCS_Uninitialized),
	mDeliveryNotificationManager(true, false),
	mLastRoundTripTime(0.f),
	mGameStarted(false)
{
}

void NetworkManagerClient::StaticInit(const SocketAddress& inServerAddress, const string& inName)
{
	sInstance = new NetworkManagerClient();
	return sInstance->Init(inServerAddress, inName);
}

void NetworkManagerClient::Init(const SocketAddress& inServerAddress, const string& inName)
{
	NetworkManager::Init(0);

	mServerAddress = inServerAddress;
	mState = NCS_SayingHello;
	mTimeOfLastHello = 0.f;
	mName = inName;

	mAvgRoundTripTime = WeightedTimedMovingAverage(1.f);
}

void NetworkManagerClient::ProcessPacket(InputMemoryBitStream& inInputStream, const SocketAddress& inFromAddress)
{
	uint32_t	packetType;
	inInputStream.Read(packetType);
	switch (packetType)
	{
	case kWelcomeCC:
		HandleWelcomePacket(inInputStream);
		break;
	case kStateCC:

		bool isGameStarted;
		inInputStream.Read(isGameStarted);
		if (isGameStarted && !mGameStarted)
		{
			mGameStarted = true;
			HUD::sInstance->SetGameState(HUD::GameState::GameStarted);
		}

		if (mDeliveryNotificationManager.ReadAndProcessState(inInputStream))
		{
			HandleStatePacket(inInputStream);
		}
		break;
	}
}


void NetworkManagerClient::SendOutgoingPackets()
{
	switch (mState)
	{
	case NCS_SayingHello:
		UpdateSayingHello();
		break;
	case NCS_Welcomed:
		if (!mGameStarted) {
			//send a ready packet
			OutputMemoryBitStream readyPacket;
			readyPacket.Write(kReadyCC);
			readyPacket.Write(InputManager::sInstance->IsReadyToStartGame());
			SendPacket(readyPacket, mServerAddress);
		}
		UpdateSendingInputPacket();
		break;
	}
}

void NetworkManagerClient::UpdateSayingHello()
{
	float time = Timing::sInstance.GetTimef();

	if (time > mTimeOfLastHello + kTimeBetweenHellos)
	{
		SendHelloPacket();
		mTimeOfLastHello = time;
	}
}

void NetworkManagerClient::SendHelloPacket()
{
	OutputMemoryBitStream helloPacket;

	helloPacket.Write(kHelloCC);
	helloPacket.Write(mName);

	SendPacket(helloPacket, mServerAddress);
}

void NetworkManagerClient::HandleWelcomePacket(InputMemoryBitStream& inInputStream)
{
	if (mState == NCS_SayingHello)
	{
		//if we got a player id, we've been welcomed!
		int playerId;
		inInputStream.Read(playerId);
		mPlayerId = playerId;
		mState = NCS_Welcomed;
		LOG("'%s' was welcomed on client as player %d", mName.c_str(), mPlayerId);
	}
}



void NetworkManagerClient::HandleStatePacket(InputMemoryBitStream& inInputStream)
{
	if (mState == NCS_Welcomed)
	{
		ReadLastMoveProcessedOnServerTimestamp(inInputStream);

		//old
		//HandleGameObjectState( inPacketBuffer );
		bool updateScoreboard;
		inInputStream.Read(updateScoreboard);
		if (updateScoreboard)
		{
			HandleScoreBoardState(inInputStream);
		}
		

		//tell the replication manager to handle the rest...
		mReplicationManagerClient.Read(inInputStream);
	}
}

void NetworkManagerClient::ReadLastMoveProcessedOnServerTimestamp(InputMemoryBitStream& inInputStream)
{
	bool isTimestampDirty;
	inInputStream.Read(isTimestampDirty);
	if (isTimestampDirty)
	{
		inInputStream.Read(mLastMoveProcessedByServerTimestamp);

		float rtt = Timing::sInstance.GetFrameStartTime() - mLastMoveProcessedByServerTimestamp;
		mLastRoundTripTime = rtt;
		mAvgRoundTripTime.Update(rtt);

		InputManager::sInstance->GetMoveList().RemovedProcessedMoves(mLastMoveProcessedByServerTimestamp);

	}
}

void NetworkManagerClient::HandleGameObjectState(InputMemoryBitStream& inInputStream)
{
	//copy the mNetworkIdToGameObjectMap so that anything that doesn't get an updated can be destroyed...
	IntToGameObjectMap	objectsToDestroy = m_network_id_to_game_object_map;

	int stateCount;
	inInputStream.Read(stateCount);
	if (stateCount > 0)
	{
		for (int stateIndex = 0; stateIndex < stateCount; ++stateIndex)
		{
			int networkId;
			uint32_t fourCC;

			inInputStream.Read(networkId);
			inInputStream.Read(fourCC);
			GameObjectPtr go;
			auto itGO = m_network_id_to_game_object_map.find(networkId);
			//didn't find it, better create it!
			if (itGO == m_network_id_to_game_object_map.end())
			{
				go = GameObjectRegistry::sInstance->CreateGameObject(fourCC);
				go->SetNetworkId(networkId);
				AddNetworkIdToGameObjectMap(go);
			}
			else
			{
				//found it
				go = itGO->second;
			}

			//now we can update into it
			go->Read(inInputStream);
			objectsToDestroy.erase(networkId);
		}
	}

	//anything left gets the axe
	DestroyGameObjectsInMap(objectsToDestroy);
}

void NetworkManagerClient::HandleScoreBoardState(InputMemoryBitStream& inInputStream)
{
	ScoreBoardManager::sInstance->Read(inInputStream);
}

void NetworkManagerClient::DestroyGameObjectsInMap(const IntToGameObjectMap& inObjectsToDestroy)
{
	for (auto& pair : inObjectsToDestroy)
	{
		pair.second->SetDoesWantToDie(true);
		//and remove from our map!
		m_network_id_to_game_object_map.erase(pair.first);
	}


}


void NetworkManagerClient::UpdateSendingInputPacket()
{
	float time = Timing::sInstance.GetTimef();

	if (time > mTimeOfLastInputPacket + kTimeBetweenInputPackets)
	{
		SendInputPacket();
		mTimeOfLastInputPacket = time;
	}
}

void NetworkManagerClient::SendInputPacket()
{
	//only send if there's any input to send!
	const MoveList& moveList = InputManager::sInstance->GetMoveList();

	if (moveList.HasMoves())
	{
		OutputMemoryBitStream inputPacket;
		inputPacket.Write(kInputCC);

		mDeliveryNotificationManager.WriteState(inputPacket);

		//we only want to send the last three moves
		int moveCount = moveList.GetMoveCount();
		int firstMoveIndex = moveCount - 3;
		if (firstMoveIndex < 3)
		{
			firstMoveIndex = 0;
		}
		auto move = moveList.begin() + firstMoveIndex;

		//only need two bits to write the move count, because it's 0, 1, 2 or 3
		inputPacket.Write(moveCount - firstMoveIndex, 2);

		for (; firstMoveIndex < moveCount; ++firstMoveIndex, ++move)
		{
			///would be nice to optimize the time stamp...
			move->Write(inputPacket);
		}

		SendPacket(inputPacket, mServerAddress);
	}
}
