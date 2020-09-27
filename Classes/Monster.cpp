#include "Monster.h"
#include "SpriteFactory.h"
#include "GaugeBar.h"

USING_NS_CC;

Monster::Monster(int scale) : Character(scale) {
}

bool Monster::init() {
	if (!Character::init()) {
		return false;
	}
	setName("monster");
	team = 1;
	status = Status(10, 10, 1, 0);

	hpBar->setMaxValue(status.getMaxHP());
	hpBar->setValue(status.getHP());


	runAction(SpriteFactory::slime());
	schedule([this](float) {
		if (currentMoveType != MoveType::Hold && !target) {
			tryToMove(currentGridPosition + GridPosition(random(-1, 1), random(-1, 1)));
		}
	},1.0f, "move");
	return true;
}

Monster * Monster::create(float scale) {
	Monster* ret = new Monster(scale);
	if (!ret || !ret->init()) {
		CC_SAFE_DELETE(ret);
		return nullptr;
	}

	return ret;
}

