#pragma once
#include "cocos2d.h"

enum class CharacterType;

enum class SpriteTileType {
	TopLeft, Top, TopRight,
	Left, Center, Right,
	BottomLeft, Bottom, BottomRight,
	VirticalTop,
	VirticalCenter,
	VirticalBottom,
	HorizontalLeft, HorizontalCenter, HorizontalRight,
	Single,
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

namespace std {
	template<>
	struct hash<CharacterType> {
		size_t operator()(CharacterType characterType) const;
	};
}

class SpriteFactory {
	SpriteFactory() = default;
	static const int iUnitSize = 16;


	static const std::pair<int, int> getTileOffset(SpriteTileType);

	static const std::string EXT;
	static std::unordered_map<CharacterType, std::string> characterPaths;
	static const std::string FLOOR;
	static const std::string TREE;
	static const std::string SLIME;
	static const std::string MELEE_WEAPON;
	static const std::string GUI;

	static cocos2d::SpriteFrame* createFrame(const std::string& fileName, int x, int y);

	static cocos2d::Action* createAction(const std::string& path, int x, int y);
	static void initCharacterPaths();
public:
	static const cocos2d::Size unitSize;

	static float getUnitScale(float sizeInPoints);

	static cocos2d::Size getUnitSizeInPoints();

	static cocos2d::SpriteFrame* characterFrame(CharacterType);
	
	static cocos2d::SpriteFrame* grassFrame(SpriteTileType tileType = SpriteTileType::Center);
	
	static cocos2d::SpriteFrame* dirtFrame(SpriteTileType tileType = SpriteTileType::Center);

	static cocos2d::SpriteFrame* GUIArrowFrame(ArrowDirection);

	static cocos2d::SpriteFrame* GUIGreenButton();

	static cocos2d::Action* tree();

	static cocos2d::Action* slime();

	static cocos2d::SpriteFrame* sword();

	static cocos2d::Action* characterMoveAction(CharacterType, CharacterDirection);

};

