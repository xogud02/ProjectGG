#pragma once

#include "cocos2d.h"

class Character : public cocos2d::Sprite {
	int speed = 100;
	cocos2d::Vec2 nextPos;

	cocos2d::Action* currentAction;
	cocos2d::Action* moveLeft;
	cocos2d::Action* moveRight;
	cocos2d::Action* moveUp;
	cocos2d::Action* moveDown;

	void tick(float);
public:
	void move(cocos2d::Vec2);

	const int SCALE;

	static Character* createCharacter(float unitSize, int scale = 2);

	static cocos2d::Action* createMoveAction(std::string file, int r);

	Character(int scale);
};
