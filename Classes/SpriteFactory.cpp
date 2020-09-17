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
const string SpriteFactory::PIT = "Tiles/PIT";
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
	auto ret = SpriteFrame::create(fileName + EXT, CC_RECT_PIXELS_TO_POINTS(Rect(x * iUnitSize, y*iUnitSize, iUnitSize, iUnitSize)));
	ret->getTexture()->setAliasTexParameters();
	return ret;
}

SpriteFrame* SpriteFactory::characterFrame(CharacterType characterType) {
	if (characterPaths.empty()) {
		initCharacterPaths();
	}
	return createFrame(characterPaths[characterType], 0, 0);
}

SpriteFrame* SpriteFactory::floorFrame(SpriteTileType type, SpriteTileTheme theme, SpriteTilePosition position) {
	auto pOffset = getTileOffset(position);
	auto offset = getTileOffset(type, theme);

	return createFrame(FLOOR, pOffset.first + offset.first, pOffset.second + offset.second + 3);
}

const pair<int, int> getPitPositionOffset(PitPositionType position) {
	auto val = static_cast<int>(position);
	if (position <= PitPositionType::Right) {
		return make_pair(val % 3, val / 3);
	}

	val -= static_cast<int>(PitPositionType::VirticalTop);
	return make_pair(val % 4 + 4, val / 4);
}

SpriteFrame* SpriteFactory::pitFrame(PitContentType content, PitWallType wall, PitPositionType position) {
	auto cv = static_cast<int>(content);
	auto wv = static_cast<int>(wall);
	auto pOffset = getPitPositionOffset(position);
	return createFrame(PIT + '0', pOffset.first, pOffset.second + 2 + 6 * cv + 2 * wv);
}

Action* SpriteFactory::liquidPitAction(LiquidPitType liquid, PitPositionType position) {
	auto pOffset = getPitPositionOffset(position);
	auto lv = static_cast<int>(liquid);
	return createAction(PIT, pOffset.first, pOffset.second + 2 * lv + 26);
}

SpriteFrame* SpriteFactory::GUIArrowFrame(ArrowDirection direction) {
	return createFrame(GUI + '0', 5, 1 + static_cast<int>(direction));
}

SpriteFrame* SpriteFactory::GUIGreenButton() {
	return createFrame(GUI + '0', 4, 13);
}

Action* SpriteFactory::tree() {
	return createAction(TREE, 9, 6);
}

Action* SpriteFactory::slime() {
	return createAction(SLIME, 0, 4);
}

SpriteFrame* SpriteFactory::sword() {
	return createFrame(MELEE_WEAPON, 0, 0);
}

Action* SpriteFactory::characterMoveAction(CharacterType characterType, CharacterDirection characterDirection) {
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
