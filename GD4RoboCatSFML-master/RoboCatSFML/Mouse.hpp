//Dominik Hampejs D00250604
//Gracie Chaudhary D00251769
class Mouse : public GameObject
{
public:
	CLASS_IDENTIFICATION('MOUS', GameObject)

	enum EMouseReplicationState
	{
		EMRS_Pose = 1 << 0,
		EMRS_Type = 1 << 1,

		EMRS_AllState = EMRS_Pose | EMRS_Type
	};

	static	GameObject* StaticCreate() { return new Mouse(); }

	virtual uint32_t	GetAllStateMask()	const override { return EMRS_AllState; }

	virtual uint32_t	Write(OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState) const override;

	virtual bool HandleCollisionWithCat(RoboCat* inCat) override;

protected:
	Mouse();
	bool mIsHealth;
};

