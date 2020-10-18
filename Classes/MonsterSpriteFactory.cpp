#include "MonsterSpriteFactory.h"
#include "SpriteFactory.h"

using namespace std;
USING_NS_CC;

const string MonsterSpriteFactory::pList = "Monsters.plist";
const string MonsterSpriteFactory::SLIME = "Slime";

Action * MonsterSpriteFactory::slime() {
	return createMonsterAction(SLIME);
}

Action * MonsterSpriteFactory::createMonsterAction(const std::string & path, int index) {
	return SpriteFactory::createAnim([path, index](int i) {return createMonsterFrame(path + to_string(i) + SpriteFactory::EXT + "/" + to_string(index) + SpriteFactory::EXT); });
}

bool inited = false;
SpriteFrame * MonsterSpriteFactory::createMonsterFrame(const std::string & fileName) {
	auto cache = SpriteFrameCache::getInstance();
	if (!inited) {
		inited = true;
		cache->addSpriteFramesWithFile(SpriteFactory::root + pList);
		cache->getSpriteFrameByName(fileName)->getTexture()->setAliasTexParameters();
	}
	return cache->getSpriteFrameByName(fileName);
}

