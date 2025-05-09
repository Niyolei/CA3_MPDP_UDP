#include "RoboCatServerPCH.hpp"


MouseServer::MouseServer()
{
}

void MouseServer::HandleDying()
{
	NetworkManagerServer::sInstance->UnregisterGameObject(this);
}


bool MouseServer::HandleCollisionWithCat(RoboCat* inCat)
{
	//kill yourself!
	SetDoesWantToDie(true);

	//SetLocation(Vector3(100,100,0));
	//NetworkManagerServer::sInstance->SetStateDirty(GetNetworkId(), EMRS_Pose);
	
	ScoreBoardManager::sInstance->IncScore(inCat->GetPlayerId(), 1);

	return false;
}






