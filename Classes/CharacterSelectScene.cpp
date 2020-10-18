#include "CharacterSelectScene.h"
#include "Player.h"
#include "SpriteFactory.h"
#include "FightScene.h"
#include "CharacterSpriteFactory.h"

USING_NS_CC;

CharacterSelectScene* CharacterSelectScene::create(SpriteTileTheme theme) {
	auto ret = new CharacterSelectScene();
	if (!ret || !ret->init()) {
		CC_SAFE_DELETE(ret);
		return nullptr;
	}

	constexpr int cols = 3;
	int rows = CharacterTypeCount / cols;
	auto size = ret->getContentSize();
	auto foo = size.width / 6;
	Vec2 center = size / 2;
	auto menu = Menu::create();
	ret->addChild(menu);
	for (int i = 0; i < CharacterTypeCount; ++i) {
		int row = i / cols;
		int col = i % cols;

		auto sprite = Sprite::createWithSpriteFrame(CharacterSpriteFactory::characterFrame(static_cast<CharacterType>(i)));
		sprite->getTexture()->setAliasTexParameters();

		auto item = MenuItemSprite::create(sprite, sprite, [theme, i](auto ref) { Director::getInstance()->replaceScene(FightScene::create(theme, static_cast<CharacterType>(i))); });
		item->setScale(foo * 4 / 5 / sprite->getContentSize().width);
		item->setPosition(Vec2((col - cols / 2) * foo, (0.5f - row)* foo));
		menu->addChild(item);
	}

	return ret;
}

bool CharacterSelectScene::init() {
	if (!Scene::init()) {
		return false;
	}


	return true;
}
