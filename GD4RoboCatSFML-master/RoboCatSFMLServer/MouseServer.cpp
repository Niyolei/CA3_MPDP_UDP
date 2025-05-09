#include "RoboCatServerPCH.hpp"


MouseServer::MouseServer()
{
	mIsHealth = RoboMath::GetRandomFloat() > 0.7;
}

void MouseServer::HandleDying()
{
	NetworkManagerServer::sInstance->UnregisterGameObject(this);
}


bool MouseServer::HandleCollisionWithCat(RoboCat* inCat)
{

	static_cast<RoboCatServer*>(inCat)->HandlePickup(mIsHealth);
	//kill yourself!
	SetDoesWantToDie(true);
	
	ScoreBoardManager::sInstance->IncScore(inCat->GetPlayerId(), 1);

	return false;
}

void MouseServer::Read(InputMemoryBitStream& inInputStream)
{
	bool stateBit;

	inInputStream.Read(stateBit);
	if (stateBit)
	{
		Vector3 location;
		inInputStream.Read(location.mX);
		inInputStream.Read(location.mY);
		SetLocation(location);
	}

	inInputStream.Read(stateBit);
	if (stateBit)
	{
		bool isHealthType;
		inInputStream.Read(isHealthType);
		mIsHealth = isHealthType;
	}
}






