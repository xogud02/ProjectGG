#include "TitleScene.h"
#include "FightScene.h"
#include "CharacterSelectScene.h"

USING_NS_CC;

bool TitleScene::init() {
	if (!Scene::init()) {
		return false;
	}

	auto title = Label::create();
	title->setString("ProjectGG");
	const auto sceneSize = getContentSize();
	auto scaler = (sceneSize.height * 1 / 5) / title->getContentSize().height;
	title->setSystemFontSize(title->getSystemFontSize()*scaler);
	title->setPosition(getContentSize() / 2);
	addChild(title);
	
	auto tapToBegin = Label::create();
	tapToBegin->setString("Tap to Begin");
	tapToBegin->runAction(RepeatForever::create(Blink::create(1, 1)));
	tapToBegin->setSystemFontSize(title->getSystemFontSize() / 5);
	tapToBegin->setPosition(Vec2(sceneSize.width / 2, sceneSize.height / 5));
	addChild(tapToBegin);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [](auto, auto) {
		//Director::getInstance()->replaceScene(FightScene::create());
		Director::getInstance()->replaceScene(CharacterSelectScene::create());
		return true; 
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}
