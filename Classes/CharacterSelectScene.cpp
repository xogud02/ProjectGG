#include "CharacterSelectScene.h"
#include "Player.h"
#include "SpriteFactory.h"
#include "FightScene.h"

USING_NS_CC;

bool CharacterSelectScene::init() {
	if (!Scene::init()) {
		return false;
	}

	constexpr int cols = 3;
	int rows = CharacterTypeCount / cols;
	auto size = getContentSize();
	auto foo = size.width / 6;
	Vec2 center = getContentSize() / 2;
	auto menu = Menu::create();
	addChild(menu);
	for (int i = 0; i < CharacterTypeCount; ++i) {
		int row = i / cols;
		int col = i % cols;
		
		auto sprite = Sprite::createWithSpriteFrame(SpriteFactory::characterFrame(static_cast<CharacterType>(i)));
		sprite->getTexture()->setAliasTexParameters();
		
		auto item = MenuItemSprite::create(sprite, sprite, [i](auto ref) { Director::getInstance()->replaceScene(FightScene::create(static_cast<CharacterType>(i))); });
		item->setScale(foo * 4 / 5 / sprite->getContentSize().width);
		item->setPosition(Vec2((col - cols / 2) * foo, (0.5f - row)* foo));
		menu->addChild(item);
		
	}

	return true;
}
