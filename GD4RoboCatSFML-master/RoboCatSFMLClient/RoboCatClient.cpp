#include "RoboCatClientPCH.hpp"

RoboCatClient::RoboCatClient() :
	mTimeLocationBecameOutOfSync(0.f),
	mTimeVelocityBecameOutOfSync(0.f)
{
	mSpriteComponent.reset(new PlayerSpriteComponent(this));
	mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture("character"));
	sf::IntRect rect(0, 0, 38, 42);
	mSpriteComponent->GetSprite().setTextureRect(rect);

	mPlayerUIComponent.reset(new PlayerUIComponent(this));
	RenderManager::sInstance->AddPlayerUIComponent(mPlayerUIComponent.get());	
}

void RoboCatClient::HandleDying()
{
	RenderManager::sInstance->RemovePlayerUIComponent(mPlayerUIComponent.get());
	RoboCat::HandleDying();

	//and if we're local, tell the hud so our health goes away!
	if (GetPlayerId() == NetworkManagerClient::sInstance->GetPlayerId())
	{
		HUD::sInstance->SetPlayerHealth(0);
		//delete the player UI component
		AudioManager::sInstance->Play("explosion1");
	}


}





void RoboCatClient::Update()
{
	//is this the cat owned by us?
	if (GetPlayerId() == NetworkManagerClient::sInstance->GetPlayerId())
	{
		const Move* pendingMove = InputManager::sInstance->GetAndClearPendingMove();
		//in theory, only do this if we want to sample input this frame / if there's a new move ( since we have to keep in sync with server )
		if (pendingMove) //is it time to sample a new move...
		{
			float deltaTime = pendingMove->GetDeltaTime();

			//apply that input

			ProcessInput(deltaTime, pendingMove->GetInputState());

			//and simulate!

			SimulateMovement(deltaTime);

			//LOG( "Client Move Time: %3.4f deltaTime: %3.4f left rot at %3.4f", latestMove.GetTimestamp(), deltaTime, GetRotation() );
		}
	}
	else
	{
		SimulateMovement(Timing::sInstance.GetDeltaTime());

		if (RoboMath::Is2DVectorEqual(GetVelocity(), Vector3::Zero))
		{
			//we're in sync if our velocity is 0
			mTimeLocationBecameOutOfSync = 0.f;
		}
	}
}

void RoboCatClient::Read(InputMemoryBitStream& inInputStream)
{
	bool stateBit;

	uint32_t readState = 0;

	inInputStream.Read(stateBit);
	if (stateBit)
	{
		uint32_t playerId;
		inInputStream.Read(playerId);
		SetPlayerId(playerId);
		readState |= ECRS_PlayerId;
	}

	float oldRotation = GetRotation();
	Vector3 oldLocation = GetLocation();
	Vector3 oldVelocity = GetVelocity();
	int oldHealth = mHealth;
	int oldAmmo = mAmmo;

	float replicatedRotation;
	Vector3 replicatedLocation;
	Vector3 replicatedVelocity;

	inInputStream.Read(stateBit);
	if (stateBit)
	{
		//int velocityX = 0;
		//int velocityY = 0;

		//inInputStream.Read(stateBit);
		//inInputStream.Read(velocityX, 24);
		//velocityX = stateBit ? -velocityX : velocityX;

		//inInputStream.Read(stateBit);
		//inInputStream.Read(velocityY, 24);
		//velocityY = stateBit ? -velocityY : velocityY;

		//replicatedVelocity.mX = (float)velocityX / 10000.f;
		//replicatedVelocity.mY = (float)velocityY / 10000.f;

		inInputStream.Read(replicatedVelocity.mX);
		inInputStream.Read(replicatedVelocity.mY);

		SetVelocity(replicatedVelocity);

		//int locationX = 0;
		//int locationY = 0;

		//inInputStream.Read(locationX, 18);
		//inInputStream.Read(locationY, 17);

		//replicatedLocation.mX = (float)locationX / 100.f;
		//replicatedLocation.mY = (float)locationY / 100.f;

		inInputStream.Read(replicatedLocation.mX);
		inInputStream.Read(replicatedLocation.mY);

		SetLocation(replicatedLocation);

		readState |= ECRS_Pose;
	}

	inInputStream.Read(stateBit);
	if (stateBit)
	{
		inInputStream.Read(stateBit);
		mThrustDir.mX = stateBit ? 1.f : -1.f;
	}
	else
	{
		mThrustDir.mX = 0.f;
	}

	inInputStream.Read(stateBit);
	if (stateBit)
	{
		inInputStream.Read(stateBit);
		mThrustDir.mY = stateBit ? 1.f : -1.f;
	}
	else
	{
		mThrustDir.mY = 0.f;
	}

	inInputStream.Read(stateBit);
	if (stateBit)
	{
		Vector3 color;
		inInputStream.Read(color);
		SetColor(color);
		readState |= ECRS_Color;
	}

	inInputStream.Read(stateBit);
	if (stateBit)
	{
		mHealth = 0;
		inInputStream.Read(mHealth, 2);
		readState |= ECRS_Health;
	}

	inInputStream.Read(stateBit);
	if (stateBit)
	{
		mAmmo = 0;
		inInputStream.Read(mAmmo, 3);
		readState |= ECRS_Ammo;
		
	}

	if (GetPlayerId() == NetworkManagerClient::sInstance->GetPlayerId())
	{
		//did we get health? if so, tell the hud!
		if ((readState & ECRS_Health) != 0)
		{
			HUD::sInstance->SetPlayerHealth(mHealth);

			if (mFirstReplicationReceived) {
				if (mHealth >= oldHealth)
				{
					AudioManager::sInstance->Play("healthPickup");
				}
				else if (mHealth < oldHealth)
				{
					AudioManager::sInstance->Play("snowballHitPlayer");
					//set mHit to true so we can play the hit animation
					mIsHit = true;
					
				}
			}
		}

		//did we get ammo?
		if ((readState & ECRS_Ammo) != 0)
		{
			if (mFirstReplicationReceived && mAmmo >= oldAmmo)
			{
				AudioManager::sInstance->Play("snowballPickup");
			}
		}

		DoClientSidePredictionAfterReplicationForLocalCat(readState);

		//if this is a create packet, don't interpolate
		if ((readState & ECRS_PlayerId) == 0)
		{
			InterpolateClientSidePrediction(oldRotation, oldLocation, oldVelocity, false);
		}
	}
	else
	{
		DoClientSidePredictionAfterReplicationForRemoteCat(readState);

		//will this smooth us out too? it'll interpolate us just 10% of the way there...
		if ((readState & ECRS_PlayerId) == 0)
		{
			InterpolateClientSidePrediction(oldRotation, oldLocation, oldVelocity, true);
		}

	}

	mFirstReplicationReceived = true;
}

void RoboCatClient::DoClientSidePredictionAfterReplicationForLocalCat(uint32_t inReadState)
{
	if ((inReadState & ECRS_Pose) != 0)
	{
		//simulate pose only if we received new pose- might have just gotten thrustDir
		//in which case we don't need to replay moves because we haven't warped backwards

		//all processed moves have been removed, so all that are left are unprocessed moves
		//so we must apply them...
		const MoveList& moveList = InputManager::sInstance->GetMoveList();

		for (const Move& move : moveList)
		{
			float deltaTime = move.GetDeltaTime();
			ProcessInput(deltaTime, move.GetInputState());

			SimulateMovement(deltaTime);
		}
	}
}


void RoboCatClient::InterpolateClientSidePrediction(float inOldRotation, const Vector3 & inOldLocation, const Vector3 & inOldVelocity, bool inIsForRemoteCat)
{
	if (inOldRotation != GetRotation() && !inIsForRemoteCat)
	{
		LOG("ERROR! Move replay ended with incorrect rotation!", 0);
	}

	float roundTripTime = NetworkManagerClient::sInstance->GetRoundTripTime();

	if (!RoboMath::Is2DVectorEqual(inOldLocation, GetLocation()))
	{
		//LOG( "ERROR! Move replay ended with incorrect location!", 0 );

		//have we been out of sync, or did we just become out of sync?
		float time = Timing::sInstance.GetFrameStartTime();
		if (mTimeLocationBecameOutOfSync == 0.f)
		{
			mTimeLocationBecameOutOfSync = time;
		}

		float durationOutOfSync = time - mTimeLocationBecameOutOfSync;
		if (durationOutOfSync < roundTripTime)
		{
			SetLocation(Lerp(inOldLocation, GetLocation(), inIsForRemoteCat ? (durationOutOfSync / roundTripTime) : 0.1f));
		}
	}
	else
	{
		//we're in sync
		mTimeLocationBecameOutOfSync = 0.f;
	}


	if (!RoboMath::Is2DVectorEqual(inOldVelocity, GetVelocity()))
	{
		//LOG( "ERROR! Move replay ended with incorrect velocity!", 0 );

		//have we been out of sync, or did we just become out of sync?
		float time = Timing::sInstance.GetFrameStartTime();
		if (mTimeVelocityBecameOutOfSync == 0.f)
		{
			mTimeVelocityBecameOutOfSync = time;
		}

		//now interpolate to the correct value...
		float durationOutOfSync = time - mTimeVelocityBecameOutOfSync;
		if (durationOutOfSync < roundTripTime)
		{
			SetVelocity(Lerp(inOldVelocity, GetVelocity(), inIsForRemoteCat ? (durationOutOfSync / roundTripTime) : 0.1f));
		}
		//otherwise, fine...

	}
	else
	{
		//we're in sync
		mTimeVelocityBecameOutOfSync = 0.f;
	}

}


//so what do we want to do here? need to do some kind of interpolation...

void RoboCatClient::DoClientSidePredictionAfterReplicationForRemoteCat(uint32_t inReadState)
{
	if ((inReadState & ECRS_Pose) != 0)
	{

		//simulate movement for an additional RTT
		float rtt = NetworkManagerClient::sInstance->GetRoundTripTime();
		//LOG( "Other cat came in, simulating for an extra %f", rtt );

		//let's break into framerate sized chunks though so that we don't run through walls and do crazy things...
		float deltaTime = 1.f / 30.f;

		while (true)
		{
			if (rtt < deltaTime)
			{
				SimulateMovement(rtt);
				break;
			}
			else
			{
				SimulateMovement(deltaTime);
				rtt -= deltaTime;
			}
		}
	}
}

