#include "TitleScene.h"
#include "MenuScene.h"
#include "TTFLabelBuilder.h"

USING_NS_CC;

bool TitleScene::init() {
	if (!Scene::init()) {
		return false;
	}

	const auto sceneSize = getContentSize();

	auto title = TTFLabelBuilder()
		.setBold(true)
		.setTextSize((sceneSize.height * 1 / 6))
		.build("ProjectGG");
	title->setPosition(getContentSize() / 2);
	addChild(title);
	
	auto tapToBegin = TTFLabelBuilder()
		.setTextSize(title->getTTFConfig().fontSize / 5)
		.build("Tap to Begin");
	tapToBegin->runAction(RepeatForever::create(Blink::create(1, 1)));
	tapToBegin->setPosition(Vec2(sceneSize.width / 2, sceneSize.height / 5));
	addChild(tapToBegin);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [](auto, auto) {
		Director::getInstance()->replaceScene(MenuScene::create());
		return true; 
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}
