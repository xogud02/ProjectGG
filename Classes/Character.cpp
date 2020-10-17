#include "Character.h"
#include "GaugeBar.h"
#include "GridLayer.h"
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
	hpBar = GaugeBar::create(Size(size.width, size.height / 6));

	status = Status();

	hpBar->setMaxValue(status.getMaxHP());
	hpBar->setValue(status.getHP());


	float width = size.width;
	hpBar->setAnchorPoint(Vec2::ONE / 2.f);
	hpBar->setPosition(Vec2(width / 2, 0));
	//addChild(hpBar, 1);

	setName("character");

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [this](Touch* e, auto) {

		auto grid = GridLayer::getInstance();
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

		if (currentMoveType == MoveType::Move) {
			return;
		}

		for (auto* n : getParent()->getChildren()) {
			auto* c = dynamic_cast<Character*>(n);
			if (c && c != this && isEnemy(c) && c->currentGridPosition.distance(currentGridPosition) <= status.getNoticeRange()) {
				setTarget(c);
			}
		}
	}, 0.1f, "findOther");

	return true;
}


Character::Character(int scale) :SCALE(scale) {}

void Character::removeFromParentAndCleanup(bool cleanup) {
	Grid::getInstance()->occupyArea(currentGridPosition, SCALE, false);
	Node::removeFromParentAndCleanup(cleanup);
}

ChracterCondition Character::getCondition() {
	return status.getCondition();
}

CharacterDirection Character::getCurrentDirection() {
	return currentDirection;
}

bool Character::isEnemy(Character *c) {
	return team != c->team;
}



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
	schedule([lastPos = currentGridPosition, this](float) mutable {
		if (status.isAttackReady() && isInAttackRange(target)) {
			onAttackBegin();
			target->hit(this, status.getDamage());
			if (target->getCondition() == ChracterCondition::Dead) {
				status.levelUp();
				hpBar->setValue(status.getHP());
				hpBar->setMaxValue(status.getMaxHP());
				releaseTarget();
			}
			status.waitForAttack(this);
			return;
		}
		if (lastPos == target->currentGridPosition) {
			return;
		}
		lastPos = target->currentGridPosition;

		if (currentMoveType == MoveType::Hold) {
			return;
		}
		tryToMove(lastPos);

	}, 0, chaseTarget);
}

Character * Character::getTarget() {
	return target;
}

void Character::setMoveType(MoveType moveType) {
	currentMoveType = moveType;
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

void Character::setPosition(const Vec2 & v) {
	auto grid = GridLayer::getInstance();
	currentGridPosition = grid->vecToGrid(v);
	Sprite::setPosition(grid->gridToPosition(currentGridPosition));
}

void Character::hit(Character* by, int damage) {
	CCLOG("Character::hit() - %s hit by %s, damage %d", getName().c_str(), by->getName().c_str(), damage);
	status.reduceHP(damage);
	hpBar->setValue(status.getHP());
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
	Grid::getInstance()->occupyArea(currentGridPosition, SCALE, false);
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

void Character::onAttackBegin() {}

CharacterDirection Character::getNextDirection(GridPosition nextPosition) {
	CCAssert(currentGridPosition != nextPosition, "invalid parameter");
	auto delta = nextPosition - currentGridPosition;

	bool downWard = delta.row < 0, upWard = delta.row > 0;
	bool leftWard = delta.col < 0, rightWard = delta.col > 0;

	auto abr = abs(delta.row), abc = abs(delta.col);
	if (abr > abc) {
		if (upWard) {
			return CharacterDirection::UP;
		}
		return CharacterDirection::DOWN;
	}

	if (abr < abc) {
		if (rightWard) {
			return CharacterDirection::RIGHT;
		}
		return CharacterDirection::LEFT;
	}
	bool currentDown = currentDirection == CharacterDirection::DOWN;
	bool currentUp = currentDirection == CharacterDirection::UP;
	bool currentLeft = currentDirection == CharacterDirection::LEFT;
	bool currentRight = currentDirection == CharacterDirection::RIGHT;
	if ((currentDown && downWard) ||(currentUp && upWard) ||
		(currentLeft && leftWard) ||(currentRight && rightWard)) {
		return currentDirection;
	}

	bool currentVirtical = currentDown || currentUp;
	bool currentHorizontal = currentLeft || currentRight;
	if (currentVirtical && leftWard) {
		return CharacterDirection::LEFT;
	}
	if (currentVirtical && rightWard) {
		return CharacterDirection::RIGHT;
	}
	if (currentHorizontal && upWard) {
		return CharacterDirection::UP;
	}

	//if (currentHorizontal && downWard)
	return CharacterDirection::DOWN;
}

void Character::onMoveBegin(GridPosition nextPosition, CharacterDirection nextDirection) {}

void Character::movePath(float) {
	if (path.empty()) {
		return;
	}
	auto next = path.front();

	auto grid = Grid::getInstance();
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
	auto gridLayer = GridLayer::getInstance();
	auto delta = gridLayer->gridToPosition(next) - gridLayer->gridToPosition(currentGridPosition);
	auto moveTo = MoveTo::create(delta.length() / static_cast<float>(status.getSpeed() * gridLayer->UNIT_SIZE), gridLayer->gridToPosition(next));

	auto nextDirection = getNextDirection(next);
	onMoveBegin(next, nextDirection);

	currentGridPosition = next;
	currentDirection = nextDirection;

	grid->occupyArea(currentGridPosition, SCALE);


	runAction(Sequence::create(
		moveTo,
		CallFunc::create([this]() {
		setPosition(GridLayer::getInstance()->gridToPosition(currentGridPosition));
		scheduleOnce(CC_SCHEDULE_SELECTOR(Character::movePath), 0);
	}), nullptr))->setTag(movingActionTag);
}

void Character::tryToMove(GridPosition position) {
	auto grid = Grid::getInstance();
	grid->occupyArea(currentGridPosition, SCALE, false);
	auto newPath = GridPathFinder().findPath(currentGridPosition, position, SCALE);
	if (!newPath.empty()) {
		newPath.pop();//remove start position
	} else {
	}
	grid->occupyArea(currentGridPosition, SCALE);
	path.swap(newPath);


	if (!isScheduled(CC_SCHEDULE_SELECTOR(Character::movePath))) {
		scheduleOnce(CC_SCHEDULE_SELECTOR(Character::movePath), 0);
	}
}
