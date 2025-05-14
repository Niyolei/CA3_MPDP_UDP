//Dominik Hampejs D00250604
//Gracie Chaudhary D00251769
#include "RoboCatServerPCH.hpp"


MouseServer::MouseServer()
{
	mIsHealth = RoboMath::GetRandomFloat() > 0.7;
}

void MouseServer::HandleDying()
{
	NetworkManagerServer::sInstance->UnregisterGameObject(this);
	PickupSpawner::sInstance->ConsumePickup();
}


bool MouseServer::HandleCollisionWithCat(RoboCat* inCat)
{

	static_cast<RoboCatServer*>(inCat)->HandlePickup(mIsHealth);
	//kill yourself!
	SetDoesWantToDie(true);

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






