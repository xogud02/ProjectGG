#pragma once

#include "cocos2d.h"
#include "SpriteTileEnums.h"

class TileSpriteFactory {
	TileSpriteFactory() = default;
	static const std::string FLOOR;
	static const std::string WALL;
	static const std::string DOOR;
	static const std::string TREE;
	static const std::string PIT;
public:

	static cocos2d::SpriteFrame* doorFrame(DoorType dt = DoorType::Wood, DoorPosition dp = DoorPosition::Horizontal, DoorLockType dl = DoorLockType::None, bool closed = true);

	static cocos2d::SpriteFrame* testWallFrame(WallPosition);
	static cocos2d::SpriteFrame* wallFrame(WallType, WallPosition, SpriteTileTheme);

	static cocos2d::SpriteFrame* floorFrame(SpriteTileType, SpriteTileTheme, SpriteTilePosition);
	static cocos2d::SpriteFrame* testPitPosition(PitPositionType);

	static cocos2d::Action* pitAction(PitContentType, PitWallType, PitPositionType);

	static cocos2d::Action* liquidPitAction(LiquidPitType, PitPositionType);

	static cocos2d::Action* tree();

};
