//I take care of rendering things!

class HUD
{
public:
	enum class GameState
	{
		StartScreen,
		WaitingForPlayers,
		GameStarted,
		GameEnded
	};

public:

	static void StaticInit();
	static std::unique_ptr< HUD >	sInstance;

	void Render();

	void SetPlayerHealth(int inHealth) { mHealth = inHealth; }
	void SetGameState(GameState inGameState) 
	{ 
		mGameState = inGameState; 
		if (inGameState == GameState::GameStarted)
		{
		mGameStartedDisplayTime = Timing::sInstance.GetTimef(); 
		}
	}

	GameState GetGameState() const { return mGameState; }

private:

	HUD();

	void	RenderBandWidth();
	void	RenderRoundTripTime();
	void	RenderScoreBoard();
	void	RenderHealth();
	void	RenderText(const string& inStr, const Vector3& origin, const Vector3& inColor);
	void	RenderHeadingText(const string& inStr, const Vector3& origin, const Vector3& inColor);

	void	RenderStartMessage();
	void	RenderWaitMessage();
	void	RenderGameStartedMessage();

	

	Vector3										mBandwidthOrigin;
	Vector3										mRoundTripTimeOrigin;
	Vector3										mScoreBoardOrigin;
	Vector3										mScoreOffset;
	Vector3										mHealthOffset;
	int											mHealth;
	GameState									mGameState;
	float										mGameStartedDisplayTime = -1.0f;
};



