#include "RoboCatPCH.hpp"

RoboCat::RoboCat() :
	GameObject(),
	mMaxLinearSpeed(400.f),
	mVelocity(Vector3::Zero),
	mFacingVector(Vector3::Zero),
	mAccelerationMultiplier(0.35f),
	mDecelerationMultiplier(0.22f),
	mAccelerationValue(mMaxLinearSpeed * mAccelerationMultiplier),
	mDecelerationValue(mMaxLinearSpeed * mDecelerationMultiplier),
	mVelocityCutoffValue(0.8f),
	mWallRestitution(0.1f),
	mCatRestitution(0.8f),
	mYarnRestitution(0.2f),
	mThrustDir(0.f, 0.f, 0.f),
	mPlayerId(0),
	mIsShooting(false),
	mIsHit(false),
	mMaxHealth(3),
	mMaxAmmo(5),
	mHealth(mMaxHealth),
	mAmmo(mMaxAmmo)
{
	SetCollisionRadius(15.f);
	SetUpSpawnPointMap();
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

void RoboCat::Heal() {
	mHealth = std::min(mHealth + 1, mMaxHealth);
}

void RoboCat::RefillAmmo() {
	mAmmo = mMaxAmmo;
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

	//UpdateFacingVector();
}

void RoboCat::AdjustVelocityByThrust(float inDeltaTime)
{
	//just set the velocity based on the thrust direction -- no thrust will lead to 0 velocity
	//simulating acceleration makes the client prediction a bit more complex
	//Vector3 forwardVector = GetForwardVector();
	//mVelocity = forwardVector * (mThrustDir.mY * inDeltaTime * mMaxLinearSpeed);
	
	//--------------------------------------------------------------------------------Change this to Acceleration--------------------------------------
	if (mThrustDir.mX != 0 || mThrustDir.mY != 0)
	{

		Vector3 velocityIncrease = mFacingVector * (inDeltaTime * mAccelerationValue);

		mVelocity += velocityIncrease;
	}


	if (abs(mVelocity.mX) > mVelocityCutoffValue || abs(mVelocity.mY) > mVelocityCutoffValue)
	{
		Vector3 moveVector = Vector3(1 - abs(mThrustDir.mX), 1 - abs(mThrustDir.mY), 0);

		moveVector = moveVector * mVelocity;

		if (moveVector.Length2D() != 0)
		{
			moveVector.Normalize2D();
		}

		Vector3 velocityDecrease = moveVector * (inDeltaTime * mDecelerationValue);
		mVelocity -= velocityDecrease;

		if (abs(mVelocity.mX) < mVelocityCutoffValue)
		{
			mVelocity.mX = 0;
		}
		if (abs(mVelocity.mY) < mVelocityCutoffValue)
		{
			mVelocity.mY = 0;
		}
	}
	else if (mThrustDir.mX == 0 && mThrustDir.mY == 0)
	{
		mVelocity = Vector3::Zero;
	}
}

void RoboCat::SetUpSpawnPointMap()
{
	float offset_x = 100.f;
	float offset_y = 100.f;

	for (uint32_t i = 0; i < kMaxPlayers; i++)
	{
		mSpawnPointMap[i] = Vector3();

		if ((i / 4) % 2 == 0)
		{
			switch (i % 4)
			{
			case 0:
				mSpawnPointMap[i].mX = offset_x;
				mSpawnPointMap[i].mY = offset_y;
				break;
			case 1:
				mSpawnPointMap[i].mX = kWindowSize.x - offset_x;
				mSpawnPointMap[i].mY = kWindowSize.y - offset_y;
				break;
			case 2:
				mSpawnPointMap[i].mX = offset_x;
				mSpawnPointMap[i].mY = kWindowSize.y - offset_y;
				break;
			case 3:
				mSpawnPointMap[i].mX = kWindowSize.x - offset_x;
				mSpawnPointMap[i].mY = offset_y;
				offset_x += 150.f;
				offset_y += 100.f;
				break;
			default:
				break;
			}
		}
		else
		{
			switch (i % 4)
			{
			case 0:
				mSpawnPointMap[i].mX = kWindowSize.x / 2.f;
				mSpawnPointMap[i].mY = offset_y;
				break;
			case 1:
				mSpawnPointMap[i].mX = kWindowSize.x - offset_x;
				mSpawnPointMap[i].mY = kWindowSize.y / 2.f;
				break;
			case 2:
				mSpawnPointMap[i].mX = kWindowSize.x / 2.f;
				mSpawnPointMap[i].mY = kWindowSize.y - offset_y;
				break;
			case 3:
				mSpawnPointMap[i].mX = offset_x;
				mSpawnPointMap[i].mY = kWindowSize.y / 2.f;
				offset_x += 150.f;
				offset_y += 100.f;
				break;
			default:
				break;
			}
		}
	}
}

void RoboCat::SimulateMovement(float inDeltaTime)
{
	UpdateFacingVector();
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

void RoboCat::SpawnAt(uint32_t spawnPointId)
{
	SetLocation(mSpawnPointMap[spawnPointId]);
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
							mVelocity = impulse * -0.7f;
							mVelocity *= mCatRestitution;

							targetCat->mVelocity = impulse * 0.3f;
							targetCat->mVelocity *= targetCat->mCatRestitution;

						}
						else
						{
							mVelocity -= impulse;
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
	if ((y + radius) >= kWindowSize.y - kBorderDistance && vy > 0)
	{
		mVelocity.mY = -vy * mWallRestitution;
		location.mY = kWindowSize.y - kBorderDistance - radius;
		SetLocation(location);
	}
	else if (y - radius <= kBorderDistance && vy < 0)
	{
		mVelocity.mY = -vy * mWallRestitution;
		location.mY = kBorderDistance + radius;
		SetLocation(location);
	}

	if ((x + radius) >= kWindowSize.x - kBorderDistance && vx > 0)
	{
		mVelocity.mX = -vx * mWallRestitution;
		location.mX = kWindowSize.x - kBorderDistance - radius;
		SetLocation(location);
	}
	else if (x - radius <= kBorderDistance && vx < 0)
	{
		mVelocity.mX = -vx * mWallRestitution;
		location.mX = kBorderDistance + radius;
		SetLocation(location);
	}
}

void RoboCat::HandleYarnCollision(Vector3 yarnVelocity) {
	//This defyes physics however the yarn should stop the cat for gameplay reasons
	mVelocity = yarnVelocity * mYarnRestitution;
	mIsHit = true;
}

uint32_t RoboCat::Write(OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState) const
{
	uint32_t writtenState = 0;

	if (inDirtyState & ECRS_PlayerId)
	{
		inOutputStream.Write((bool)true);
		inOutputStream.Write(GetPlayerId(),8);

		writtenState |= ECRS_PlayerId;
	}
	else
	{
		inOutputStream.Write((bool)false);
	}


	if (inDirtyState & ECRS_Pose)
	{
		inOutputStream.Write((bool)true);


		//Save bandwidth by converting to int
		//7000000 in binary ≈ 24 bits
		Vector3 velocity = mVelocity;

		//inOutputStream.Write(velocity.mX < 0.f);
		//int velocityX = abs((int)(velocity.mX * 10000.f));
		//inOutputStream.Write(velocityX, 24);

		//inOutputStream.Write(velocity.mY < 0.f);
		//int velocityY = abs((int)(velocity.mY * 10000.f));
		//inOutputStream.Write(velocityY, 24);

		inOutputStream.Write(velocity.mX);
		inOutputStream.Write(velocity.mY);


		//Save bandwidth by converting to int
		//192000 in binary ≈ 18 bits
		//108000 in binary ≈ 17 bits

		Vector3 location = GetLocation();

		int locationX = (int)(location.mX * 100.f);
		int locationY = (int)(location.mY * 100.f);

		inOutputStream.Write(locationX, 18);
		inOutputStream.Write(locationY, 17);

		//inOutputStream.Write(location.mX);
		//inOutputStream.Write(location.mY);

		writtenState |= ECRS_Pose;
	}
	else
	{
		inOutputStream.Write((bool)false);
	}

	if (mThrustDir.mX != 0.f) {
		inOutputStream.Write(true);
		inOutputStream.Write(mThrustDir.mX > 0.f);
	}
	else
	{
		inOutputStream.Write(false);
	}

	//always write mThrustDir- it's just two bits
	if (mThrustDir.mY != 0.f)
	{
		inOutputStream.Write(true);
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
		inOutputStream.Write(mHealth, 2);

		writtenState |= ECRS_Health;
	}
	else
	{
		inOutputStream.Write((bool)false);
	}

	if (inDirtyState & ECRS_Ammo)
	{
		inOutputStream.Write((bool)true);
		inOutputStream.Write(mAmmo, 3);
		writtenState |= ECRS_Ammo;
	}
	else
	{
		inOutputStream.Write((bool)false);
	}

	return writtenState;


}

int RoboCat::GetHealth() const
{
	return mHealth;
}

int RoboCat::GetAmmo() const
{
	return mAmmo;
}

int RoboCat::GetMaxHealth() const
{
	return mMaxHealth;
}

int RoboCat::GetMaxAmmo() const
{
	return mMaxAmmo;
}

void RoboCat::SetIsShooting(bool inIsShooting)
{
	mIsShooting = inIsShooting;
}

bool RoboCat::IsShooting() const
{
	return mIsShooting;
}

void RoboCat::SetIsHit(bool inIsHit)
{
	mIsHit = inIsHit;
}

bool RoboCat::IsHit() const
{
	return mIsHit;
}



