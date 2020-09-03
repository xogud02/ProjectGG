#include "Character.h"
#include "HpBar.h"
#include "Grid.h"
#include "GridPathFinder.h"
#include "SpriteFactory.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

const string chaseTarget = "chaseTarget";

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

	schedule([this](float) {
		if (isScheduled(chaseTarget)) {
			return;
		}

		for (auto* n : getParent()->getChildren()) {
			auto* c = dynamic_cast<Character*>(n);
			if (c && c != this && c->currentGridPosition.distance(currentGridPosition) <= noticeRange) {
				setTarget(c);
			}
		}
	}, 0.1f, "findOther");

	return true;
}


Character::Character(int scale) :SCALE(scale) {}



void Character::setTarget(Character * newTarget) {
	if (target == newTarget) {
		return;
	}

	if (!newTarget) {
		releaseTarget();
		return;
	}
	target = newTarget;
	target->retain();
	schedule([lastPos = currentGridPosition, this, attackReady = true](float) mutable {
		if (attackReady) {
			AttackResult result = attack(target);
			if (result == AttackResult::Die) {
				releaseTarget();
				return;
			}
			attackReady = false;
			scheduleOnce([&attackReady](float) {attackReady = true; }, attackInterval, "waitForAttack");
			return;
		}
		if (lastPos == target->currentGridPosition) {
			return;
		}
		lastPos = target->currentGridPosition;
		tryToMove(lastPos);

	}, 0, chaseTarget);
}

bool Character::isInAttackRange(Character * who) const {
	if (!who) {
		return false;
	}
	const int minR = currentGridPosition.row - who->SCALE;
	const int minC = currentGridPosition.col - who->SCALE;
	const int maxR = currentGridPosition.row + SCALE;
	const int maxC = currentGridPosition.col + SCALE;
	const int targetRow = who->currentGridPosition.row;
	const int targetCol = who->currentGridPosition.col;
	return minR <= targetRow && targetRow <= maxR && minC <= targetCol && targetCol <= maxC;
}

AttackResult Character::attack(Character * c) {
	if (!c || c == this || !isInAttackRange(c)) {
		return AttackResult::None;
	}
	c->hit(status.getDamage());
	if (c->status.getHP() <= 0) {
		return AttackResult::Die;
	}
	return AttackResult::Normal;
}

void Character::setPosition(const Vec2 & v) {
	auto grid = getGrid();
	if (!grid) {
		Sprite::setPosition(v);
		return;
	}
	if (grid) {
		currentGridPosition = grid->vecToGrid(v);
		Sprite::setPosition(grid->gridToPosition(currentGridPosition));
	}
}

void Character::hit(int damage) {
	auto grid = getGrid();
	status.reduceHP(damage);
	hpBar->setHP(static_cast<float>(status.getHP()) / status.getMaxHP());
	if (status.getHP() <= 0) {
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

void Character::releaseTarget() {
	if (!target) {
		return;
	}
	target->release();
	target = nullptr;	
	if (isScheduled(chaseTarget)) {
		unschedule(chaseTarget);
	}
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

	path.pop();
	auto delta = grid->gridToPosition(next) - grid->gridToPosition(currentGridPosition);
	auto moveTo = MoveTo::create(delta.length() / static_cast<float>(speed * grid->UNIT_SIZE), grid->gridToPosition(next));
	currentGridPosition = next;
	grid->occupyArea(currentGridPosition, SCALE);


	onMoveBegin(next);
	runAction(Sequence::create(
		moveTo,
		CallFunc::create([this]() {
		setPosition(getGrid()->gridToPosition(currentGridPosition));
		scheduleOnce(CC_SCHEDULE_SELECTOR(Character::movePath), 0);
	}), nullptr))->setTag(movingActionTag);
}

void Character::tryToMove(GridPosition position) {
	auto newPath = GridPathFinder().findPath(getGrid(), currentGridPosition, position, SCALE);
	if (!newPath.empty()) {
		newPath.pop();//remove start position
	}
	path.swap(newPath);


	if (!isScheduled(CC_SCHEDULE_SELECTOR(Character::movePath))) {
		scheduleOnce(CC_SCHEDULE_SELECTOR(Character::movePath), 0);
	}
}
