//Dominik Hampejs D00250604
//Gracie Chaudhary D00251769
class YarnServer : public Yarn
{
public:
	static GameObjectPtr	StaticCreate() { return NetworkManagerServer::sInstance->RegisterAndReturn(new YarnServer()); }
	void HandleDying() override;

	virtual bool		HandleCollisionWithCat(RoboCat* inCat) override;

	virtual void Update() override;

protected:
	YarnServer();

private:
	float mTimeToDie;

};

