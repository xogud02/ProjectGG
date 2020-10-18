#pragma once

#include "cocos2d.h"

class MonsterSpriteFactory {

	static const std::string pList;
	static const std::string SLIME;
	MonsterSpriteFactory() = default;


	static cocos2d::SpriteFrame* createMonsterFrame(const std::string& fileName);
	static cocos2d::Action* createMonsterAction(const std::string& path, int index = 0);

public:
	static cocos2d::Action* slime();

};
