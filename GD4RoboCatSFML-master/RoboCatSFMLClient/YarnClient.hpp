//Dominik Hampejs D00250604
//Gracie Chaudhary D00251769
class YarnClient : public Yarn
{
public:
	static	GameObjectPtr	StaticCreate() { return GameObjectPtr(new YarnClient()); }

	virtual void		Read(InputMemoryBitStream& inInputStream) override;

protected:
	YarnClient();

private:

	SpriteComponentPtr	mSpriteComponent;
};
