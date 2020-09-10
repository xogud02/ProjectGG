#include "StageSelectScene.h"
#include "ui\CocosGUI.h"
#include "CharacterSelectScene.h"
#include "SpriteFactory.h"

USING_NS_CC;
using namespace ui;
using namespace std;

constexpr int minStage = 1;
constexpr int maxStage = 99;

int clampStage(int value) {
	if (value < minStage) {
		return minStage;
	}

	if (value > maxStage) {
		return maxStage;
	}

	return value;
}

class StageTextField : public TextField {
	int currentStage = 1;
public:
	CREATE_FUNC(StageTextField);
	virtual bool init() {
		if (!TextField::init()) {
			return false;
		}
		setValue(1);
		addEventListener([this](auto, auto) mutable{
			try {
				int tmp = stoi(getString());
				currentStage = clampStage(tmp);
			} catch (...) {
				auto currentText = getString();
				CCLOG("StageSelectScene::value must be digits %d~%d (current value : %s)", minStage, maxStage, currentText.c_str());
				if (currentText.length() == 0) {
					currentStage = minStage;
				}
			}
			setValue(currentStage);
		});

		return true;
	}
	void setValue(int i) {
		i = clampStage(i);
		setString(to_string(i));
	}
};

MenuItemSprite* createArrowItem(ArrowDirection direction, int delta, StageTextField* field) {
	auto sprite = Sprite::createWithSpriteFrame(SpriteFactory::GUIArrowFrame(direction));
	auto ret = MenuItemSprite::create(sprite, sprite, [field, delta](auto) {
		auto value = stoi(field->getString());
		value += delta;
		value = clampStage(value);
		field->setString(to_string(value));
	});
	ret->setScale(5);
	return ret;
}

bool StageSelectScene::init() {
	if (!Scene::init()) {
		return false;
	}


	auto size = getContentSize();
	auto field = StageTextField::create();
	field->setFontSize(50);

	field->setPosition(size / 2);
	addChild(field);

	auto left = createArrowItem(ArrowDirection::Left, -1, field);
	left->setPosition(Vec2(-size.width / 4, 0));

	auto right = createArrowItem(ArrowDirection::Right, 1, field);
	right->setPosition(Vec2(size.width / 4, 0));

	auto menu = Menu::create();

	menu->addChild(left);
	menu->addChild(right);
	addChild(menu);

	auto sSelect = Sprite::createWithSpriteFrame(SpriteFactory::GUIGreenButton());
	auto select = MenuItemSprite::create(sSelect, sSelect, [field](auto) {
		auto value = stoi(field->getString());//TODO do something with this value
		Director::getInstance()->replaceScene(CharacterSelectScene::create());
	});
	menu->addChild(select);
	select->setScale(5);
	select->setPosition(Vec2(0, -size.height / 4));

	return true;
}
