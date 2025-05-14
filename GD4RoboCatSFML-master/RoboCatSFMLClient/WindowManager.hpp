//Dominik Hampejs D00250604
//Gracie Chaudhary D00251769
class WindowManager
{

public:

	static bool StaticInit();

	void CloseWindow()
	{
		sInstance->close();
	}
	static std::unique_ptr< sf::RenderWindow >	sInstance;

};

