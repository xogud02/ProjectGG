#include "Character.h"
#include "HpBar.h"
#include "Grid.h"
#include "GridPathFinder.h"
#include "SpriteFactory.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

Character* Character::create(int scale) {
	Character* ret = new Character(scale);
	if (!ret || !ret->init()) {
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
	return ret;
}


bool Character::init() {
	if (!Sprite::init()) {
		return false;
	}

	setAnchorPoint(Vec2::ZERO);
	autorelease();

	Size size(SpriteFactory::getUnitSizeInPoints());
	setScale(SCALE);
	hpBar = HPBar::create(Size(size.width, size.height / 6));

	float width = size.width;
	hpBar->setContentSize(Size(width, width / 6));
	hpBar->setHP(1);
	hpBar->setAnchorPoint(Vec2::ONE / 2.f);
	hpBar->setPosition(Vec2(width / 2, 0));
	addChild(hpBar, 1);

	setName("character");

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [this](Touch* e, auto) {

		auto grid = getGrid();
		if (!getBoundingBox().containsPoint(grid->convertToNodeSpace(e->getLocation()))) {
			return false;
		}

		grid->touched(this);
		return true;
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}


Character::Character(int scale) :SCALE(scale) {}

void Character::setTarget(Character * target) {
	if (!target) {
		return;
	}
	const string chaseTarget = "chaseTarget";
	schedule([target, lastPos = currentGridPosition, this, chaseTarget] (float) mutable {
		if (!target) {
			unschedule(chaseTarget);
			return;
		}
		if (lastPos == target->currentGridPosition) {
			return;
		}
		lastPos = target->currentGridPosition;
		tryToMove(lastPos);
	}, 0, chaseTarget);
}

void Character::hit(int damage) {
	auto grid = getGrid();
	hp -= damage;
	hpBar->setHP(static_cast<float>(hp) / maxHP);
	if (hp <= 0) {
		removeFromParent();
		return;
	}
	auto text = Label::create();
	text->setString(to_string(damage));
	addChild(text, 1);
	text->runAction(Sequence::create(
		MoveBy::create(0.5f, Vec2(0, 30)),
		RemoveSelf::create(),
		nullptr
	));
}

Character::~Character() {
	getGrid()->occupyArea(currentGridPosition, SCALE, false);
}

Grid * Character::getGrid() {
	if (!grid) {
		grid = dynamic_cast<Grid*>(getParent());
	}
	return grid;
}

void Character::onMoveBegin(GridPosition nextPosition) {}

void Character::movePath(float) {
	if (path.empty()) {
		return;
	}
	auto next = path.front();

	auto grid = getGrid();
	grid->occupyArea(currentGridPosition, SCALE, false);
	if (grid->isOccupied(next, SCALE)) {
		grid->occupyArea(currentGridPosition, SCALE);
		scheduleOnce(CC_SCHEDULE_SELECTOR(Character::movePath), 1);
		return;
	}

	const int movingActionTag = 1;
	if (getActionByTag(movingActionTag)) {
		return;
	}

	onMoveBegin(next);
	path.pop();
	auto delta = grid->gridToPosition(next) - grid->gridToPosition(currentGridPosition);
	auto moveTo = MoveTo::create(delta.length() / static_cast<float>(speed * grid->UNIT_SIZE), grid->gridToPosition(next));
	currentGridPosition = next;
	grid->occupyArea(currentGridPosition, SCALE);


	runAction(Sequence::create(
		moveTo,
		CallFunc::create([this]() {
		setPosition(getGrid()->gridToPosition(currentGridPosition));
		scheduleOnce(CC_SCHEDULE_SELECTOR(Character::movePath), 0);
	}), nullptr))->setTag(movingActionTag);
}

void Character::tryToMove(GridPosition position) {
	auto newPath = GridPathFinder().findPath(getGrid(), currentGridPosition, position, SCALE);
	path.swap(newPath);
	if (!isScheduled(CC_SCHEDULE_SELECTOR(Character::movePath))) {
		scheduleOnce(CC_SCHEDULE_SELECTOR(Character::movePath), 0);
	}
}
