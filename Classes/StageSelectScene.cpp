#include "StageSelectScene.h"
#include "ui\CocosGUI.h"
#include "CharacterSelectScene.h"
#include "SpriteFactory.h"

USING_NS_CC;
using namespace ui;
using namespace std;



class IndexRotator {
public:
	virtual void previous() = 0;
	virtual void next() = 0;
};

class StageTextField : public TextField, public IndexRotator {
	const int minStage = 1;
	const int maxStage = 99;

	int clampStage(int value) {
		if (value < minStage) {
			return minStage;
		}

		if (value > maxStage) {
			return maxStage;
		}

		return value;
	}

	int currentStage = 1;
public:
	CREATE_FUNC(StageTextField);
	virtual bool init() override {
		if (!TextField::init()) {
			return false;
		}

		setValue(1);
		addEventListener([this](auto, auto) mutable {
			try {
				int tmp = stoi(getString());
				currentStage = tmp;
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
		currentStage = clampStage(i);
		setString(to_string(currentStage));
	}

	virtual void previous() override {
		setValue(currentStage - 1);
	}
	virtual void next() override {
		setValue(currentStage + 1);
	}
};

class StageThemeLabel : public Label, public IndexRotator {
	int index = 0;
	vector<string> texts;
public:
	CREATE_FUNC(StageThemeLabel);

	virtual bool init() override {
		setString(texts[0]);
		return true;
	}

	StageThemeLabel() {
		texts = { "Easy","Normal","Hard","Hell" };
	}

	virtual void previous() override {
		index += texts.size() - 1;
		index %= texts.size();
		setString(texts[index]);
	}

	virtual void next() override {
		++index;
		index %= texts.size();
		setString(texts[index]);
	}

	SpriteTileTheme getItem() {
		return static_cast<SpriteTileTheme>(index);
	}

};

MenuItemSprite* createArrowItem(ArrowDirection direction, const ccMenuCallback& callback) {
	auto sprite = Sprite::createWithSpriteFrame(SpriteFactory::GUIArrowFrame(direction));
	auto ret = MenuItemSprite::create(sprite, sprite, callback);
	ret->setScale(5);
	return ret;
}

void attatchArrows(Menu* menu, IndexRotator* ir,float xOffset, float yOffset) {
	auto left = createArrowItem(ArrowDirection::Left, [ir](auto) {ir->previous(); });
	left->setPosition(Vec2(-xOffset, yOffset));

	auto right = createArrowItem(ArrowDirection::Right, [ir](auto) {ir->next(); });
	right->setPosition(Vec2(xOffset, yOffset));
	menu->addChild(left);
	menu->addChild(right);
}

bool StageSelectScene::init() {
	if (!Scene::init()) {
		return false;
	}

	auto size = getContentSize();
	auto field = StageTextField::create();
	field->setFontSize(50);

	field->setPosition(Vec2(size.width / 2, size.height / 2));
	addChild(field);

	auto menu = Menu::create();

	attatchArrows(menu, field, size.width / 8, 0);
	addChild(menu);

	auto label = StageThemeLabel::create();
	label->setPosition(Vec2(size.width / 2, size.height * 3 / 8));
	label->setSystemFontSize(50);
	addChild(label);
	attatchArrows(menu, label, size.width / 8, -size.height / 8);


	auto sSelect = Sprite::createWithSpriteFrame(SpriteFactory::GUIFrame(GUIFrameColor::Green, GUIFramePart::Single, true));
	auto select = MenuItemSprite::create(sSelect, sSelect, [field, label](auto) {
		auto value = stoi(field->getString());//TODO do something with this value
		Director::getInstance()->replaceScene(CharacterSelectScene::create(label->getItem()));
	});
	menu->addChild(select);
	select->setScale(5);
	select->setPosition(Vec2(0, -size.height / 4));

	return true;
}
