#include "Animations.h"

#include "ResourceManager.h"

void Animations::BomberAnim::animate()
{
	if (animType == Animations::Bomber::Spawn)
		doAnimation(4, Bomber::Idle, ResourceManager::get().getAnimation(Entities::Bomber, animType), spawningDuration);
	else if (animType == Animations::Bomber::Death)
		doAnimation(4, Bomber::Idle, ResourceManager::get().getAnimation(Entities::Bomber, animType), dyingDuration);
	else if (animType == Animations::Bomber::Down)
		doAnimation(4, Bomber::Idle, ResourceManager::get().getAnimation(Entities::Bomber, animType), movementDuration);
	else if (animType == Animations::Bomber::Up)
		doAnimation(7, Bomber::Idle, ResourceManager::get().getAnimation(Entities::Bomber, animType), movementDuration);
	else if (animType == Animations::Bomber::Right)
		doAnimation(10, Bomber::Idle, ResourceManager::get().getAnimation(Entities::Bomber, animType), movementDuration);
	else if (animType == Animations::Bomber::Left)
		doAnimation(1, Bomber::Idle, ResourceManager::get().getAnimation(Entities::Bomber, animType), movementDuration);
}

void Animations::BombAnim::animate()
{
	if (animType == Animations::Bomb::Tick)
		doAnimation(0, Bomb::Freeze, ResourceManager::get().getAnimation(Entities::Bomb, animType), 15);
}

void Animations::BlockAnim::animate()
{
	if (animType == Animations::Block::VerticalExplosion)
		doAnimation(28, Block::Empty, ResourceManager::get().getAnimation(Entities::Block, animType), explosionDuration);
	else if (animType == Animations::Block::HorizontalExplosion)
		doAnimation(28, Block::Empty, ResourceManager::get().getAnimation(Entities::Block, animType), explosionDuration);
	else if (animType == Animations::Block::LeftExplosion)
		doAnimation(28, Block::Empty, ResourceManager::get().getAnimation(Entities::Block, animType), explosionDuration);
	else if (animType == Animations::Block::RightExplosion)
		doAnimation(28, Block::Empty, ResourceManager::get().getAnimation(Entities::Block, animType), explosionDuration);
	else if (animType == Animations::Block::UpExplosion)
		doAnimation(28, Block::Empty, ResourceManager::get().getAnimation(Entities::Block, animType), explosionDuration);
	else if (animType == Animations::Block::DownExplosion)
		doAnimation(28, Block::Empty, ResourceManager::get().getAnimation(Entities::Block, animType), explosionDuration);
	else if (animType == Animations::Block::CenterExplosion)
		doAnimation(28, Block::Empty, ResourceManager::get().getAnimation(Entities::Block, animType), explosionDuration);
	else if (animType == Animations::Block::Destruction)
		doAnimation(5, Block::Empty, ResourceManager::get().getAnimation(Entities::Block, animType), destructionDuration);
	else if (animType == Animations::Block::FirePowerup)
		doAnimation(0, Block::Static, ResourceManager::get().getAnimation(Entities::Block, animType), powerupDuration);
	else if (animType == Animations::Block::MegaFirePowerup)
		doAnimation(36, Block::Static, ResourceManager::get().getAnimation(Entities::Block, animType), powerupDuration);
	else if (animType == Animations::Block::BombPowerup)
		doAnimation(72, Block::Static, ResourceManager::get().getAnimation(Entities::Block, animType), powerupDuration);
	else if (animType == Animations::Block::SpeedPowerup)
		doAnimation(108, Block::Static, ResourceManager::get().getAnimation(Entities::Block, animType), powerupDuration);
}
