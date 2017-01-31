#pragma once

#include <SFML\Config.hpp>

namespace Animations
{
	enum Entities : sf::Uint16
	{
		Bomber,
		Bomb,
		Block
	};

	enum Bomber : sf::Uint16
	{
		Idle,
		Left,
		Up,
		Right,
		Down,
		Spawn,
		Death
	};

	enum Bomb : sf::Uint16
	{
		Freeze,
		Tick
	};

	enum Block : sf::Uint16
	{
		Empty,
		Static,
		CenterExplosion,
		LeftExplosion,
		RightExplosion,
		UpExplosion,
		DownExplosion,
		VerticalExplosion,
		HorizontalExplosion,
		Destruction,
		FirePowerup,
		MegaFirePowerup,
		BombPowerup,
		SpeedPowerup
	};

	struct Animation
	{
		Animation() = default;
		Animation(sf::Uint16 startFrame, sf::Uint16 animationType)
			:
			selectedFrame(startFrame),
			animType(animationType),
			timeLeft(0)
		{

		}
		virtual ~Animation() = default;

		void animateByType(sf::Uint16 newAnimType, sf::Uint16 duration)
		{
			if (animType != newAnimType) {
				timeLeft = duration;
				animType = newAnimType;
			}
		}

		template <typename T>
		void doAnimation(sf::Uint16 lastFrame, sf::Uint16 resetState, const T & framesContainer, sf::Uint16 duration)
		{
			if (!timeLeft) {
				animType = resetState;
				selectedFrame = lastFrame;
			}
			else {
				selectedFrame = framesContainer[(duration - timeLeft) / (duration / framesContainer.size())];
				--timeLeft;
			}
		}

		virtual void animate() = 0;

		sf::Uint16 animType;
		sf::Uint16 selectedFrame;
		sf::Uint16 timeLeft;
	};

	struct BomberAnim : public Animation
	{
		BomberAnim()
			:
			Animation(4, Animations::Bomber::Idle),
			dyingDuration(56),
			movementDuration(12),
			spawningDuration(27)
		{

		}

		virtual void animate();

		bool spawnFlag;
		int dyingDuration;
		int movementDuration;
		int spawningDuration;
	};

	struct BombAnim : public Animation
	{
		BombAnim()
			:
			Animation(0, Animations::Bomb::Freeze)
		{

		}

		virtual void animate();
	};

	struct BlockAnim : public Animation
	{
		BlockAnim() { }

		BlockAnim(sf::Uint16 startFrame) : 
			Animation(startFrame, Animations::Block::Empty),
			explosionDuration(21),
			destructionDuration(20),
			powerupDuration(108)
		{
			
		}

		virtual void animate();

		int destructionDuration;
		int explosionDuration;
		int powerupDuration;
	};
}