#include "CharacterSelectScene.h"
#include "Player.h"
#include "SpriteFactory.h"

USING_NS_CC;

bool CharacterSelectScene::init() {
	if (!Scene::init()) {
		return false;
	}

	constexpr int cols = 3;
	Vec2 center = getContentSize() / 2;
	for (int i = 0; i < CharacterTypeCount; ++i) {
		int row = -(i / cols);
		int col = i % cols;
		auto sprite = Sprite::createWithSpriteFrame(SpriteFactory::characterFrame(static_cast<CharacterType>(i)));
		addChild(sprite);
		sprite->setScale(5);
		sprite->setPosition(center + Vec2(col * 100, row * 100));
	}

	return true;
}
