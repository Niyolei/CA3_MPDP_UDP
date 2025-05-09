class MouseClient : public Mouse
{
public:
	static	GameObjectPtr	StaticCreate() { return GameObjectPtr(new MouseClient()); }

protected:
	MouseClient();

	void Read(InputMemoryBitStream& inInputStream);

private:

	SpriteComponentPtr	mSpriteComponent;

	void UpdateSprite();
};
