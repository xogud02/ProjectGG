#include "Monster.h"
#include "MonsterSpriteFactory.h"
#include "GaugeBar.h"
#include "MonsterEnums.h"

USING_NS_CC;

Monster::Monster(int scale) : Unit(scale) {
}

bool Monster::init() {
	if (!Unit::init()) {
		return false;
	}
	setName("monster");
	team = 1;
	status = Status(10, 10, 1, 0);

	hpBar->setMaxValue(status.getMaxHP());
	hpBar->setValue(status.getHP());


	runAction(MonsterSpriteFactory::createMonsterAnimation(FishType::BrownWhale));
	schedule([this](float) {
		if (currentMoveType != MoveType::Hold && !target) {
			tryToMove(currentGridPosition + GridPosition(random(-1, 1), random(-1, 1)));
		}
	},1.0f, "move");
	return true;
}

void Monster::onMoveBegin(GridPosition nextPosition, CharacterDirection nextDirection) {
	auto delta = nextPosition - currentGridPosition;
	if (delta.col < 0 && !isFlippedX()) {
		setFlippedX(true);
		return;
	}

	if (delta.col > 0 && isFlippedX()) {
		setFlippedX(false);
		return;
	}
}

Monster * Monster::create(float scale) {
	Monster* ret = new Monster(scale);
	if (!ret || !ret->init()) {
		CC_SAFE_DELETE(ret);
		return nullptr;
	}

	return ret;
}

