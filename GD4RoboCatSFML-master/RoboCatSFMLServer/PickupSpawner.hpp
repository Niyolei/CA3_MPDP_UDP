//Dominik Hampejs D00250604
//Gracie Chaudhary D00251769
#pragma once
class PickupSpawner
{
public:
	static bool StaticInit();
	static std::unique_ptr<PickupSpawner> sInstance;

	void CheckForMiceSpawn();
	void CreateRandomMice();

	void ConsumePickup();


private:

	PickupSpawner();

	int mLastQuadrant = -1;
	float mSinceLastSpawnTime = 0.f;
	float mPickupSpawnInterval = 2.f;
	float mMaxPickupSpawnInterval = 4.5f;

	int mPickupsSpawned = 0;
	int mMaxPickups = 4;
};

