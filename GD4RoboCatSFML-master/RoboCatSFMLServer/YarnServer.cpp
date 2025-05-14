//Dominik Hampejs D00250604
//Gracie Chaudhary D00251769
#include "RoboCatServerPCH.hpp"


YarnServer::YarnServer()
{
	//yarn lives a second...
	mTimeToDie = Timing::sInstance.GetFrameStartTime() + 4.f;
}

void YarnServer::HandleDying()
{
	NetworkManagerServer::sInstance->UnregisterGameObject(this);
}


void YarnServer::Update()
{
	Yarn::Update();

	if (Timing::sInstance.GetFrameStartTime() > mTimeToDie)
	{
		SetDoesWantToDie(true);
	}

}

bool YarnServer::HandleCollisionWithCat(RoboCat* inCat)
{
	if (inCat->GetPlayerId() != GetPlayerId())
	{
		//kill yourself!
		SetDoesWantToDie(true);

		inCat->HandleYarnCollision(mVelocity);

		static_cast<RoboCatServer*>(inCat)->TakeDamage(GetPlayerId());

	}

	return false;
}



