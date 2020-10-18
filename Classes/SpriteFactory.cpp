#include "SpriteFactory.h"
#include "Player.h"
using namespace std;
USING_NS_CC;

const string SpriteFactory::EXT = ".png";
const string root = "Sprites/";

const string SpriteFactory::pList = root + "Monsters.plist";

namespace std {
	size_t hash<CharacterType>::operator()(CharacterType characterType) const {
		return hash<int>()(static_cast<int>(characterType));
	}
}

const string characters = "Characters/";
const unordered_map<CharacterType, string> SpriteFactory::characterPaths = {
	{CharacterType::Engineer,characters + "Engineer"},
	{CharacterType::Mage, characters + "Mage"},
	{CharacterType::Paladin, characters + "Paladin"},
	{CharacterType::Rogue, characters + "Rogue"},
	{CharacterType::Template, characters + "Template"},
	{CharacterType::Warrior, characters + "Warrior"},
};

const string tiles = "Tiles/";

const string SpriteFactory::FLOOR = tiles+"Floor";
const string SpriteFactory::PIT = tiles+"PIT";
const string SpriteFactory::TREE = tiles+"Tree";
const string SpriteFactory::SLIME = "Slime";
const string SpriteFactory::MELEE_WEAPON = "Items/MedWep";
const string SpriteFactory::GUI = "GUIs/GUI";

const Size SpriteFactory::unitSize(SpriteFactory::iUnitSize, SpriteFactory::iUnitSize);

Action* createAnim(function<SpriteFrame*(char)>createFrame) {
	Vector<SpriteFrame*> frames;
	for (int i = 0; i < 2; ++i) {
		frames.pushBack(createFrame(i));
	}
	auto animation = Animation::createWithSpriteFrames(frames, 0.3f);
	auto singleLoop = Animate::create(animation);
	return RepeatForever::create(singleLoop);
}

Action* SpriteFactory::createAction(const string& path, int x, int y) {
	return createAnim([path, x, y](int i) {return createFrame(path + to_string(i), x, y); });
}

Action * SpriteFactory::createMonsterAction(const std::string & path, int index) {
	return createAnim([path, index](int i) {return createMonsterFrame(path + to_string(i) + EXT + "/"+ to_string(index) + EXT); });
}

const string SpriteFactory::getCharacterFileName(CharacterType type) {
	return characterPaths.find(type)->second;
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

bool flag = true;

SpriteFrame* SpriteFactory::createFrame(const string& fileName, int x, int y) {
	auto xOffset = x * iUnitSize, yOffset = y * iUnitSize;
	auto ret = SpriteFrame::create(root + fileName + EXT, CC_RECT_PIXELS_TO_POINTS(Rect(x * iUnitSize, y*iUnitSize, iUnitSize, iUnitSize)));
	ret->getTexture()->setAliasTexParameters();
	return ret;
}

bool inited = false;
SpriteFrame * SpriteFactory::createMonsterFrame(const std::string & fileName) {
	auto cache = SpriteFrameCache::getInstance();
	if (!inited) {
		inited = true;
		cache->addSpriteFramesWithFile(pList);
		cache->getSpriteFrameByName(fileName)->getTexture()->setAliasTexParameters();
	}
	return cache->getSpriteFrameByName(fileName);
}

SpriteFrame* SpriteFactory::characterFrame(CharacterType characterType) {
	return createFrame(getCharacterFileName(characterType), 0, 0);
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

SpriteFrame * SpriteFactory::testPitPosition(PitPositionType position) {

	//exception for resource file
	if (position == PitPositionType::BottomLeft) {
		position = PitPositionType::BottomRight;
	} else if (position == PitPositionType::BottomRight) {
		position = PitPositionType::BottomLeft;
	}

	auto offset = getPitPositionOffset(position);

	return createFrame(PIT + "0", offset.first, offset.second);
}

Action* SpriteFactory::pitAction(PitContentType content, PitWallType wall, PitPositionType position) {
	auto cv = static_cast<int>(content);
	auto wv = static_cast<int>(wall);
	auto pOffset = getPitPositionOffset(position);
	return createAction(PIT, pOffset.first, pOffset.second + 2 + 6 * cv + 2 * wv);
}

Action* SpriteFactory::liquidPitAction(LiquidPitType liquid, PitPositionType position) {
	auto pOffset = getPitPositionOffset(position);
	auto lv = static_cast<int>(liquid);
	return createAction(PIT, pOffset.first, pOffset.second + 2 * lv + 26);
}

SpriteFrame* SpriteFactory::GUIArrowFrame(ArrowDirection direction) {
	return createFrame(GUI + '0', 5, 1 + static_cast<int>(direction));
}

const pair<int, int> getGUIColorOffset(GUIFrameColor color, bool filled) {
	auto val = static_cast<int>(color);
	return make_pair((val % 2 * 2 + filled ? 1 : 0) * 4, val / 2 * 3);
}

const pair<int, int>getGUIPositionOffset(GUIFramePart position) {
	if (position == GUIFramePart::Single) {
		return make_pair(0, 0);
	}

	auto val = static_cast<int>(position);
	return make_pair(val % 3 + 1, val / 3);
}

SpriteFrame * SpriteFactory::GUIFrame(GUIFrameColor color, GUIFramePart pos, bool filled) {
	auto cOffset = getGUIColorOffset(color, filled);
	auto pOffset = getGUIPositionOffset(pos);
	return createFrame(GUI + "0", cOffset.first + pOffset.first, 7 + cOffset.second + pOffset.second);
}

Action* SpriteFactory::tree() {
	return createAction(TREE, 9, 6);
}

Action* SpriteFactory::slime() {
	return createMonsterAction(SLIME);
}

SpriteFrame* SpriteFactory::sword() {
	return createFrame(MELEE_WEAPON, 0, 0);
}

Action* SpriteFactory::characterMoveAction(CharacterType characterType, CharacterDirection characterDirection) {
	auto anim = Animation::create();
	anim->setDelayPerUnit(0.3f);
	for (int i = 0; i < 4; ++i) {
		auto frame = createFrame(getCharacterFileName(characterType), i, static_cast<int>(characterDirection));
		anim->addSpriteFrame(frame);
	}

	auto ret = RepeatForever::create(Animate::create(anim));
	ret->retain();
	return ret;
}
