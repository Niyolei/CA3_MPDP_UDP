class ClientProxy
{
public:

	ClientProxy(const SocketAddress& inSocketAddress, const string& inName, int inPlayerId);

	const	SocketAddress& GetSocketAddress()	const { return mSocketAddress; }
	int				GetPlayerId()		const { return mPlayerId; }
	const	string& GetName()			const { return mName; }
	void	SetInputState(const InputState& inInputState) { mInputState = inInputState; }
	const	InputState& GetInputState()		const { return mInputState; }

	void			UpdateLastPacketTime();
	float			GetLastPacketFromClientTime()	const { return mLastPacketFromClientTime; }

	DeliveryNotificationManager& GetDeliveryNotificationManager() { return mDeliveryNotificationManager; }
	ReplicationManagerServer& GetReplicationManagerServer() { return mReplicationManagerServer; }

	const	MoveList& GetUnprocessedMoveList() const { return mUnprocessedMoveList; }
	MoveList& GetUnprocessedMoveList() { return mUnprocessedMoveList; }

	void	SetIsLastMoveTimestampDirty(bool inIsDirty) { mIsLastMoveTimestampDirty = inIsDirty; }
	bool	IsLastMoveTimestampDirty()						const { return mIsLastMoveTimestampDirty; }

	void	HandleCatDied();
	void	RespawnCatIfNecessary();

	void	SetReadyToStartGame(bool inIsReady) { mIsReadyToStartGame = inIsReady; }

	bool	IsReadyToStartGame() const { return mIsReadyToStartGame; }

	bool IsAlive() const { return mIsAlive; }

	void SetSpawnPointId(uint32_t inSpawnPointId) { mSpawnPointId = inSpawnPointId; }

	uint32_t GetSpawnPointId() const { return mSpawnPointId; }

private:

	DeliveryNotificationManager	mDeliveryNotificationManager;
	ReplicationManagerServer	mReplicationManagerServer;

	SocketAddress	mSocketAddress;
	string			mName;
	int				mPlayerId;

	InputState mInputState;

	float			mLastPacketFromClientTime;
	float			mTimeToRespawn;

	MoveList		mUnprocessedMoveList;
	bool			mIsLastMoveTimestampDirty;

	bool 		    mIsReadyToStartGame;
	bool            mIsAlive;

	uint32_t        mSpawnPointId;


};

typedef shared_ptr< ClientProxy >	ClientProxyPtr;
