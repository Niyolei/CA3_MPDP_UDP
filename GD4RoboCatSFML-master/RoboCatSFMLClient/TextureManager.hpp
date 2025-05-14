//Dominik Hampejs D00250604
//Gracie Chaudhary D00251769
class TextureManager
{
public:
	static void StaticInit();

	static std::unique_ptr<TextureManager>		sInstance;

	TexturePtr	GetTexture(const string& inTextureName);

private:
	TextureManager();

	bool CacheTexture(string inName, const char* inFileName);

	unordered_map< string, TexturePtr >	mNameToTextureMap;
};

