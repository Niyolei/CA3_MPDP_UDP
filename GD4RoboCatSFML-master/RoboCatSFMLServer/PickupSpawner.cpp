//Dominik Hampejs D00250604
//Gracie Chaudhary D00251769
#include "RoboCatServerPCH.hpp"

std::unique_ptr<PickupSpawner> PickupSpawner::sInstance;

bool PickupSpawner::StaticInit()
{
	sInstance.reset(new PickupSpawner());
	return true;
}

void PickupSpawner::CheckForMiceSpawn()
{
	float deltaTime = Timing::sInstance.GetDeltaTime();
	int alivePlayers = NetworkManagerServer::sInstance->GetAliveCount();

	if (mSinceLastSpawnTime > mPickupSpawnInterval)
	{
		mSinceLastSpawnTime = 0.f;
		mPickupsSpawned++;
		CreateRandomMice();
	}

	mPickupSpawnInterval = mMaxPickupSpawnInterval - (alivePlayers / 4.f);

	if (mPickupsSpawned == 0)
	{
		mSinceLastSpawnTime += deltaTime;
	}

	if (mPickupsSpawned < mMaxPickups)
	{
		mSinceLastSpawnTime += deltaTime;
	}
}

void PickupSpawner::CreateRandomMice()
{
	GameObjectPtr go;
	go = GameObjectRegistry::sInstance->CreateGameObject('MOUS');


	int quadrant = RoboMath::GetRandomFloat() * 4;
	while (quadrant == mLastQuadrant) {
		quadrant = RoboMath::GetRandomFloat() * 4;
	}
	mLastQuadrant = quadrant;

	Vector3 position;

	float mid_x = kWindowSize.x / 2;
	float mid_y = kWindowSize.y / 2;

	Vector3 minOne = Vector3(kBorderDistance, kBorderDistance, 0.f);
	Vector3 maxOne = Vector3(mid_x, mid_y, 0.f);

	Vector3 minTwo = Vector3(mid_x, kBorderDistance, 0.f);
	Vector3 maxTwo = Vector3(kWindowSize.x - kBorderDistance, mid_y, 0.f);

	Vector3 minThree = Vector3(kBorderDistance, mid_y, 0.f);
	Vector3 maxThree = Vector3(mid_x, kWindowSize.y - kBorderDistance, 0.f);

	Vector3 minFour = Vector3(mid_x, mid_y, 0.f);
	Vector3 maxFour = Vector3(kWindowSize.x - kBorderDistance, kWindowSize.y - kBorderDistance, 0.f);

	switch (quadrant) {
	case 0: // Top-left quadrant
		position = RoboMath::GetRandomVector(minOne, maxOne);
		break;
	case 1: // Top-right quadrant
		position = RoboMath::GetRandomVector(minTwo, maxTwo);
		break;
	case 2: // Bottom-left quadrant
		position = RoboMath::GetRandomVector(minThree, maxThree);
		break;
	case 3: // Bottom-right quadrant
		position = RoboMath::GetRandomVector(minFour, maxFour);
		break;
	default:
		return;
	}

	go->SetLocation(position);
}

void PickupSpawner::ConsumePickup()
{
	mPickupsSpawned--;
}

PickupSpawner::PickupSpawner() 
{
}
