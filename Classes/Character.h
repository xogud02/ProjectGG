#pragma once

#include "cocos2d.h"

class Character : public cocos2d::Sprite {
	int speed = 100;
	cocos2d::Vec2 nextPos;

	void tick(float);
public:
	void move(cocos2d::Vec2);

	const int SCALE;

	static Character* createCharacter(int scale = 2);

	Character(int scale);
};
