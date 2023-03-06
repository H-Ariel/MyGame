#include "Player.h"
#include "LevelEngine.h"
#include "TilesCodes.h"
#include "CollisionDistances.h"
#include "MovingTile.h"


#define SpeedX_Normal					( 5.0f   * TILE_SIZE)
#define SpeedX_Disqualification			( 2.0f   * TILE_SIZE)
#define SpeedY_Normal					( 4.0f   * TILE_SIZE)
#define SpeedY_Gravity					(20.0f   * TILE_SIZE)
//#define SpeedY_Jump_Regular				(11.5f   * TILE_SIZE)
#define SpeedY_Jump_Regular				( 9.5f   * TILE_SIZE)
#define SpeedY_Jump_Small				( 8.333f * TILE_SIZE)
#define SpeedY_Jump_Disqualification	( 6.667f * TILE_SIZE)

#define MAX_X_POSITION	((mapSize.width - 1) * TILE_SIZE_F)
#define MAX_Y_POSITION	(mapSize.height * TILE_SIZE_F)

#define SUPER_SPEED_MAGIC_NUMBER 2


Player::Player(LevelEngine* engine, const D2D1_SIZE_U& mapSize)
	: Character(engine, { 0,0 }, 5), mapSize(mapSize)
{
	Reset();
}

void Player::Reset()
{
	position = { 0,0 };
	prevPosition = { 0,0 };
	speed = { 0,0 };
	lives = 5;
	coins = 0;
	stars = 0;
	GoesLeft = false;
	GoesRight = false;
	GoesUp = false;
	GoesDown = false;
	OnLadder = false;
	hurt = false;
	forward = false;
	leftCollision = false;
	rightCollision = false;
	movingTile = nullptr;
	superPower = SuperPower::None;
	superPowerTimer = 0;
	itemsCollected.clear();
}

// I used the above physics formulas:
// -> `x = v * t + x0`  for player position (`position.x` and `position.y`)
// -> `v = g * t + v0`  to apply gravity (for `speed.y`)
void Player::Logic(float elapsedTime)
{
	if (superPower != SuperPower::None)
		superPowerTimer -= elapsedTime;
	if (superPowerTimer <= 0)
		EndSuperPower();

	prevPosition = position; // save previous position. it used in `Player::GetMoveType`

	if (GoesUp && !OnLadder && speed.y == 0)
	{
		Jump(JumpTypes::RegularJump);
		return;
	}

	if (movingTile && !GoesRight && !GoesLeft && speed.y == 0)
	{
		// the player just stand
		return;
	}

	if (!GoesLeft && !GoesRight && !hurt) // if no key is pressed (and the player is not hurt) - stop move
	{
		speed.x = 0;
	}
	else
	{
		if (GoesLeft && !leftCollision)
		{
			speed.x = -SpeedX_Normal;
			if (!hurt) forward = true;
		}
		if (GoesRight && !rightCollision)
		{
			speed.x = SpeedX_Normal;
			if (!hurt) forward = false;
		}
	}

	if (OnLadder)
	{
		hurt = false;

		if (!GoesUp && !GoesDown) speed.y = 0;
		else
		{
			if (GoesUp) speed.y = -SpeedY_Normal;
			if (GoesDown) speed.y = SpeedY_Normal;
		}
	}
	else
	{
		if (movingTile == nullptr) // if not on moving tile apply gravity
			speed.y += SpeedY_Gravity * elapsedTime;
	}

	if (superPower == SuperPower::SuperSpeed)
	{
		speed.x *= SUPER_SPEED_MAGIC_NUMBER;
		//if (OnLadder || speed.y < 0) // we can't fall faster (`speed.y<0` is in jump case)
		if (OnLadder) // only "super climb" (not "super-jump" or "super-fall")
			speed.y *= SUPER_SPEED_MAGIC_NUMBER;
	}

	// update position based on speed, but make sure we don't go outside the level
	position.x += speed.x * elapsedTime;
	position.y += speed.y * elapsedTime;

	if (position.x < 0)
	{
		position.x = 0;
		speed.x = 0;
	}
	if (position.x > MAX_X_POSITION)
	{
		position.x = MAX_X_POSITION;
		speed.x = 0;
	}
	if (position.y < 0)
	{
		position.y = 0;
		speed.y = 0;
	}
	if (position.y > MAX_Y_POSITION)
	{
		NoLives(); // if the player falls below the level, game over
	}

	leftCollision = false;
	rightCollision = false;
	OnLadder = false;
}
void Player::Draw() const
{
	D2D1_RECT_F dst = GetRect();
	engine->RemoveOffset(dst);
	AssetsManager::DrawPlayer(dst, GetMoveType(), forward);

	if (superPower == SuperPower::Immortal)
	{
		AssetsManager::DrawTile(
			D2D1::RectF(dst.left + TILE_SIZE / 2, dst.top + TILE_SIZE / 5, dst.right + TILE_SIZE / 2, dst.bottom),
			TilesCodes::ShieldSilver
		);

		AssetsManager::DrawTile(
			D2D1::RectF(dst.left - TILE_SIZE / 1.7f, dst.top - TILE_SIZE / 5, dst.right - TILE_SIZE / 1.7f, dst.bottom - TILE_SIZE / 5),
			TilesCodes::SwordSilver, -15
		);
	}
	// TODO: draw something to indicate the more super-powers

}

void Player::Disqualification(D2D1_RECT_F collisionRect)
{
	if (superPower == SuperPower::Immortal || hurt) return;

	Character::Disqualification();
	hurt = true;

	// jump back with sad face
	Jump(JumpTypes::DisqualificationJump);
	CollisionDistances::keepSmallest(collisionRect);
	if (collisionRect.left > 0) speed.x = SpeedX_Disqualification;
	else if (collisionRect.right > 0) speed.x = -SpeedX_Disqualification;
}

void Player::StopFalling(float collisionSize)
{
	speed.y = 0;
	position.y -= collisionSize;
	hurt = false;
}
void Player::Jump(JumpTypes jumpType, float force)
{
	if (OnLadder) return;

	switch (jumpType)
	{
	case JumpTypes::RegularJump:			speed.y = -SpeedY_Jump_Regular; break;
	case JumpTypes::SmallJump:				speed.y = -SpeedY_Jump_Small; break;
	case JumpTypes::DisqualificationJump:	speed.y = -SpeedY_Jump_Disqualification; break;
	case JumpTypes::SpringboardJump:		speed.y = -force * TILE_SIZE; break;
	default: break;
	}
	GoesUp = false;
	movingTile = nullptr;
}
void Player::BounceTop()
{
	speed.y = abs(speed.y);
}
void Player::StopMovingLeft(float collisionSize)
{
	speed.x = 0;
	position.x += collisionSize;
	leftCollision = true;
}
void Player::StopMovingRight(float collisionSize)
{
	speed.x = 0;
	position.x -= collisionSize;
	rightCollision = true;
}

void Player::CollectItem(short itemId)
{
	switch (itemId)
	{
	case CoinBronze:
	case CoinSilver:
	case CoinGold:
		coins += 1;
		break;

	case Star:
		stars += 1;
		break;

	case GemBlue:
//	case GemGreen:
//	case GemRed:
//	case GemYellow:
//	case ShieldBronze:
//	case ShieldGold:
	case ShieldSilver:
		SetSuperPower(itemId);
		break;

	case KeyBlue:
	case KeyGreen:
	case KeyRed:
	case KeyYellow:
		itemsCollected.push_back(itemId);
		break;

	case Heart:
		lives += 1;
		break;

	default:
		break;
	}
}
bool Player::HasItem(short itemId) const
{
	return FIND_IN_ARRAY(itemsCollected, itemId);
}
void Player::RemoveItem(short itemId)
{
	for (size_t i = 0; i < itemsCollected.size(); i++)
	{
		if (itemsCollected[i] == itemId)
		{
			itemsCollected.erase(itemsCollected.begin() + i);
			return; // do not remove more
		}
	}
}

int Player::GetLives() const
{
	return lives;
}
int Player::GetCoins() const
{
	return coins;
}
int Player::GetStars() const
{
	return stars;
}
float Player::GetSpeedY() const
{
	return speed.y;
}
bool Player::IsFalling() const
{
	return speed.y > 0;
}
bool Player::IsHurt() const
{
	return hurt;
}
void Player::NoLives()
{
	lives = 0;
}

void Player::SetSuperPower(short itemId)
{
	// todo: add more super powers

	switch (itemId)
	{
	case GemBlue:
		superPower = SuperPower::SuperSpeed;
		break;

	case ShieldSilver:
		superPower = SuperPower::Immortal;
		break;

	default:
		return; // no super power
	}

	itemsCollected.push_back(itemId);
	superPowerTimer = 10; // super power for 10 seconds
}
void Player::EndSuperPower()
{
	superPower = SuperPower::None;
	superPowerTimer = 0;

	RemoveItem(GemBlue);
	RemoveItem(GemGreen);
	RemoveItem(GemRed);
	RemoveItem(GemYellow);
}

bool Player::IsImmortal() const
{
	return superPower == SuperPower::Immortal;
}

D2D1_SIZE_F Player::GetSize() const
{
	return AssetsManager::GetPlayerSize(GetMoveType());
}
string Player::GetMoveType() const
#define GetStep(pos) (int)(fmodf(pos, TILE_SIZE * 2.0f) / TILE_SIZE + 1) // the result is 1 or 2
{
	string type = "stand"; // `type` could be: (front,) climb[1/2], duck, hurt, jump, stand, walk[1/2]

	if (hurt)
	{
		type = "hurt";
	}
	else if (position.y != prevPosition.y && !OnLadder) // if the Player is in the middle of the jump
	{
		type = "jump";
	}
	else if (GoesDown && !OnLadder)
	{
		type = "duck";
	}
	else if (position.x != prevPosition.x) // if the Player moves on ground
	{
		type = "walk" + to_string(GetStep(position.x));
	}
	else if (OnLadder)
	{
		type = "climb" + to_string(GetStep(position.y));
	}

	return "alien" + SettingManager::PlayerColor + "_" + type;
}
