//Dominik Hampejs D00250604
//Gracie Chaudhary D00251769
class Client : public Engine
{
public:

	static bool StaticInit();

protected:

	Client();

	virtual void	DoFrame() override;
	virtual void	HandleEvent(sf::Event& p_event) override;
	virtual bool	PollEvent(sf::Event& p_event) override;

private:

};
