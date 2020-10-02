#pragma once

#include "cocos2d.h"

class Character;
class Weapon : public cocos2d::Sprite {
	Character* target = nullptr;
public:
	static Weapon* create();
	virtual bool init() override;
	void setTarget(Character* c);
	void swing(float duration);
};
