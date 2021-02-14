#include "Weapon.h"
#include "WeaponSpriteFactory.h"
#include "GridUnit.h"
#include <unordered_map>

USING_NS_CC;
using namespace std;

constexpr int swingTag = 123;

Weapon * Weapon::create()
{
	auto ret = new Weapon();
	if (!ret || !ret->init()) {
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
	ret->setName("Weapon");
	ret->setAnchorPoint(Vec2(1, 0));


	return ret;
}

bool Weapon::init() {
	if (!Sprite::initWithSpriteFrame(WeaponSpriteFactory::sword())) {
		return false;
	}
	unordered_map<CharacterDirection,Vec2> deltas;
	deltas[CharacterDirection::DOWN] = Vec2(0,-1);
	deltas[CharacterDirection::LEFT] = Vec2(-1, 0);
	deltas[CharacterDirection::RIGHT] = Vec2(1, 0);
	deltas[CharacterDirection::UP] = Vec2(0, 1);

	schedule([this, deltas](float){
		if (!getParent()) {
			return;
		}

		if (getActionByTag(swingTag)) {
			return;
		}

		Vec2 delta;
		if (!target) {
			auto owner = dynamic_cast<GridUnit*>(getParent());
			auto direction = owner->getCurrentDirection();
			delta = deltas.find(direction)->second;
		} else {
			delta = target->getPosition() - getParent()->getPosition();
		}

		float angle = CC_RADIANS_TO_DEGREES(delta.getAngle());
		setRotation(-angle + 45);
		setPosition((delta.getNormalized() + Vec2::ONE)* getContentSize().width / 2);
	}, "followTarget");
}

void Weapon::setTarget(GridUnit* newTarget) {
	if (target) {
		target->release();
	}

	target = newTarget;

	if (!newTarget) {
		return;
	}
	target->retain();
}

void Weapon::swing(float duration){
	runAction(EaseInOut::create(RotateBy::create(duration, Vec3(0, 0, 360)), 2))->setTag(swingTag);
}
