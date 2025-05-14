#include "RoboCatPCH.hpp"

Yarn::Yarn() :
	mMaxSpeed(600.f),
	mVelocity(Vector3::Zero),
	mPlayerId(0)
{
	SetScale(GetScale() * 0.25f);
	SetCollisionRadius(20.f);
}


// 32 + 32 + 4 + 8 + 3-> 79 bits per snowball * 4 snowballs = 316 bits
uint32_t Yarn::Write(OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState) const
{
	uint32_t writtenState = 0;

	if (inDirtyState & EYRS_Pose)
	{
		inOutputStream.Write((bool)true);

		Vector3 location = GetLocation();
		inOutputStream.Write(location.mX);
		inOutputStream.Write(location.mY);

		writtenState |= EYRS_Pose;
	}
	else
	{
		inOutputStream.Write((bool)false);
	}

	//if (inDirtyState & EYRS_Color)
	//{
	//	inOutputStream.Write((bool)true);

	//	inOutputStream.Write(GetColor());

	//	writtenState |= EYRS_Color;
	//}
	//else
	//{
	//	inOutputStream.Write((bool)false);
	//}

	if (inDirtyState & EYRS_PlayerId)
	{
		inOutputStream.Write((bool)true);

		inOutputStream.Write(mPlayerId, 8);

		writtenState |= EYRS_PlayerId;
	}
	else
	{
		inOutputStream.Write((bool)false);
	}

	if (inDirtyState & EYRS_Velo)
	{
		inOutputStream.Write((bool)true);

		Vector3 velocity = GetVelocity();
		//inOutputStream.Write(velocity.mX);
		//inOutputStream.Write(velocity.mY);

		if (velocity.mX != 0.f)
		{
			inOutputStream.Write((bool)true);
			inOutputStream.Write(velocity.mX > 0.f);
		}
		else
		{
			inOutputStream.Write((bool)false);
		}

		if (velocity.mY != 0.f)
		{
			inOutputStream.Write((bool)true);
			inOutputStream.Write(velocity.mY > 0.f);
		}
		else
		{
			inOutputStream.Write((bool)false);
		}

		writtenState |= EYRS_Velo;
	}
	else
	{
		inOutputStream.Write((bool)false);
	}


	return writtenState;
}



bool Yarn::HandleCollisionWithCat(RoboCat* inCat)
{
	(void)inCat;

	//you hit a cat, so look like you hit a cat
	return false;
}


void Yarn::InitFromShooter(RoboCat* inShooter)
{
	SetColor(inShooter->GetColor());
	SetPlayerId(inShooter->GetPlayerId());

	/*Vector3 forward = inShooter->GetForwardVector();
	SetVelocity(inShooter->GetVelocity() + forward * mMuzzleSpeed);*/

	SetVelocity(inShooter->GetFacingVector() * mMaxSpeed);
	SetLocation(inShooter->GetLocation() /* + forward * 0.55f */);

	SetRotation(inShooter->GetRotation());
}

void Yarn::Update()
{

	float deltaTime = Timing::sInstance.GetDeltaTime();

	SetLocation(GetLocation() + mVelocity * deltaTime);


	//we'll let the cats handle the collisions
}

