#include "Portal.h"
#include "ObjectSpriteFactory.h"
#include "CharacterSelectScene.h"
#include "SpriteTileEnums.h"

USING_NS_CC;

Portal* Portal::create() {
	auto ret = new Portal();
	if (!ret->init()) {
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
	auto portalSprite = Sprite::create();
	portalSprite->runAction(ObjectSpriteFactory::portal());
	portalSprite->setAnchorPoint(Vec2::ZERO);
	//ret->addTile(GridPosition(), TileType::EventTrigger, portalSprite);
	return ret;
}

void Portal::onTriggerIn(Unit * who) {
	Director::getInstance()->replaceScene(CharacterSelectScene::create(SpriteTileTheme::Bright));//TODO implement various portal
}
