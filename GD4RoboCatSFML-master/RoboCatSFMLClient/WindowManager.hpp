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

