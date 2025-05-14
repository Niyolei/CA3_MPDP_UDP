//Dominik Hampejs D00250604
//Gracie Chaudhary D00251769
#include "RoboCatClientPCH.hpp"

std::unique_ptr< HUD >	HUD::sInstance;


HUD::HUD() :
	mScoreBoardOrigin(50.f, 60.f, 0.0f),
	mBandwidthOrigin(50.f, 10.f, 0.0f),
	mRoundTripTimeOrigin(580.f, 10.f, 0.0f),
	mScoreOffset(0.f, 50.f, 0.0f),
	mHealthOffset(1000, 10.f, 0.0f),
	mHealth(0),
	mGameState(GameState::StartScreen)
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
	
	/*RenderScoreBoard();

	RenderStartMessage();
	RenderWaitMessage();
	RenderGameStartedMessage();*/
	//RenderHealth();

	if (ScoreBoardManager::sInstance->GetGameEnded())
	{
		SetGameState(GameState::GameEnded);
	}

	switch (mGameState)
	{
	case GameState::StartScreen:
		RenderStartMessage();
		break;
	case GameState::WaitingForPlayers:
		RenderWaitMessage();
		break;
	case GameState::GameStarted:
		if (Timing::sInstance.GetTimef() - mGameStartedDisplayTime < 1.0f)
		{
			RenderGameStartedMessage(); 
		}
		break;
	case GameState::GameEnded:
		RenderScoreBoard();
		break;
	}
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
	const float windowWidth = 1920.f;

	const float columnSpacing = 400.f; 

	// Total width across all three columns (two gaps between three columns)
	const float totalWidth = 2 * columnSpacing;

	// Start X will center the full block horizontally
	float startX = (windowWidth - totalWidth) / 2.f;

	float startY = 400.f;
	float startYCenter = 100.f;

	Vector3 leftOffset(startX-columnSpacing, startY, 0.f);
	Vector3 centerOffset(startX +0.5*columnSpacing, startYCenter, 0.f);
	Vector3 rightOffset(startX + 2.25* columnSpacing, startY, 0.f);


	RenderHeadingText("TOP TIMES", leftOffset, Colors::Black);
	RenderHeadingText("SCOREBOARD", centerOffset, Colors::Black);
	RenderHeadingText("TOP KILLS", rightOffset, Colors::Black);

	leftOffset.mY += mScoreOffset.mY + 20.f;
	centerOffset.mY += mScoreOffset.mY + 20.f;
	rightOffset.mY += mScoreOffset.mY + 20.f;

	//reducing the horizontal difference between rendering the texts inside the loop
	//leftOffset.mX -=

	const vector< ScoreBoardManager::Entry >& entries = ScoreBoardManager::sInstance->GetEntries();
	for (const auto& entry : entries)
	{		
		// Name + Score + Time
		RenderText(entry.GetPlayerName(), centerOffset ,Colors::Black);
		RenderText(StringUtils::Sprintf("%d", entry.GetPlayerKills()), centerOffset + Vector3(150.f, 0.f, 0.f), Colors::Black);
		RenderText(StringUtils::Sprintf("%.2f", entry.GetPlayerTime()), centerOffset + Vector3(250.f, 0.f, 0.f), Colors::Black);
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
		RenderText(StringUtils::Sprintf("%.2f", entry.mPlayerTime), leftOffset + Vector3(150.f, 0.f, 0.f), Colors::Black);
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

void HUD::RenderHeadingText(const string& inStr, const Vector3& origin, const Vector3& inColor)
{
	sf::Text text;
	text.setString(inStr);
	text.setFillColor(sf::Color(inColor.mX, inColor.mY, inColor.mZ, 255));
	text.setCharacterSize(60);
	text.setPosition(origin.mX, origin.mY);
	text.setFont(*FontManager::sInstance->GetFont("carlito"));
	text.setStyle(sf::Text::Bold);
	WindowManager::sInstance->draw(text);
}

void HUD::RenderStartMessage()
{
	string nameOfTheGame = "Snowballed";
	string startMessage = "Press Enter to Start";
	string startMessage2 = "Press Escape to Quit";

	RenderHeadingText(nameOfTheGame, Vector3(800.f, 300.f, 0.f), Colors::Black);
	RenderText(startMessage, Vector3(750.f, 400.f, 0.f), Colors::Black);
	RenderText(startMessage2, Vector3(742.f, 450.f, 0.f), Colors::Black);
}

void HUD::RenderWaitMessage()
{
	RenderText("Waiting for other players", Vector3(710.f, 400.f, 0.f), Colors::Black);
}

void HUD::RenderGameStartedMessage()
{
	RenderText("Game Started", Vector3(790.f, 400.f, 0.f), Colors::Black);
}

