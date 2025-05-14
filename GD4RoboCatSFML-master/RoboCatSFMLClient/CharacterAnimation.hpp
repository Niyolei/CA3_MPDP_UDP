//Dominik Hampejs D00250604
//Gracie Chaudhary D00251769
#pragma once
class CharacterAnimation : public sf::Drawable, public sf::Transformable
{
	public: 
		enum class CharacterAnimationState {
			kIdle,
			kWalk,
			kAttack,
			kImpact,
			kCount
		};


	public:
		CharacterAnimation();
		CharacterAnimation(CharacterAnimationState state, const sf::Texture& texture);
		

		void Init();

        void SetTexture(TexturePtr texture);
        const sf::Texture* GetTexture() const;

        void SetFrameSize(sf::Vector2i frameSize);
        sf::Vector2i GetFrameSize() const;

        void SetNumFrames(std::size_t numFrames);
        std::size_t GetNumFrames() const;

        void SetDuration(sf::Time duration);
        sf::Time GetDuration() const;

        void SetRepeating(bool flag);
        bool IsRepeating() const;

        void Restart();
        bool IsFinished() const;

        void SetRow(int row);
        void Update(sf::Time dt);

        sf::IntRect GetCurrentTextureRect() const;
        std::size_t GetCurrentFrame() const;

        sf::Sprite& GetSprite(); // for compatibility

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	sf::Sprite mSprite;
	sf::Vector2i mFrameSize;
	std::size_t mNumFrames;
	std::size_t mCurrentFrame;
	sf::Time mDuration;
	sf::Time mElaspedTime;
	bool mIsRepeating;
	int mRow;
	CharacterAnimationState mCharacterAnimationState;

};

