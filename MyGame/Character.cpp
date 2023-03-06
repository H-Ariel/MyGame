#include "Character.h"
#include "LevelEngine.h"


Character::Character(LevelEngine* engine, D2D1_POINT_2F initialPosition, int lives)
	: LevelUIElementBase(engine, initialPosition), speed({ 0,0 }), lives(lives), forward(false)
{
}
Character::~Character()
{
}

bool Character::HasLives()
{
	return lives > 0;
}
void Character::Disqualification()
{
	if (lives > 0)
		lives -= 1;
}

void Character::ChangeZoomRatio(int oldTileSize, int newTileSize)
{
	LevelUIElementBase::ChangeZoomRatio(oldTileSize, newTileSize);
	ChangePointZoomRatio(speed, oldTileSize, newTileSize);
}

D2D1_RECT_F Character::GetRect() const
{
	D2D1_SIZE_F size = GetSize();
	return D2D1::RectF(
		position.x,
		position.y - size.height,
		position.x + size.width,
		position.y
	);
}
