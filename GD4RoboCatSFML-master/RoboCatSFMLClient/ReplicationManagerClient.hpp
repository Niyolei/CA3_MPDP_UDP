//Dominik Hampejs D00250604
//Gracie Chaudhary D00251769
class ReplicationManagerClient
{
public:
	void Read(InputMemoryBitStream& inInputStream);

private:

	void ReadAndDoCreateAction(InputMemoryBitStream& inInputStream, int inNetworkId);
	void ReadAndDoUpdateAction(InputMemoryBitStream& inInputStream, int inNetworkId);
	void ReadAndDoDestroyAction(InputMemoryBitStream& inInputStream, int inNetworkId);

};

