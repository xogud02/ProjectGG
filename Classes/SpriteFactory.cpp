#include "SpriteFactory.h"
#include "Player.h"
using namespace std;
USING_NS_CC;

const string SpriteFactory::EXT = ".png";

namespace std {
	size_t hash<CharacterType>::operator()(CharacterType characterType) const {
			return hash<int>()(static_cast<int>(characterType));
	}
}

unordered_map<CharacterType, string> SpriteFactory::characterPaths;

const string SpriteFactory::FLOOR = "Tiles/Floor";
const string SpriteFactory::TREE = "Tiles/Tree";
const string SpriteFactory::SLIME = "Monsters/Slime";
const string SpriteFactory::MELEE_WEAPON = "Items/MedWep";

const Size SpriteFactory::unitSize(SpriteFactory::iUnitSize, SpriteFactory::iUnitSize);

Action* SpriteFactory::createAction(const string& path, int x, int y) {
	auto anim = cocos2d::Animation::create();
	anim->setDelayPerUnit(0.3f);
	for (char i = '0'; i < '2'; ++i) {
		auto frame = createFrame(path + i, x, y);
		anim->addSpriteFrame(frame);
	}

	auto ret = cocos2d::RepeatForever::create(cocos2d::Animate::create(anim));
	ret->retain();
	return ret;
}

void SpriteFactory::initCharacterPaths() {
	string characters = "Characters/";
	characterPaths[CharacterType::Engineer] = characters + "Engineer";
	characterPaths[CharacterType::Mage] = characters + "Mage";
	characterPaths[CharacterType::Paladin] = characters + "Paladin";
	characterPaths[CharacterType::Rogue] = characters + "Rogue";
	characterPaths[CharacterType::Template] = characters + "Template";
	characterPaths[CharacterType::Warrior] = characters + "Warrior";
}

float SpriteFactory::getUnitScale(float sizeInPoints) {
	return sizeInPoints * CC_CONTENT_SCALE_FACTOR() / iUnitSize;
}

cocos2d::Size SpriteFactory::getUnitSizeInPoints() {
	return CC_SIZE_PIXELS_TO_POINTS(unitSize);
}

SpriteFrame* SpriteFactory::createFrame(const std::string& fileName, int x, int y) {
	auto ret =  SpriteFrame::create(fileName + EXT, CC_RECT_PIXELS_TO_POINTS(Rect(x * iUnitSize, y*iUnitSize, iUnitSize, iUnitSize)));
	ret->getTexture()->setAliasTexParameters();
	return ret;
}

SpriteFrame * SpriteFactory::characterFrame(CharacterType characterType) {
	if (characterPaths.empty()) {
		initCharacterPaths();
	}
	return createFrame(characterPaths[characterType], 0, 0);
}

cocos2d::SpriteFrame * SpriteFactory::grassFrame() {
	return createFrame(FLOOR, 8, 7);
}

cocos2d::Action * SpriteFactory::tree() {
	return createAction(TREE, 9, 6);
}

cocos2d::Action * SpriteFactory::slime() {
	return createAction(SLIME, 0, 4);
}

cocos2d::SpriteFrame * SpriteFactory::sword() {
	return createFrame(MELEE_WEAPON, 0, 0);
}

cocos2d::Action * SpriteFactory::characterMoveAction(CharacterType characterType, CharacterDirection characterDirection) {
	if (characterPaths.empty()) {
		initCharacterPaths();
	}

	auto anim = cocos2d::Animation::create();
	anim->setDelayPerUnit(0.3f);
	for (int i = 0; i < 4; ++i) {
		auto frame = createFrame(characterPaths[characterType], i, static_cast<int>(characterDirection));
		anim->addSpriteFrame(frame);
	}

	auto ret = cocos2d::RepeatForever::create(cocos2d::Animate::create(anim));
	ret->retain();
	return ret;
	return nullptr;
}
