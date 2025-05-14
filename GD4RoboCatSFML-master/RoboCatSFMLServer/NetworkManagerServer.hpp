class NetworkManagerServer : public NetworkManager
{
public:
	static NetworkManagerServer* sInstance;

	static bool				StaticInit(uint16_t inPort);

	virtual void			ProcessPacket(InputMemoryBitStream& inInputStream, const SocketAddress& inFromAddress) override;
	virtual void			HandleConnectionReset(const SocketAddress& inFromAddress) override;

	void			SendOutgoingPackets();
	void			CheckForDisconnects();

	void			RegisterGameObject(GameObjectPtr inGameObject);
	inline	GameObjectPtr	RegisterAndReturn(GameObject* inGameObject);
	void			UnregisterGameObject(GameObject* inGameObject);
	void			SetStateDirty(int inNetworkId, uint32_t inDirtyState);

	void			RespawnCats();

	ClientProxyPtr	GetClientProxy(int inPlayerId) const;

	const bool            HasGameStarted();

	void            ShouldGameEnd();

	const bool         HasGameEnded() const { return mGameEnded; }
  
	const int			GetAliveCount() const;
  
	float         GetGameStartTime() { return mGameStartTime; }

	bool            mEndExecuted;

	

private:
	NetworkManagerServer();

	void	HandlePacketFromNewClient(InputMemoryBitStream& inInputStream, const SocketAddress& inFromAddress);
	void	ProcessPacket(ClientProxyPtr inClientProxy, InputMemoryBitStream& inInputStream);

	void	SendWelcomePacket(ClientProxyPtr inClientProxy);
	void	UpdateAllClients();

	void	AddWorldStateToPacket(OutputMemoryBitStream& inOutputStream);
	void	AddScoreBoardStateToPacket(OutputMemoryBitStream& inOutputStream);

	void	SendStatePacketToClient(ClientProxyPtr inClientProxy);
	void	WriteLastMoveTimestampIfDirty(OutputMemoryBitStream& inOutputStream, ClientProxyPtr inClientProxy);

	void	HandleInputPacket(ClientProxyPtr inClientProxy, InputMemoryBitStream& inInputStream);

	void	HandleClientDisconnected(ClientProxyPtr inClientProxy);

	void    CheckEveryoneIsReady();

	int		GetNewNetworkId();

	typedef unordered_map< int, ClientProxyPtr >	IntToClientMap;
	typedef unordered_map< SocketAddress, ClientProxyPtr >	AddressToClientMap;

	AddressToClientMap		mAddressToClientMap;
	IntToClientMap			mPlayerIdToClientMap;

	int				mNewPlayerId;
	int				mNewNetworkId;

	float			mTimeOfLastSatePacket;
	float			mTimeBetweenStatePackets;
	float			mClientDisconnectTimeout;

	bool			mGameStarted;
	bool            mGameEnded;
	float           mGameStartTime;


	std::deque< uint32_t> mAvailableSpwawnPoints;
};


inline GameObjectPtr NetworkManagerServer::RegisterAndReturn(GameObject* inGameObject)
{
	GameObjectPtr toRet(inGameObject);
	RegisterGameObject(toRet);
	return toRet;
}


