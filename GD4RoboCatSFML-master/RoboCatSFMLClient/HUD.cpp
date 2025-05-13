#include "RoboCatClientPCH.hpp"

std::unique_ptr< HUD >	HUD::sInstance;


HUD::HUD() :
	mScoreBoardOrigin(50.f, 60.f, 0.0f),
	mBandwidthOrigin(50.f, 10.f, 0.0f),
	mRoundTripTimeOrigin(580.f, 10.f, 0.0f),
	mScoreOffset(0.f, 50.f, 0.0f),
	mHealthOffset(1000, 10.f, 0.0f),
	mHealth(0)
{
}


void HUD::StaticInit()
{
	sInstance.reset(new HUD());
}

void HUD::Render()
{
	RenderBandWidth();
	RenderRoundTripTime();
	RenderScoreBoard();
	//RenderHealth();
}

void HUD::RenderHealth()
{
	if (mHealth > 0)
	{
		string healthString = StringUtils::Sprintf("Health %d", mHealth);
		RenderText(healthString, mHealthOffset, Colors::Red);
	}
}

void HUD::RenderBandWidth()
{
	string bandwidth = StringUtils::Sprintf("In %d  Bps, Out %d Bps",
		static_cast<int>(NetworkManagerClient::sInstance->GetBytesReceivedPerSecond().GetValue()),
		static_cast<int>(NetworkManagerClient::sInstance->GetBytesSentPerSecond().GetValue()));
	RenderText(bandwidth, mBandwidthOrigin, Colors::White);
}

void HUD::RenderRoundTripTime()
{
	float rttMS = NetworkManagerClient::sInstance->GetAvgRoundTripTime().GetValue() * 1000.f;

	string roundTripTime = StringUtils::Sprintf("RTT %d ms", (int)rttMS);
	RenderText(roundTripTime, mRoundTripTimeOrigin, Colors::White);
}

void HUD::RenderScoreBoard()
{
	const vector< ScoreBoardManager::Entry >& entries = ScoreBoardManager::sInstance->GetEntries();

	Vector3 leftOffset = mScoreBoardOrigin;     
	
	Vector3 centerOffset = mScoreBoardOrigin + Vector3(300.f, 0.f, 0.f);  
	Vector3 rightOffset = mScoreBoardOrigin + Vector3(600.f, 0.f, 0.f); 

	RenderText("Top Times", leftOffset, Colors::Black);
	RenderText("All Players", centerOffset, Colors::Black);
	RenderText("Top Kills", rightOffset, Colors::Black);

	leftOffset.mY += mScoreOffset.mY;
	centerOffset.mY += mScoreOffset.mY;
	rightOffset.mY += mScoreOffset.mY;

	
	for (const auto& entry : entries)
	{		
		// Name + Score + Time
		RenderText(entry.GetPlayerName(), centerOffset,Colors::Black);
		RenderText(StringUtils::Sprintf("%d", entry.GetPlayerKills()), centerOffset + Vector3(200.f, 0.f, 0.f), Colors::Black);
		RenderText(StringUtils::Sprintf("%.2f", entry.GetPlayerTime()), centerOffset + Vector3(400.f, 0.f, 0.f), Colors::Black);
		centerOffset.mY += mScoreOffset.mY;		
	}

	const vector< ScoreBoardManager::KillEntry >& kills = ScoreBoardManager::sInstance->GetTopKills();
	for (const auto& entry : kills)
	{
		RenderText(entry.mPlayerName, rightOffset, Colors::Black);
		RenderText(StringUtils::Sprintf("%d", entry.mPlayerKills), rightOffset + Vector3(200.f, 0.f, 0.f), Colors::Black);
		rightOffset.mY += mScoreOffset.mY;
	}

	const vector< ScoreBoardManager::TimeEntry >& times = ScoreBoardManager::sInstance->GetTopTimes();
	for (const auto& entry : times)
	{
		RenderText(entry.mPlayerName, leftOffset, Colors::Black);
		RenderText(StringUtils::Sprintf("%.2f", entry.mPlayerTime), leftOffset + Vector3(200.f, 0.f, 0.f), Colors::Black);
		leftOffset.mY += mScoreOffset.mY;
	}
}

void HUD::RenderText(const string& inStr, const Vector3& origin, const Vector3& inColor)
{
	sf::Text text;
	text.setString(inStr);
	text.setFillColor(sf::Color(inColor.mX, inColor.mY, inColor.mZ, 255));
	text.setCharacterSize(50);
	text.setPosition(origin.mX, origin.mY);
	text.setFont(*FontManager::sInstance->GetFont("carlito"));
	WindowManager::sInstance->draw(text);
}

