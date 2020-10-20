#include "TileSpriteFactory.h"
#include "SpriteFactory.h"

using namespace std;
USING_NS_CC;

const string tiles = "Tiles/";

const string TileSpriteFactory::FLOOR = tiles + "Floor";
const string TileSpriteFactory::PIT = tiles + "PIT";
const string TileSpriteFactory::TREE = tiles + "Tree";

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

SpriteFrame * TileSpriteFactory::floorFrame(SpriteTileType type, SpriteTileTheme theme, SpriteTilePosition position) {
	auto pOffset = getTileOffset(position);
	auto offset = getTileOffset(type, theme);

	return SpriteFactory::createFrame(FLOOR, pOffset.first + offset.first, pOffset.second + offset.second + 3);
}

const pair<int, int> getPitPositionOffset(PitPositionType position) {
	auto val = static_cast<int>(position);
	if (position <= PitPositionType::Right) {
		return make_pair(val % 3, val / 3);
	}

	val -= static_cast<int>(PitPositionType::VirticalTop);
	return make_pair(val % 4 + 4, val / 4);
}

SpriteFrame * TileSpriteFactory::testPitPosition(PitPositionType position) {
	//exception for resource file
	if (position == PitPositionType::BottomLeft) {
		position = PitPositionType::BottomRight;
	} else if (position == PitPositionType::BottomRight) {
		position = PitPositionType::BottomLeft;
	}

	auto offset = getPitPositionOffset(position);

	return SpriteFactory::createFrame(PIT + "0", offset.first, offset.second);
}

Action * TileSpriteFactory::pitAction(PitContentType content, PitWallType wall, PitPositionType position) {
	auto cv = static_cast<int>(content);
	auto wv = static_cast<int>(wall);
	auto pOffset = getPitPositionOffset(position);
	return SpriteFactory::createAction(PIT, pOffset.first, pOffset.second + 2 + 6 * cv + 2 * wv);
}

Action * TileSpriteFactory::liquidPitAction(LiquidPitType liquid, PitPositionType position) {
	auto pOffset = getPitPositionOffset(position);
	auto lv = static_cast<int>(liquid);
	return SpriteFactory::createAction(PIT, pOffset.first, pOffset.second + 2 * lv + 26);
}

Action * TileSpriteFactory::tree() {
	return SpriteFactory::createAction(TREE, 9, 6);
}