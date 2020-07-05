#pragma once
#include "cocos2d.h";

enum class CharacterDirection {
	DOWN = 0,
	LEFT,
	RIGHT,
	UP
};

class SpriteFactory {
	SpriteFactory() = default;

	static const std::string EXT;
	static const std::string WARRIOR;
	static const std::string FLOOR;
	static const std::string TREE;
	static const std::string SLIME;

	static cocos2d::SpriteFrame* createFrame(const std::string& fileName, int x, int y) {
		return cocos2d::SpriteFrame::create(fileName + EXT, CC_RECT_PIXELS_TO_POINTS(cocos2d::Rect(x * unitSize, y*unitSize, unitSize, unitSize)));
	}

	static cocos2d::Action* createAction(const std::string& path, int x, int y) {
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
public:
	static const int unitSize = 16;

	static float getUnitScale(float scale) {
		return scale * CC_CONTENT_SCALE_FACTOR() / unitSize;
	}

	static cocos2d::SpriteFrame* worriorFrame() {
		return createFrame(WARRIOR, 0, 0);
	}

	static cocos2d::SpriteFrame* grassFrame() {
		return createFrame(FLOOR, 8, 7);
	}

	static cocos2d::Action* tree() {
		return createAction(TREE, 9,6);
	}

	static cocos2d::Action* slime() {
		return createAction(SLIME, 0, 4);
	}

	static cocos2d::Action* worriorMoveAction(CharacterDirection characterDirection)
	{
		auto anim = cocos2d::Animation::create();
		anim->setDelayPerUnit(0.3f);
		for (int i = 0; i < 4; ++i) {
			auto frame = createFrame(WARRIOR, i, static_cast<int>(characterDirection));
			anim->addSpriteFrame(frame);
		}

		auto ret = cocos2d::RepeatForever::create(cocos2d::Animate::create(anim));
		ret->retain();
		return ret;
	}
};

