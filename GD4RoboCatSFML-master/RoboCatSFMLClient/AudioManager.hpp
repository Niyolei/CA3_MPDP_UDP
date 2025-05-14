//Dominik Hampejs D00250604
//Gracie Chaudhary D00251769
#pragma once

typedef shared_ptr<sf::SoundBuffer> SoundPtr;
typedef shared_ptr<sf::Music> MusicPtr;


class AudioManager
{

public:
	static void StaticInit();
	static std::unique_ptr<AudioManager> sInstance;

	SoundPtr GetSound(const string& inSoundName);
	MusicPtr GetMusic(const string& inMusicName);
	string GetCurrentMusicName() const;

	void Play(const string& soundName);
	void Play(const string& soundName, const sf::Vector2f& inPosition);
	void RemoveStoppedSounds();
	void SetListnerPosition(const sf::Vector3f& inPosition) const;
	sf::Vector2f GetListenerPosition() const;

	void PlayMusic(const string& musicName, bool forceRestart = false);
	void StopMusic();
	void PauseMusic(bool paused) const;
	void SetMusicVolume(float inVolume);


private:
	AudioManager();
	bool CacheSound(const string& inName, const char* inFileName);
	bool CacheMusic(const string& inName, const char* inFileName);

	unordered_map<string, SoundPtr> mSoundMap;
	unordered_map<string, MusicPtr> mMusicMap;
	std::list<sf::Sound> mSounds;

	std::string mCurrentMusicName;
	sf::Music* mMusic{};
	float mMusicVolume;
};

