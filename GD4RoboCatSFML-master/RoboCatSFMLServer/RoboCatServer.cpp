//Dominik Hampejs D00250604
//Gracie Chaudhary D00251769
#include "RoboCatServerPCH.hpp"

RoboCatServer::RoboCatServer() :
	mCatControlType(ESCT_Human),
	mTimeOfNextShot(0.f),
	mTimeBetweenShots(1.f)
{
}

void RoboCatServer::HandleDying()
{
	NetworkManagerServer::sInstance->UnregisterGameObject(this);

	ClientProxyPtr clientProxy = NetworkManagerServer::sInstance->GetClientProxy(GetPlayerId());
	if (clientProxy)
	{
		clientProxy->HandleCatDied();			
	}
}

void RoboCatServer::Update()
{
	RoboCat::Update();

	Vector3 oldLocation = GetLocation();
	Vector3 oldVelocity = GetVelocity();

	//are you controlled by a player?
	//if so, is there a move we haven't processed yet?
	if (mCatControlType == ESCT_Human)
	{
		ClientProxyPtr client = NetworkManagerServer::sInstance->GetClientProxy(GetPlayerId());
		if (client)
		{
			MoveList& moveList = client->GetUnprocessedMoveList();
			for (const Move& unprocessedMove : moveList)
			{
				const InputState& currentState = unprocessedMove.GetInputState();

				float deltaTime = unprocessedMove.GetDeltaTime();

				ProcessInput(deltaTime, currentState);
				SimulateMovement(deltaTime);

				//LOG( "Server Move Time: %3.4f deltaTime: %3.4f left rot at %3.4f", unprocessedMove.GetTimestamp(), deltaTime, GetRotation() );

			}

			moveList.Clear();
		}
	}
	else
	{
		//do some AI stuff
		SimulateMovement(Timing::sInstance.GetDeltaTime());
	}


	HandleShooting();

	if (!RoboMath::Is2DVectorEqual(oldLocation, GetLocation()) ||
		!RoboMath::Is2DVectorEqual(oldVelocity, GetVelocity()))
	{
		NetworkManagerServer::sInstance->SetStateDirty(GetNetworkId(), ECRS_Pose);
	}
}

void RoboCatServer::HandleShooting()
{
	float time = Timing::sInstance.GetFrameStartTime();
	if (mIsShooting && Timing::sInstance.GetFrameStartTime() > mTimeOfNextShot && mAmmo > 0)
	{
		mAmmo--;
		NetworkManagerServer::sInstance->SetStateDirty(GetNetworkId(), ECRS_Ammo);

		//not exact, but okay
		mTimeOfNextShot = time + mTimeBetweenShots;

		//fire!
		YarnPtr yarn = std::static_pointer_cast<Yarn>(GameObjectRegistry::sInstance->CreateGameObject('YARN'));
		yarn->InitFromShooter(this);


	}
}

void RoboCatServer::TakeDamage(int inDamagingPlayerId)
{
	mHealth--;
	if (mHealth <= 0.f)
	{
		//score one for damaging player...
		ScoreBoardManager::sInstance->IncKills(inDamagingPlayerId, 1);

		//and you want to die
		SetDoesWantToDie(true);
	}

	//tell the world our health dropped
	NetworkManagerServer::sInstance->SetStateDirty(GetNetworkId(), ECRS_Health);
}

void RoboCatServer::HandlePickup(bool isHealthType)
{
	if (isHealthType)
	{
		Heal();
		NetworkManagerServer::sInstance->SetStateDirty(GetNetworkId(), ECRS_Health);
	}
	else
	{
		RefillAmmo();
		NetworkManagerServer::sInstance->SetStateDirty(GetNetworkId(), ECRS_Ammo);
	}

}

