//Dominik Hampejs D00250604
//Gracie Chaudhary D00251769
class Yarn : public GameObject
{
public:

	CLASS_IDENTIFICATION('YARN', GameObject)

		enum EYarnReplicationState
	{
		EYRS_Pose = 1 << 0,
		//EYRS_Color = 1 << 1,
		EYRS_PlayerId = 1 << 1,
		EYRS_Velo = 1 << 2,

		EYRS_AllState = EYRS_Pose | EYRS_PlayerId | EYRS_Velo
	};

	static	GameObject* StaticCreate() { return new Yarn(); }

	virtual uint32_t	GetAllStateMask()	const override { return EYRS_AllState; }

	virtual uint32_t	Write(OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState) const override;

	void			SetVelocity(const Vector3& inVelocity) { mVelocity = inVelocity; }
	const Vector3& GetVelocity() const { return mVelocity; }

	void		SetPlayerId(int inPlayerId) { mPlayerId = inPlayerId; }
	int			GetPlayerId() const { return mPlayerId; }

	void		InitFromShooter(RoboCat* inShooter);

	virtual void Update() override;

	virtual bool HandleCollisionWithCat(RoboCat* inCat) override;

protected:
	Yarn();
	Vector3		mVelocity;
	float		mMaxSpeed;
	int			mPlayerId;

};

typedef shared_ptr< Yarn >	YarnPtr;

