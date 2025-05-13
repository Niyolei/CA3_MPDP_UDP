class ScoreBoardManager
{
public:

	static void StaticInit();
	static std::unique_ptr< ScoreBoardManager >	sInstance;

	class Entry
	{
	public:
		Entry() {};

		Entry(uint32_t inPlayerID, const string& inPlayerName, const Vector3& inColor);

		const Vector3& GetColor()		const { return mColor; }
		uint32_t		GetPlayerId()	const { return mPlayerId; }
		const string& GetPlayerName()	const { return mPlayerName; }
		const string& GetFormattedNameScore()	const { return mFormattedNameKills; }
		const string& GetFormattedNameTime()	const { return mFormattedNameTime; }
		int				GetPlayerKills()		const { return mPlayerKills; }
		float			GetPlayerTime()		const { return mPlayerTime; }

		void			SetPlayerKills(int inScore);
		void 			SetTime(float inTime); 

		bool			Write(OutputMemoryBitStream& inOutputStream) const;
		bool			Read(InputMemoryBitStream& inInputStream);
		static uint32_t	GetSerializedSize();
	private:
		Vector3			mColor;

		uint32_t		mPlayerId;
		string			mPlayerName;

		int				mPlayerKills;
		float			mPlayerTime;


		string			mFormattedNameKills;
		string			mFormattedNameTime;
	};

	struct TimeEntry
	{
		string			mPlayerName;
		float			mPlayerTime;
	};

	struct KillEntry
	{
		string			mPlayerName;
		int				mPlayerKills;
	};

	Entry*	GetEntry(uint32_t inPlayerId);
	bool	RemoveEntry(uint32_t inPlayerId);
	void	AddEntry(uint32_t inPlayerId, const string& inPlayerName);
	void	IncKills(uint32_t inPlayerId, int inAmount);
	void	SetTime(uint32_t inPlayerId, float inTime);

	bool	Write(OutputMemoryBitStream& inOutputStream) const;
	bool	Read(InputMemoryBitStream& inInputStream);
		
	//reading from the top kill and time file
	void GetTopTimesFromFile();
	void GetTopKillsFromFile();

	//writing to the top kill and time file
	void WriteTopTimesToFile();
	void WriteTopKillsToFile();

	void CheckForNewTops();

	void GenerateLeaderboard();

	const vector< Entry >& GetEntries()	const { return mEntries; }

private:

	ScoreBoardManager();

	vector< Entry >	mEntries;

	//for persistence storing data - top kills and top time
	//to store name and kills
	vector<KillEntry>	mTopKills;
	//to store name and time
	vector<TimeEntry>	mTopTime;

	vector< Vector3 >	mDefaultColors;
};

