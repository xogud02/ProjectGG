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
const string SpriteFactory::GUI = "GUIs/GUI";

const Size SpriteFactory::unitSize(SpriteFactory::iUnitSize, SpriteFactory::iUnitSize);

Action* SpriteFactory::createAction(const string& path, int x, int y) {
	auto anim = Animation::create();
	anim->setDelayPerUnit(0.3f);
	for (char i = '0'; i < '2'; ++i) {
		auto frame = createFrame(path + i, x, y);
		anim->addSpriteFrame(frame);
	}

	auto ret = RepeatForever::create(Animate::create(anim));
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

Size SpriteFactory::getUnitSizeInPoints() {
	return CC_SIZE_PIXELS_TO_POINTS(unitSize);
}

const pair<int, int> getTileOffset(SpriteTilePosition position) {
	auto typeVal = static_cast<int>(position);
	if (SpriteTilePosition::TopLeft <= position && position <= SpriteTilePosition::BottomRight) {
		return pair<int, int>(typeVal % 3, typeVal / 3);
	}
	if (SpriteTilePosition::VirticalTop <= position && position <= SpriteTilePosition::VirticalBottom) {
		return pair<int, int>(3, typeVal % 3);
	}
	if (SpriteTilePosition::HorizontalLeft <= position && position <= SpriteTilePosition::HorizontalRight) {
		return pair<int, int>(4 + typeVal % 3, 1);
	}
	return pair<int, int>(5, 0);
}

const pair<int, int> getTileOffset(SpriteTileType type, SpriteTileTheme theme) {
	auto typeVal = static_cast<int>(type);
	int typeX = typeVal % 3;
	int typeY = typeVal / 3;

	auto themeY = static_cast<int>(theme);
	
	return pair<int, int>(typeX * 7, typeY * 12 + themeY * 3);
}


SpriteFrame* SpriteFactory::createFrame(const string& fileName, int x, int y) {
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

SpriteFrame * SpriteFactory::floorFrame(SpriteTileType type, SpriteTileTheme theme, SpriteTilePosition position) {
	auto pOffset = getTileOffset(position);
	auto offset = getTileOffset(type, theme);

	return createFrame(FLOOR, pOffset.first + offset.first, pOffset.second + offset.second + 3);
}

SpriteFrame * SpriteFactory::grassFrame(SpriteTilePosition type) {
	auto offset = getTileOffset(type);
	return createFrame(FLOOR, 7 + offset.first, 6 + offset.second);
}

SpriteFrame * SpriteFactory::dirtFrame(SpriteTilePosition type) {
	auto offset = getTileOffset(type);
	return createFrame(FLOOR, offset.first, 18 + offset.second);
}

SpriteFrame * SpriteFactory::GUIArrowFrame(ArrowDirection direction) {
	return createFrame(GUI + '0', 5, 1 + static_cast<int>(direction));
}

SpriteFrame * SpriteFactory::GUIGreenButton() {
	return createFrame(GUI + '0', 4, 13);
}

Action * SpriteFactory::tree() {
	return createAction(TREE, 9, 6);
}

Action * SpriteFactory::slime() {
	return createAction(SLIME, 0, 4);
}

SpriteFrame * SpriteFactory::sword() {
	return createFrame(MELEE_WEAPON, 0, 0);
}

Action * SpriteFactory::characterMoveAction(CharacterType characterType, CharacterDirection characterDirection) {
	if (characterPaths.empty()) {
		initCharacterPaths();
	}

	auto anim = Animation::create();
	anim->setDelayPerUnit(0.3f);
	for (int i = 0; i < 4; ++i) {
		auto frame = createFrame(characterPaths[characterType], i, static_cast<int>(characterDirection));
		anim->addSpriteFrame(frame);
	}

	auto ret = RepeatForever::create(Animate::create(anim));
	ret->retain();
	return ret;
}
