#include "MonsterSpriteFactory.h"
#include "SpriteFactory.h"
#include "MonsterEnums.h"
#include <unordered_map>

using namespace std;
USING_NS_CC;

const string MonsterSpriteFactory::pList = "Monsters.plist";
const string MonsterSpriteFactory::SLIME = "Slime";
const unordered_map<MonsterCategory, string> map = {
	{MonsterCategory::Aquatic, "Aquatic" },
	{MonsterCategory::Slime, "Slime"},
	{MonsterCategory::Elemental,"Elemental"}
};

string getFileName(const string& path, int animationIndex, int categoryIndex) {
	return path + to_string(animationIndex) + SpriteFactory::EXT + "/" + to_string(categoryIndex) + SpriteFactory::EXT;
}

void MonsterSpriteFactory::init() {
	auto cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile(SpriteFactory::root + pList);
	cache->getSpriteFrameByName(getFileName(::map.begin()->second, 0, 0))->getTexture()->setAliasTexParameters();
}

Action * MonsterSpriteFactory::createMonsterAction(const string & path, int index) {
	return SpriteFactory::createAnim([path, index](int i) {return createMonsterFrame(getFileName(path, i, index)); });
}

Action * MonsterSpriteFactory::createMonsterAction(MonsterCategory catergory, int index) {
	return createMonsterAction(::map.find(catergory)->second, index);
}

SpriteFrame * MonsterSpriteFactory::createMonsterFrame(const std::string & fileName) {
	return SpriteFrameCache::getInstance()->getSpriteFrameByName(fileName);
}

