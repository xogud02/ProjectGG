#include "GUILayer.h"
#include <algorithm>
#include "Player.h"
#include "CharacterSpriteFactory.h"
#include "MonsterSpriteFactory.h"
#include "SpriteFactory.h"
#include "TTFLabelBuilder.h"
#include "SkillIconBox.h"
#include "GUISpriteFactory.h"
#include "GridLayer.h"

USING_NS_CC;
using namespace std;

GUILayer* GUILayer::instance = nullptr;

GUILayer * GUILayer::getInstance() {
	return instance;
}

GUILayer::GUILayer() : theme(GUIBoxCreator(GUIFrameColor::Blue, true, 30, Size())){

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

	arrow = Sprite::createWithSpriteFrame(GUISpriteFactory::GUIArrowFrame(ArrowDirection::Up));
	arrow->setAnchorPoint(Vec2(0.5f, 0));
	arrow->setScaleX(7.5f);
	arrow->setScaleY(30);
	addChild(arrow, 1);
	setArrowVisible(false);
	canvas = DrawNode::create();
	if (!canvas) {
		return false;
	}
	addChild(canvas, 2);

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
	theme.size = Size(targetUISize, targetUISize);
	Node* targetUI = theme.create();
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
	theme.size = size;
	auto bottomUI = BottomUICreator(size, theme, characterType).create();
	addChild(bottomUI, 3);
	
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

void GUILayer::setArrowPosition(const Vec2& wFrom, const Vec2& wTo) {
	auto gridLayer = GridLayer::getInstance();
	arrow->setPosition(convertToNodeSpace(wFrom));
	auto delta = wTo - wFrom;
	auto angle = CC_RADIANS_TO_DEGREES(Vec2::angle(delta, Vec2(0, 1)));
	if (delta.x < 0) {
		angle *= -1;
	}
	arrow->setRotation(angle);
}

void GUILayer::setArrowVisible(bool visible) {
	arrow->setVisible(visible);
}

void GUILayer::setArrowTransParent(bool transParent) {
	arrow->setOpacity(transParent ? 64 : 128);
}

SkillIconBox * GUILayer::getSkillIconBox(int i) {
	return skillIconBoxes.at(i);
}

SkillIconBox* GUILayer::getBlinkIconBox() {
	return blinkIconBox;
}

void GUILayer::addBuff(cocos2d::Node *icon, float duration) {
	auto buffIcon = SkillIconBox::create(theme);
	buffIcon->attachIcon(icon);
	addChild(buffIcon, 5);
	buffIcon->setCooldown(duration);
	buffIcon->startCooldown();
	buffIcon->scheduleOnce([buffIcon](float) {buffIcon->removeFromParent(); }, duration, "buff");
}

