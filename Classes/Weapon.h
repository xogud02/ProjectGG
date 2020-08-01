#pragma once

#include "cocos2d.h"
class Weapon : public cocos2d::Sprite {
public:
	static Weapon* create();
	void swing();
};
