//Dominik Hampejs D00250604
//Gracie Chaudhary D00251769
namespace StringUtils
{
	string GetCommandLineArg(int inIndex);

	string Sprintf(const char* inFormat, ...);

	void	Log(const char* inFormat);
	void	Log(const char* inFormat, ...);
}

#define LOG( ... ) StringUtils::Log( __VA_ARGS__ );

