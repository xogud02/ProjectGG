#pragma once

#include "cocos2d.h"
#include "MonsterEnums.h"

class MonsterSpriteFactory {

	static const std::string pList;
	static const std::string SLIME;
	MonsterSpriteFactory() = default;


	static cocos2d::SpriteFrame* createMonsterFrame(const std::string& fileName);
	static cocos2d::Action* createMonsterAction(const std::string& path, int index = 0);
	static cocos2d::Action* createMonsterAction(MonsterCategory, int index = 0);

public:
	static cocos2d::Action* slime();

	template<typename MonsterEnum>
	static cocos2d::Action* createMonsterAnimation(MonsterEnum type);
};

#include "MonsterSpriteFactory.hpp"
