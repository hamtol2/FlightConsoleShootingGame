#pragma once

#include "Actor/DrawableActor.h"

class Player : public DrawableActor
{
	RTTI_DECLARATIONS(Player, DrawableActor)

public:
	Player(const char* image);

	virtual void Update(float deltaTime) override;
};