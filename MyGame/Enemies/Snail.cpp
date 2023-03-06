#include "Snail.h"
#include "..\LevelEngine.h"


Snail::Snail(LevelEngine* engine, D2D1_POINT_2F initialPosition, float maxX)
	: HorizontalEnemy(engine, initialPosition, maxX, EnemyTypes::snail)
{
	lives = 2;
}
bool Snail::CanDisqualify() const
{
	return lives > 1; // if lives=1 its shell, and shell can't disqualify (if lives=0 its dead)
}
void Snail::Logic(float elapsedTime)
{
	if (lives > 1) // if lives=1 its shell, and shell can't move
	{
		HorizontalEnemy::Logic(elapsedTime);
	}
}
string Snail::GetMoveType() const
{
	string moveType(GetName(type));
	if (lives == 1) moveType += "_shell";
	else if (GetStep(position.x) == 2) moveType += "_walk";
	return moveType;
}
