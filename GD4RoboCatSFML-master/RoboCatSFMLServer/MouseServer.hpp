//Dominik Hampejs D00250604
//Gracie Chaudhary D00251769
class MouseServer : public Mouse
{
public:
	static GameObjectPtr	StaticCreate() { return NetworkManagerServer::sInstance->RegisterAndReturn(new MouseServer()); }
	void HandleDying() override;
	virtual bool HandleCollisionWithCat(RoboCat* inCat) override;

	void Read(InputMemoryBitStream& inInputStream) override;

protected:
	MouseServer();
};

