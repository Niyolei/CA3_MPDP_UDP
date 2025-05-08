#include "RoboCatPCH.hpp"

RoboCat::RoboCat() :
	GameObject(),
	mMaxRotationSpeed(100.f),
	mMaxLinearSpeed(3000.f),
	mVelocity(Vector3::Zero),
	mFacingVector(Vector3::Zero),
	mAccelerationMultiplier(0.07f),
	mDecelerationMultiplier(0.05f),
	mAccelerationValue(mMaxLinearSpeed * mAccelerationMultiplier),
	mDecelerationValue(mMaxLinearSpeed * mDecelerationMultiplier),
	mVelocityCutoffValue(0.2f),
	mCurrentSpeed(0.f),
	mWallRestitution(0.1f),
	mCatRestitution(0.1f),
	mThrustDir(0.f, 0.f, 0.f),
	mPlayerId(0),
	mIsShooting(false),
	mHealth(10)
{
	SetCollisionRadius(60.f);
}

void RoboCat::UpdateFacingVector()
{
	if (mThrustDir.mX != 0 && mThrustDir.mY != 0)
	{
		Vector3 vector = mThrustDir;
		vector.Normalize2D();
		mFacingVector = vector;
	}
	else if (mThrustDir.mX != 0)
	{
		mFacingVector = Vector3(mThrustDir.mX, 0.f, 0.f);
	}
	else if (mThrustDir.mY != 0)
	{
		mFacingVector = Vector3(0.f, mThrustDir.mY, 0.f);
	}
}

void RoboCat::ProcessInput(float inDeltaTime, const InputState& inInputState)
{
	//process our input....

	////turning...
	//float newRotation = GetRotation() + inInputState.GetDesiredHorizontalDelta() * mMaxRotationSpeed * inDeltaTime;
	//SetRotation(newRotation);

	//moving...
	float inputForwardDelta = inInputState.GetDesiredVerticalDelta();
	mThrustDir.mY = inputForwardDelta * -1;

	float inputRightDelta = inInputState.GetDesiredHorizontalDelta();
	mThrustDir.mX = inputRightDelta;


	mIsShooting = inInputState.IsShooting();

	UpdateFacingVector();
}

void RoboCat::AdjustVelocityByThrust(float inDeltaTime)
{
	//just set the velocity based on the thrust direction -- no thrust will lead to 0 velocity
	//simulating acceleration makes the client prediction a bit more complex
	//Vector3 forwardVector = GetForwardVector();
	//mVelocity = forwardVector * (mThrustDir.mY * inDeltaTime * mMaxLinearSpeed);
	
	//--------------------------------------------------------------------------------Change this to Acceleration--------------------------------------
	if (mThrustDir.Length2D() != 0)
	{
		mCurrentSpeed += mAccelerationValue;
		if (mCurrentSpeed > mMaxLinearSpeed)
		{
			mCurrentSpeed = mMaxLinearSpeed;
		}

		Vector3 velocityIncrease = mFacingVector * (inDeltaTime * mAccelerationValue);

		mVelocity += velocityIncrease;
	}


	if (mVelocity.Length2D() > mVelocityCutoffValue)
	{
		Vector3 moveVector = Vector3(1 - abs(mThrustDir.mX), 1 - abs(mThrustDir.mY), 0);

		moveVector = moveVector * mVelocity;

		if (moveVector.Length2D() != 0)
		{
			moveVector.Normalize2D();
		}

		mCurrentSpeed -= mDecelerationMultiplier;
		if (mCurrentSpeed < 0)
		{
			mCurrentSpeed = 0;
		}

		Vector3 velocityDecrease = moveVector * (inDeltaTime * mDecelerationValue);
		mVelocity -= velocityDecrease;
	}
	else
	{
		mCurrentSpeed = 0;
		mVelocity = Vector3::Zero;
	}
}

void RoboCat::SimulateMovement(float inDeltaTime)
{
	//simulate us...
	AdjustVelocityByThrust(inDeltaTime);
	
	SetLocation(GetLocation() + mVelocity * inDeltaTime);

	ProcessCollisions();
}


Vector3 RoboCat::GetFacingVector() const
{
	return mFacingVector;
}

void RoboCat::Update()
{

}

void RoboCat::ProcessCollisions()
{
	//right now just bounce off the sides..
	ProcessCollisionsWithScreenWalls();

	float sourceRadius = GetCollisionRadius();
	Vector3 sourceLocation = GetLocation();

	//now let's iterate through the world and see what we hit...
	//note: since there's a small number of objects in our game, this is fine.
	//but in a real game, brute-force checking collisions against every other object is not efficient.
	//it would be preferable to use a quad tree or some other structure to minimize the
	//number of collisions that need to be tested.
	for (auto goIt = World::sInstance->GetGameObjects().begin(), end = World::sInstance->GetGameObjects().end(); goIt != end; ++goIt)
	{
		GameObject* target = goIt->get();
		if (target != this && !target->DoesWantToDie())
		{
			//simple collision test for spheres- are the radii summed less than the distance?
			Vector3 targetLocation = target->GetLocation();
			float targetRadius = target->GetCollisionRadius();

			Vector3 delta = targetLocation - sourceLocation;
			float distSq = delta.LengthSq2D();
			float collisionDist = (sourceRadius + targetRadius);
			if (distSq < (collisionDist * collisionDist))
			{
				//first, tell the other guy there was a collision with a cat, so it can do something...

				if (target->HandleCollisionWithCat(this))
				{
					//okay, you hit something!
					//so, project your location far enough that you're not colliding
					Vector3 dirToTarget = delta;
					dirToTarget.Normalize2D();
					Vector3 acceptableDeltaFromSourceToTarget = dirToTarget * collisionDist;
					//important note- we only move this cat. the other cat can take care of moving itself
					SetLocation(targetLocation - acceptableDeltaFromSourceToTarget);


					Vector3 relVel = mVelocity;

					//if other object is a cat, it might have velocity, so there might be relative velocity...
					RoboCat* targetCat = target->GetAsCat();
					if (targetCat)
					{
						relVel -= targetCat->mVelocity;
					}

					//got vel with dir between objects to figure out if they're moving towards each other
					//and if so, the magnitude of the impulse ( since they're both just balls )
					float relVelDotDir = Dot2D(relVel, dirToTarget);

					if (relVelDotDir > 0.f)
					{
						Vector3 impulse = relVelDotDir * dirToTarget;

						if (targetCat)
						{
							mVelocity -= impulse;
							mVelocity *= mCatRestitution;
						}
						else
						{
							mVelocity -= impulse * 2.f;
							mVelocity *= mWallRestitution;
						}

					}
				}
			}
		}
	}

}

void RoboCat::ProcessCollisionsWithScreenWalls()
{
	Vector3 location = GetLocation();
	float x = location.mX;
	float y = location.mY;

	float vx = mVelocity.mX;
	float vy = mVelocity.mY;

	float radius = GetCollisionRadius();

	//if the cat collides against a wall, the quick solution is to push it off
	if ((y + radius) >= kWindowSize.y && vy > 0)
	{
		mVelocity.mY = -vy * mWallRestitution;
		location.mY = kWindowSize.y - radius;
		SetLocation(location);
	}
	else if (y - radius <= 0 && vy < 0)
	{
		mVelocity.mY = -vy * mWallRestitution;
		location.mY = radius;
		SetLocation(location);
	}

	if ((x + radius) >= kWindowSize.x && vx > 0)
	{
		mVelocity.mX = -vx * mWallRestitution;
		location.mX = kWindowSize.x - radius;
		SetLocation(location);
	}
	else if (x - radius <= 0 && vx < 0)
	{
		mVelocity.mX = -vx * mWallRestitution;
		location.mX = radius;
		SetLocation(location);
	}
}

uint32_t RoboCat::Write(OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState) const
{
	uint32_t writtenState = 0;

	if (inDirtyState & ECRS_PlayerId)
	{
		inOutputStream.Write((bool)true);
		inOutputStream.Write(GetPlayerId());

		writtenState |= ECRS_PlayerId;
	}
	else
	{
		inOutputStream.Write((bool)false);
	}


	if (inDirtyState & ECRS_Pose)
	{
		inOutputStream.Write((bool)true);

		Vector3 velocity = mVelocity;
		inOutputStream.Write(velocity.mX);
		inOutputStream.Write(velocity.mY);

		Vector3 location = GetLocation();
		inOutputStream.Write(location.mX);
		inOutputStream.Write(location.mY);

		inOutputStream.Write(GetRotation());

		writtenState |= ECRS_Pose;
	}
	else
	{
		inOutputStream.Write((bool)false);
	}

	//always write mThrustDir- it's just two bits
	if (mThrustDir.mY != 0.f || mThrustDir.mX != 0.f)
	{
		inOutputStream.Write(true);
		inOutputStream.Write(mThrustDir.mX > 0.f);
		inOutputStream.Write(mThrustDir.mY > 0.f);
	}
	else
	{
		inOutputStream.Write(false);
	}

	if (inDirtyState & ECRS_Color)
	{
		inOutputStream.Write((bool)true);
		inOutputStream.Write(GetColor());

		writtenState |= ECRS_Color;
	}
	else
	{
		inOutputStream.Write((bool)false);
	}

	if (inDirtyState & ECRS_Health)
	{
		inOutputStream.Write((bool)true);
		inOutputStream.Write(mHealth, 4);

		writtenState |= ECRS_Health;
	}
	else
	{
		inOutputStream.Write((bool)false);
	}

	return writtenState;


}



