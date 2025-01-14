#pragma once

#include <Actor/DrawableActor.h>

class Enemy : public DrawableActor
{
	// 적 캐릭터가 이동할 방향.
	enum class Direction
	{
		None = -1,
		Left,
		Right
	};

	RTTI_DECLARATIONS(Enemy, DrawableActor)

public:
	// 그릴때 사용할 문자열 값과 생성 y위치를 전달 받음.
	Enemy(const char* image, int yPosition);

	virtual void Update(float deltaTime) override;

private:

	// 이동 방향.
	Direction direction = Direction::None;

	// 좌우 이동에 필요한 변수.
	float xPosition = 0.0f;
	float speed = 5.0f;
};