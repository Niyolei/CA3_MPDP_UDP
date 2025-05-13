#include "RoboCatPCH.hpp"

std::unique_ptr< ScoreBoardManager >	ScoreBoardManager::sInstance;


void ScoreBoardManager::StaticInit()
{
	sInstance.reset(new ScoreBoardManager());
}

ScoreBoardManager::ScoreBoardManager()
{
	mDefaultColors.push_back(Colors::LightYellow);
	mDefaultColors.push_back(Colors::LightBlue);
	mDefaultColors.push_back(Colors::LightPink);
	mDefaultColors.push_back(Colors::LightGreen);
}

ScoreBoardManager::Entry::Entry(uint32_t inPlayerId, const string& inPlayerName, const Vector3& inColor) :
	mPlayerId(inPlayerId),
	mPlayerName(inPlayerName),
	mColor(inColor)
{
	SetPlayerKills(0);
}

void ScoreBoardManager::Entry::SetPlayerKills(int32_t inScore)
{
	mPlayerKills = inScore;

	char	buffer[256];
	snprintf(buffer, 256, "%s %i", mPlayerName.c_str(), mPlayerKills);
	mFormattedNameKills = string(buffer);

}

void ScoreBoardManager::Entry::SetTime(float inTime)
{
	mPlayerTime = inTime;

	char	buffer[256];
	snprintf(buffer, 256, "%s %.2f", mPlayerName.c_str(), mPlayerTime);
	mFormattedNameTime = string(buffer);
}


ScoreBoardManager::Entry* ScoreBoardManager::GetEntry(uint32_t inPlayerId)
{
	for (Entry& entry : mEntries)
	{
		if (entry.GetPlayerId() == inPlayerId)
		{
			return &entry;
		}
	}

	return nullptr;
}

bool ScoreBoardManager::RemoveEntry(uint32_t inPlayerId)
{
	for (auto eIt = mEntries.begin(), endIt = mEntries.end(); eIt != endIt; ++eIt)
	{
		if ((*eIt).GetPlayerId() == inPlayerId)
		{
			mEntries.erase(eIt);
			return true;
		}
	}

	return false;
}

void ScoreBoardManager::AddEntry(uint32_t inPlayerId, const string& inPlayerName)
{
	//if this player id exists already, remove it first- it would be crazy to have two of the same id
	RemoveEntry(inPlayerId);

	mEntries.emplace_back(inPlayerId, inPlayerName, mDefaultColors[inPlayerId % mDefaultColors.size()]);
}

void ScoreBoardManager::IncKills(uint32_t inPlayerId, int inAmount)
{
	Entry* entry = GetEntry(inPlayerId);
	if (entry)
	{
		entry->SetPlayerKills(entry->GetPlayerKills() + inAmount);
	}
}

void ScoreBoardManager::SetTime(uint32_t inPlayerId, float inTime)
{
	Entry* entry = GetEntry(inPlayerId);
	if (entry)
	{
		entry->SetTime(inTime);
	}
}



bool ScoreBoardManager::Write(OutputMemoryBitStream& inOutputStream) const
{
	int entryCount = mEntries.size();

	//we don't know our player names, so it's hard to check for remaining space in the packet...
	//not really a concern now though
	inOutputStream.Write(entryCount);
	for (const Entry& entry : mEntries)
	{
		entry.Write(inOutputStream);
	}

	return true;
}



bool ScoreBoardManager::Read(InputMemoryBitStream& inInputStream)
{
	int entryCount;
	inInputStream.Read(entryCount);
	//just replace everything that's here, it don't matter...
	mEntries.resize(entryCount);
	for (Entry& entry : mEntries)
	{
		entry.Read(inInputStream);
	}

	return true;
}

void ScoreBoardManager::GetTopTimesFromFile()
{
	
	mTopTime.clear();
	std::ifstream inFile("TopTimes.txt");
	if (!inFile)
	{
		std::cerr << "Error opening file for reading TopTimes.txt " <<  std::endl;
		return;
	}

	for (int i = 0; i < 5; ++i)
	{		
		TimeEntry topTime;
		inFile >> topTime.mPlayerName >> topTime.mPlayerTime;
		mTopTime.push_back(topTime);
	}
}

void ScoreBoardManager::GetTopKillsFromFile()
{
	mTopKills.clear();
	std::ifstream inFile("TopKills.txt");
	if (!inFile)
	{
		std::cerr << "Error opening file for reading TopKills.txt " << std::endl;
		return;
	}
	for (int i = 0; i < 5; ++i)
	{
		KillEntry topKill;
		inFile >> topKill.mPlayerName >> topKill.mPlayerKills;
		mTopKills.push_back(topKill);
	}
}

void ScoreBoardManager::WriteTopTimesToFile()
{
	std::ofstream outFile("TopTimes.txt");
	if (!outFile)
	{
		std::cerr << "Error opening file for writing TopTimes.txt " << std::endl;
		return;
	}
	for (const TimeEntry& entry : mTopTime)
	{
		outFile << entry.mPlayerName << " " << entry.mPlayerTime << std::endl;
	}
	outFile.close();
}

void ScoreBoardManager::WriteTopKillsToFile()
{
	std::ofstream outFile("TopKills.txt");
	if (!outFile)
	{
		std::cerr << "Error opening file for writing TopKills.txt " << std::endl;
		return;
	}
	for (const KillEntry& entry : mTopKills)
	{
		outFile << entry.mPlayerName << " " << entry.mPlayerKills << std::endl;
	}
	outFile.close();
}

void ScoreBoardManager::CheckForNewTops()
{
	for (const Entry& entry : mEntries)
	{
		TimeEntry newTimeEntry;
		newTimeEntry.mPlayerName = entry.GetPlayerName();
		newTimeEntry.mPlayerTime = entry.GetPlayerTime();

		KillEntry newKillEntry;
		newKillEntry.mPlayerName = entry.GetPlayerName();
		newKillEntry.mPlayerKills = entry.GetPlayerKills();

		mTopKills.push_back(newKillEntry);
		mTopTime.push_back(newTimeEntry);

		std::sort(mTopKills.begin(), mTopKills.end(), [](const KillEntry& a, const KillEntry& b) {
			return a.mPlayerKills > b.mPlayerKills;
			});

		std::sort(mTopTime.begin(), mTopTime.end(), [](const TimeEntry& a, const TimeEntry& b) {
			return a.mPlayerTime > b.mPlayerTime;
			});	

		if (mTopKills.size() > 5) {
			mTopKills.pop_back();
		}

		if (mTopTime.size() > 5) {
			mTopTime.pop_back();
		}
	}

	
}

bool ScoreBoardManager::Entry::Write(OutputMemoryBitStream& inOutputStream) const
{
	bool didSucceed = true;

	inOutputStream.Write(mColor);
	inOutputStream.Write(mPlayerId);
	inOutputStream.Write(mPlayerName);
	inOutputStream.Write(mPlayerKills);
	inOutputStream.Write(mPlayerTime);

	return didSucceed;
}

bool ScoreBoardManager::Entry::Read(InputMemoryBitStream& inInputStream)
{
	bool didSucceed = true;

	inInputStream.Read(mColor);
	inInputStream.Read(mPlayerId);

	inInputStream.Read(mPlayerName);

	int score;
	inInputStream.Read(score);
	if (didSucceed)
	{
		SetPlayerKills(score);
	}

	float time;
	inInputStream.Read(time);
	if (didSucceed)
	{
		SetTime(time);
	}


	return didSucceed;
}




