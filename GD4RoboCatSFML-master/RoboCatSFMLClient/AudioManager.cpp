#include "RoboCatClientPCH.hpp"
#include "AudioManager.hpp"


namespace
{
	// Sound coordinate system, point of view of a player in front of the screen:
	// X = left; Y = up; Z = back (out of the screen)
	constexpr float ListenerZ = 300.f;
	constexpr float Attenuation = 8.f;
	constexpr float MinDistance2D = 200.f;
	const float MinDistance3D = std::sqrt(MinDistance2D * MinDistance2D + ListenerZ * ListenerZ);
}

std::unique_ptr<AudioManager> AudioManager::sInstance;

void AudioManager::StaticInit()
{
	sInstance.reset(new AudioManager());
}

AudioManager::AudioManager()
    :mMusicVolume(100.0f)
{
	//old code
   /* m_sound_buffers.Load(SoundEffect::kExplosion1, "MediaFiles/Sound/ExplosionEffectV1.wav");
    m_sound_buffers.Load(SoundEffect::kExplosion2, "MediaFiles/Sound/ExplosionEffectV2.wav");

    m_sound_buffers.Load(SoundEffect::kSnowballPickup, "MediaFiles/Sound/SnowballPickupEffect.wav");
    m_sound_buffers.Load(SoundEffect::kHealthPickup, "MediaFiles/Sound/HealthPickupEffect.wav");
    m_sound_buffers.Load(SoundEffect::kButton, "MediaFiles/Sound/ButtonSoundEffect1.wav");

    m_sound_buffers.Load(SoundEffect::kSnowballHitPlayer, "MediaFiles/Sound/SnowballHitSoundEffect.ogg");
    m_sound_buffers.Load(SoundEffect::kSnowballThrow, "MediaFiles/Sound/ThrowSoundEffect.ogg");
    
    //GracieChaudhary - uploading new music
	m_filenames[MusicThemes::kMenu] = "MediaFiles/Music/MenuMusic.ogg";
	m_filenames[MusicThemes::kGame] = "MediaFiles/Music/GameplayMusic002.ogg";
	m_filenames[MusicThemes::kGameOver] = "MediaFiles/Music/GameOverMusic.wav";*/

	//sounds
    CacheSound("explosion1", "../MediaFiles/Sound/ExplosionEffectV1.wav");
	CacheSound("explosion2", "../MediaFiles/Sound/ExplosionEffectV2.wav");
	CacheSound("snowballPickup", "../MediaFiles/Sound/SnowballPickupEffect.wav");
	CacheSound("healthPickup", "../MediaFiles/Sound/HealthPickupEffect.wav");
	CacheSound("button", "../MediaFiles/Sound/ButtonSoundEffect1.wav");
	CacheSound("snowballHitPlayer", "../MediaFiles/Sound/SnowballHitSoundEffect.ogg");
	CacheSound("snowballThrow", "../MediaFiles/Sound/ThrowSoundEffect.ogg");

    //music
	CacheMusic("menu", "../MediaFiles/Music/MenuMusic.ogg");
	CacheMusic("game", "../MediaFiles/Music/GameplayMusic002.ogg");
	CacheMusic("gameOver", "../MediaFiles/Music/GameOverMusic.wav");

    sf::Listener::setDirection(0.f, 0.f, -1.f);
}

SoundPtr AudioManager::GetSound(const string& inSoundName)
{
	return mSoundMap[inSoundName];
}

MusicPtr AudioManager::GetMusic(const string& inMusicName)
{
	return mMusicMap[inMusicName];
}

string AudioManager::GetCurrentMusicName() const
{
	return mCurrentMusicName;
}

void AudioManager::Play(const string& soundName)
{
	Play(soundName, GetListenerPosition());
}

void AudioManager::Play(const string& soundName, const sf::Vector2f& inPosition)
{
	auto it = mSoundMap.find(soundName);
	if (it == mSoundMap.end() || !it->second)
	{
		LOG("AudioManager: sound '%s' not found or failed to load!", soundName.c_str());
		return;
	}

	mSounds.emplace_back(sf::Sound());
	sf::Sound& sound = mSounds.back();

	sound.setBuffer(*it->second);
	//sound.setRelativeToListener(true); 
	sound.play();
}

void AudioManager::RemoveStoppedSounds()
{
	mSounds.remove_if([](const sf::Sound& s)
		{
			return s.getStatus() == sf::Sound::Stopped;
		});
}

void AudioManager::SetListnerPosition(const sf::Vector3f& inPosition) const
{
	sf::Listener::setPosition(inPosition.x, -inPosition.y, ListenerZ);
}

sf::Vector2f AudioManager::GetListenerPosition() const
{
	sf::Vector3f position = sf::Listener::getPosition();
	return { position.x, -position.y };
}

void AudioManager::PlayMusic(const string& musicName, bool forceRestart)
{
	if (!forceRestart && mCurrentMusicName == musicName)
	{
		return;
	}

	StopMusic();

	mMusic = mMusicMap[musicName].get();
	mMusic->setVolume(mMusicVolume);
	mMusic->setLoop(true);
	mMusic->play();
	mCurrentMusicName = musicName;
}

void AudioManager::StopMusic()
{
	if (mMusic != nullptr)
	{
		mMusic->stop();
		mMusic = nullptr;
	}
	mCurrentMusicName.clear();
}

void AudioManager::PauseMusic(bool paused) const
{
	if (mMusic == nullptr)
	{
		return;
	}

	if (paused)
	{
		mMusic->pause();
	}
	else
	{
		mMusic->play();
	}
}

void AudioManager::SetMusicVolume(float inVolume)
{
	mMusicVolume = inVolume;

	if (mMusic != nullptr)
	{
		mMusic->setVolume(mMusicVolume);
	}
}



bool AudioManager::CacheSound(const string& inName, const char* inFileName)
{
	const SoundPtr newSound(new sf::SoundBuffer());
	if (!newSound->loadFromFile(inFileName))
	{
		LOG("Failed to load sound: %s", inFileName);
		return false;
	}

	mSoundMap[inName] = newSound;

	return true;
}

bool AudioManager::CacheMusic(const string& inName, const char* inFileName)
{
	const MusicPtr newMusic(new sf::Music());
	if (!newMusic->openFromFile(inFileName))
	{
		return false;
	}

	mMusicMap[inName] = newMusic;

	return true;
}
