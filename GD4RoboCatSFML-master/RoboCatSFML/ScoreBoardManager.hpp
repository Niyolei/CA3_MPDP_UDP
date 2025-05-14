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
		const string& GetFormattedNameKills()	const { return mFormattedNameKills; }
		const string& GetFormattedNameTime()	const { return mFormattedNameTime; }
		const string& GetFormattedNameTimeKill() const { return mFormattedNameTimeKill; }
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
		string			mFormattedNameTimeKill;
	};

	struct TimeEntry
	{
		TimeEntry():mPlayerName(""), mPlayerTime(0.f) {};

		string			mPlayerName;
		float			mPlayerTime;

		bool			Write(OutputMemoryBitStream& inOutputStream) const;
		bool			Read(InputMemoryBitStream& inInputStream);

		

	};

	struct KillEntry
	{
		KillEntry() :mPlayerName(""), mPlayerKills(0) {};

		string			mPlayerName;
		int				mPlayerKills;

		bool			Write(OutputMemoryBitStream& inOutputStream) const;
		bool			Read(InputMemoryBitStream& inInputStream);
	};

	Entry*	GetEntry(uint32_t inPlayerId);
	bool	RemoveEntry(uint32_t inPlayerId);
	void	AddEntry(uint32_t inPlayerId, const string& inPlayerName);
	void	IncKills(uint32_t inPlayerId, int inAmount);
	void	SetTime(uint32_t inPlayerId, float inTime);

	bool	Write(OutputMemoryBitStream& inOutputStream) const;
	bool	Read(InputMemoryBitStream& inInputStream);
		
	//reading from the top kill and time file
	void ReadTopsFromFile();

	//writing to the top kill and time file
	void WriteTopTimesToFile();
	void WriteTopKillsToFile();

	void CheckForNewTops();

	

	const vector< Entry >& GetEntries()	const { return mEntries; }

public:
	bool mGameEnded = false;
	const vector< KillEntry >& GetTopKills() const { return mTopKills; }
	const vector< TimeEntry >& GetTopTimes() const { return mTopTimes; }

	bool GetGameEnded() const { return mGameEnded; }

private:
	void GetTopTimesFromFile();
	void GetTopKillsFromFile();

	ScoreBoardManager();

	vector< Entry >	mEntries;

	//for persistence storing data - top kills and top time
	//to store name and kills
	vector<KillEntry>	mTopKills;
	//to store name and time
	vector<TimeEntry>	mTopTimes;

	vector< Vector3 >	mDefaultColors;
};

