#pragma once

template<typename MonsterEnum>
cocos2d::Action* MonsterSpriteFactory::createMonsterAnimation(MonsterEnum type) {
	return createMonsterAction(MonsterEnums::categoryOf(type), static_cast<int>(type));
}
