#include "GUILayer.h"
#include <algorithm>
#include "GUIBoxCreator.h"
#include "Player.h"
#include "CharacterSpriteFactory.h"
#include "MonsterSpriteFactory.h"
#include "SpriteFactory.h"
#include "TTFLabelBuilder.h"

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
public:
	BottomUICreator(Size size, CharacterType character) :
		size(size),
		leftSize(size.height),
		boxCreator(GUIBoxCreator(GUIFrameColor::Blue, true, 30, size)),
		character(character) {
	}

	Node* create() {
		auto ret = Node::create();
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
		right->setPosition(Vec2(leftSize, 0));

		auto iconSize = size.height / 2;
		boxCreator.size = Size(iconSize, iconSize);
		auto icon = boxCreator.create();
		auto blink = Sprite::create();
		blink->runAction(MonsterSpriteFactory::createMonsterAnimation(ElementalType::LightBlink));
		blink->setPosition(Vec2::ONE * iconSize / 2);
		auto innerIconSize = iconSize - boxCreator.edgeThickness * 2;
		blink->setScale(SpriteFactory::getUnitScale(innerIconSize));
		icon->addChild(blink, 1);

		auto txt = TTFLabelBuilder().setTextSize(30).build("3");
		icon->addChild(txt, 3);
		txt->setPosition(Vec2::ONE * iconSize / 2);

		auto draw = DrawNode::create();

		float cooldown = 10.f;
		
		draw->schedule([draw, time = cooldown, remain = cooldown, size = iconSize, txt](float delta)mutable {
			draw->clear();
			auto degree = 360 - 360 * remain / time;
			auto calcDegree = 450 - degree;
			while (calcDegree > 360) {
				calcDegree -= 360;
			}
			txt->setString(to_string((int)remain));
			auto center = size / 2;
			//y - c = tan(d)(x-c)
			//y = tan(d)(x-c) + c
			//x = (y-c)/tan(d) + c

			vector<Vec2> points;
			
			auto color = Color4F(0, 0, 1, 0.5f);
			Vec2 c(center, center), t(center, size), tl(0, size), tr(size, size), bl(0, 0), br(size, 0), b(center, 0);
			auto tan = ::tan(CC_DEGREES_TO_RADIANS(calcDegree));
			if (45 < calcDegree && calcDegree < 90) {//topright
				draw->drawSolidRect(bl, t, color);
				points = { c, Vec2(center / tan + center,size) ,tr,br,b };
			} else if (calcDegree == 90) {
				points = { tl,tr,br,bl };
			} else if (90 < calcDegree && calcDegree <= 135) {//topleft
				points = { Vec2(center / tan + center,size) ,t,c };
			} else if (135 < calcDegree && calcDegree <= 225) {//left
				Vec2 v(0, center);
				if (calcDegree != 180) {
					v.y = -tan * center + center;
				}
				points = { tl,t,c,v };
			} else if (225 < calcDegree && calcDegree <= 315) {//bottom
				Vec2 v(center, 0);
				if (calcDegree != 270) {
					v.x = -center / tan + center;
				}
				points = { tl, t,c,v,bl };
			} else {//right
				draw->drawSolidRect(bl, t, color);
				Vec2 v(size, center);
				if (calcDegree != 0) {
					v.y = tan * center + center;
				}
				points = { c,v,br,b };
			}

			draw->drawPolygon(points.data(), points.size(), Color4F(0, 0, 1, 0.5f), 0, Color4F::BLACK);

			remain -= delta;
			if (remain < 0) {
				remain += time;
			}
		}, "draw");

		icon->addChild(draw, 2);

		right->addChild(icon);
		return right;
	}
};

void GUILayer::createBottomUI(Size size, CharacterType characterType) {
	addChild(BottomUICreator(size, characterType).create());
}
