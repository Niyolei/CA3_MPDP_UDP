//Dominik Hampejs D00250604
//Gracie Chaudhary D00251769
#include "RoboCatClientPCH.hpp"
#include "CharacterAnimation.hpp"

CharacterAnimation::CharacterAnimation()
    : mNumFrames(0)
    , mCurrentFrame(0)
    , mDuration(sf::Time::Zero)
    , mElaspedTime(sf::Time::Zero)
    , mIsRepeating(true)
    , mRow(0)
{
	
}

CharacterAnimation::CharacterAnimation(CharacterAnimationState state, const sf::Texture& texture)
    :mSprite(texture)
    , mFrameSize()
    , mNumFrames(0)
    , mCurrentFrame(0)
    , mDuration(sf::Time::Zero)
    , mElaspedTime(sf::Time::Zero)
    , mIsRepeating(true)
    , mCharacterAnimationState(state)
    , mRow(0)
{

}

void CharacterAnimation::Init()
{
	// Initialize the animation properties for default walking animation
	mCharacterAnimationState = CharacterAnimationState::kWalk;
	mFrameSize = sf::Vector2i(38, 42);
   // set the centre to be in the middle of the frame not the spritesheet
	mSprite.setOrigin(mFrameSize.x / 2.f, mFrameSize.y / 2.f);
	mNumFrames = 4;
	mDuration = sf::seconds(1.f); 
	mIsRepeating = true;
	mRow = 0; 
}

void CharacterAnimation::SetTexture(TexturePtr texture)
{
	mSprite.setTexture(*texture);
	
	   
}

const sf::Texture* CharacterAnimation::GetTexture() const
{
	return mSprite.getTexture();
}

void CharacterAnimation::SetFrameSize(sf::Vector2i frameSize)
{
	mFrameSize = frameSize;
   
}

sf::Vector2i CharacterAnimation::GetFrameSize() const
{
	return mFrameSize;
}

void CharacterAnimation::SetNumFrames(std::size_t numFrames)
{
	mNumFrames = numFrames;
}

std::size_t CharacterAnimation::GetNumFrames() const
{
	return mNumFrames;
}

void CharacterAnimation::SetDuration(sf::Time duration)
{
    mDuration = duration;
}

sf::Time CharacterAnimation::GetDuration() const
{
    return sf::Time();
}

void CharacterAnimation::SetRepeating(bool flag)
{
	mIsRepeating = flag;
}

bool CharacterAnimation::IsRepeating() const
{
    return mIsRepeating;
}

void CharacterAnimation::Restart()
{
	mElaspedTime = sf::Time::Zero;
	mCurrentFrame = 0;

	//m_sprite.setTextureRect(sf::IntRect(0, 0, m_frame_size.x, m_frame_size.y));
}

bool CharacterAnimation::IsFinished() const
{
    return !mIsRepeating && mCurrentFrame >= mNumFrames;
}

void CharacterAnimation::SetRow(int row)
{
    mRow = row;
}

void CharacterAnimation::Update(sf::Time dt)
{
    if (mNumFrames == 0 || mDuration == sf::Time::Zero) return;

    mElaspedTime += dt;
    sf::Time time_per_frame = mDuration / static_cast<float>(mNumFrames);

    while (mElaspedTime >= time_per_frame)
    {
        mElaspedTime -= time_per_frame;
        if (mCurrentFrame + 1 < mNumFrames)
            ++mCurrentFrame;
        else if (mIsRepeating)
            mCurrentFrame = 0;
    }

    sf::IntRect textureRect(
        static_cast<int>(mCurrentFrame) * mFrameSize.x,
        mRow * mFrameSize.y,
        mFrameSize.x,
        mFrameSize.y
    );

    mSprite.setTextureRect(textureRect);
}

sf::IntRect CharacterAnimation::GetCurrentTextureRect() const
{
    return sf::IntRect(
        static_cast<int>(mCurrentFrame) * mFrameSize.x,
        mRow * mFrameSize.y,
        mFrameSize.x,
        mFrameSize.y
    );
}

std::size_t CharacterAnimation::GetCurrentFrame() const
{
    return mCurrentFrame;

}

sf::Sprite& CharacterAnimation::GetSprite()
{
    return mSprite;
}

void CharacterAnimation::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(mSprite, states);
}
