#pragma once
#include "cocos2d.h"

enum class CharacterType;

enum class SpriteTilePosition {
	TopLeft, Top, TopRight,
	Left, Center, Right,
	BottomLeft, Bottom, BottomRight,
	VirticalTop,
	VirticalCenter,
	VirticalBottom,
	HorizontalLeft, HorizontalCenter, HorizontalRight,
	Single,
};

enum class SpriteTileTheme {
	Brighter,
	Bright,
	Dark,
	Darker
};

enum class SpriteTileType {
	BrickLoop,Grass,Bush,
	Dirt,WoodFloor,Sand,
	Mound
};

enum class CharacterDirection {
	DOWN = 0,
	LEFT,
	RIGHT,
	UP
};

enum class ArrowDirection {
	Up,
	Right,
	Down,
	Left
};

enum class PitContentType {
	Empty,
	Skyblue,
	Blue,
	Green
};

enum class PitWallType {
	Brick,
	Dirt,
	Solid
};

enum class PitPositionType {
	TopLeft, Top, TopRight,
	Left, CenterOrBottom, Right,
	VirticalTop, BottomLeft, VirticalBottom, BottomRight, VirticalMiddle
};

enum class LiquidPitType {
	Lava,
	Ocean,
	Swamp
};

namespace std {
	template<>
	struct hash<CharacterType> {
		size_t operator()(CharacterType characterType) const;
	};
}

class SpriteFactory {
	SpriteFactory() = default;
	static const int iUnitSize = 16;

	static const std::string EXT;
	static std::unordered_map<CharacterType, std::string> characterPaths;
	static const std::string FLOOR;
	static const std::string TREE;
	static const std::string SLIME;
	static const std::string MELEE_WEAPON;
	static const std::string GUI;
	static const std::string PIT;

	static cocos2d::SpriteFrame* createFrame(const std::string& fileName, int x, int y);

	static cocos2d::Action* createAction(const std::string& path, int x, int y);
	static void initCharacterPaths();
public:
	static const cocos2d::Size unitSize;

	static float getUnitScale(float sizeInPoints);

	static cocos2d::Size getUnitSizeInPoints();

	static cocos2d::SpriteFrame* characterFrame(CharacterType);
	
	static cocos2d::SpriteFrame* floorFrame(SpriteTileType, SpriteTileTheme, SpriteTilePosition);

	static cocos2d::SpriteFrame* pitFrame(PitContentType, PitWallType, PitPositionType);

	static cocos2d::Action* liquidPitAction(LiquidPitType, PitPositionType);

	static cocos2d::SpriteFrame* GUIArrowFrame(ArrowDirection);

	static cocos2d::SpriteFrame* GUIGreenButton();

	static cocos2d::Action* tree();

	static cocos2d::Action* slime();

	static cocos2d::SpriteFrame* sword();

	static cocos2d::Action* characterMoveAction(CharacterType, CharacterDirection);

};

