//Dominik Hampejs D00250604
//Gracie Chaudhary D00251769
#include "RoboCatClientPCH.hpp"


YarnClient::YarnClient()
{
	mSpriteComponent.reset(new SpriteComponent(this));
	mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture("snowball"));
	mSpriteComponent->GetSprite().setScale(1.f, 1.f);
}


void YarnClient::Read(InputMemoryBitStream& inInputStream)
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


	/*inInputStream.Read(stateBit);
	if (stateBit)
	{
		Vector3 color;
		inInputStream.Read(color);
		SetColor(color);
	}*/

	inInputStream.Read(stateBit);
	if (stateBit)
	{
		inInputStream.Read(mPlayerId, 8);
	}

	inInputStream.Read(stateBit);
	if (stateBit)
	{
		Vector3 velocityDir;

		inInputStream.Read(stateBit);
		if (stateBit)
		{
			inInputStream.Read(stateBit);
			velocityDir.mX = stateBit ? 1.f : -1.f;
		}
		else
		{
			velocityDir.mX = 0.f;
		}

		inInputStream.Read(stateBit);
		if (stateBit)
		{
			inInputStream.Read(stateBit);
			velocityDir.mY = stateBit ? 1.f : -1.f;
		}
		else
		{
			velocityDir.mY = 0.f;
		}

		if (velocityDir.mX != 0.f && velocityDir.mY != 0.f)
		{
			velocityDir.Normalize2D();
		}

		SetVelocity(velocityDir * mMaxSpeed);
	}



}

