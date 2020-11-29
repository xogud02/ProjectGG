#include "GUILayer.h"
#include <algorithm>
#include "GUIBoxCreator.h"
#include "Player.h"
#include "CharacterSpriteFactory.h"
#include "MonsterSpriteFactory.h"
#include "SpriteFactory.h"
#include "TTFLabelBuilder.h"
#include "SkillIconBox.h"

USING_NS_CC;
using namespace std;

GUILayer* GUILayer::instance = nullptr;

GUILayer * GUILayer::getInstance() {
	return instance;
}

void removeDied(CharacterBarMap& cb) {
	auto pred = [](auto& p) {return p.first->getCondition() == ChracterCondition::Dead; };
	auto itr = find_if(cb.begin(), cb.end(), pred);
	while (itr != cb.end()) {
		itr->first->release();
		itr = cb.erase(itr);
		itr = find_if(itr, cb.end(), pred);
	}
}

bool GUILayer::init() {
	if (!Node::init()) {
		return false;
	}

	canvas = DrawNode::create();
	if (!canvas) {
		return false;
	}
	addChild(canvas);

	instance = this;

	schedule([this](float) mutable {

		removeDied(characterBarMap);
		canvas->clear();
		for (const auto& p : characterBarMap) {
			auto owner = p.first;
			auto pBar = p.second;
			auto characterWorldPosition = owner->getParent()->convertToWorldSpace(owner->getPosition());
			auto drawPosition = convertToNodeSpace(characterWorldPosition);
			auto cSize = owner->getBoundingBox().size;
			drawPosition.y += cSize.height;
			pBar->drawBar(canvas, drawPosition, Size(cSize.width, cSize.height / 6));
		}
	}, "positionBars");
	return true;
}

void GUILayer::addGaugeBar(Character* owner, pBar newBar) {
	if (characterBarMap.find(owner) == characterBarMap.cend()) {
		owner->retain();
	}
	characterBarMap[owner] = newBar;
}

void GUILayer::setPlayer(Character * player) {
	auto size = Director::getInstance()->getWinSize();
	float targetUISize = size.height / 7;
	Node* targetUI = GUIBoxCreator(GUIFrameColor::Blue, true, 30, Size(targetUISize, targetUISize)).create();
	targetUI->setPosition(Vec2(0, size.height - targetUISize));
	targetUI->setVisible(false);
	addChild(targetUI);
	Sprite* thumbnail = Sprite::create();
	targetUI->addChild(thumbnail);
	schedule([player, currentTarget = player->getTarget(), targetUI, thumbnail = thumbnail](float) mutable{
		auto newTarget = player->getTarget();
		if (newTarget == currentTarget) {
			return;
		}

		currentTarget = newTarget;

		if (!currentTarget) {
			targetUI->setVisible(false);
			return;
		}

		targetUI->setVisible(true);
		targetUI->removeChild(thumbnail);

		thumbnail = Sprite::createWithSpriteFrame(newTarget->getSpriteFrame());

		auto size = targetUI->getContentSize();

		targetUI->addChild(thumbnail, 500);
		thumbnail->setPosition(size / 2);
		thumbnail->setScale(size.width * 0.9f / thumbnail->getContentSize().width);
	}, "observePlayer");
}
class BottomUICreator {
	const Size size;
	const float leftSize;
	GUIBoxCreator boxCreator;
	const CharacterType character;
	Node* ret;
public:
	BottomUICreator(Size size,GUIBoxCreator boxCreator, CharacterType character) :
		size(size),
		leftSize(size.height),
		boxCreator(boxCreator),
		character(character)
	{}

	Node* create() {
		ret = Node::create();
		ret->addChild(createLeftBox(), 1);
		ret->addChild(createRightBox(), 1);
		return ret;
	}

private:
	Node* createLeftBox() {
		boxCreator.size = Size(leftSize, leftSize);
		auto left = boxCreator.create();
		left->addChild(createThumbnail());
		return left;
	}

	Sprite* createThumbnail() {
		auto thumbNail = Sprite::createWithSpriteFrame(CharacterSpriteFactory::characterFrame(character));
		thumbNail->setPosition(Vec2::ONE*leftSize / 2);
		auto targetSize = leftSize - boxCreator.edgeThickness * 2;
		thumbNail->setScale(SpriteFactory::getUnitScale(targetSize));
		return thumbNail;
	}

	Node* createRightBox() {
		boxCreator.size = Size(size.width - leftSize, size.height);
		auto right = boxCreator.create();
		right->setName("right");
		right->setPosition(Vec2(leftSize, 0));
		return right;
	}
};

void GUILayer::createBottomUI(Size size, CharacterType characterType) {
	auto theme = GUIBoxCreator(GUIFrameColor::Blue, true, 30, size);
	auto bottomUI = BottomUICreator(size, theme, characterType).create();
	addChild(bottomUI);
	
	auto iconSize = size.height / 2;
	theme.size = Size(iconSize, iconSize);
	blinkIconBox = SkillIconBox::create(theme);
	auto rightBox = bottomUI->getChildByName("right");
	rightBox->addChild(blinkIconBox);
	for (int i = 0; i < 4; ++i) {
		auto skillIconBox = SkillIconBox::create(theme);
		skillIconBoxes.pushBack(skillIconBox);
		rightBox->addChild(skillIconBox);
		skillIconBox->setPosition(Vec2(2 + i, 1) * skillIconBox->getContentSize().width);
	}
}

SkillIconBox * GUILayer::getSkillIconBox(int i) {
	return skillIconBoxes.at(i);
}

SkillIconBox* GUILayer::getBlinkIconBox() {
	return blinkIconBox;
}

