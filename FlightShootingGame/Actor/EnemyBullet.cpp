#include "EnemyBullet.h"
#include "Engine/Engine.h"

EnemyBullet::EnemyBullet(const Vector2& position)
	: DrawableActor("o")
{
	// 위치 설정.
	this->position = position;
	yPosition = (float)position.y;
}

void EnemyBullet::Update(float deltaTime)
{
	Super::Update(deltaTime);

	// 이동 로직.
	yPosition += speed * deltaTime;
	position.y = (int)yPosition;

	// 화면 벗어나면 제거.
	if (yPosition >= Engine::Get().ScreenSize().y)
	{
		Destroy();
	}
}