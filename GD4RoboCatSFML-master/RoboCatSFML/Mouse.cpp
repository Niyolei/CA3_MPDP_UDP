#include "RoboCatPCH.hpp"

Mouse::Mouse()
{
	SetScale(GetScale() * 0.5f);
	SetCollisionRadius(20.f);
	mIsHealth = false;
}


bool Mouse::HandleCollisionWithCat(RoboCat* inCat)
{
	(void)inCat;
	return false;
}


uint32_t Mouse::Write(OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState) const
{
	uint32_t writtenState = 0;

	if (inDirtyState & EMRS_Pose)
	{
		inOutputStream.Write((bool)true);

		Vector3 location = GetLocation();
		inOutputStream.Write(location.mX);
		inOutputStream.Write(location.mY);

		writtenState |= EMRS_Pose;
	}
	else
	{
		inOutputStream.Write((bool)false);
	}

	if (inDirtyState & EMRS_Type)
	{
		inOutputStream.Write((bool)true);

		inOutputStream.Write(mIsHealth);

		writtenState |= EMRS_Type;
	}
	else
	{
		inOutputStream.Write((bool)false);
	}


	return writtenState;
}


