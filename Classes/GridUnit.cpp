#include "GridUnit.h"
#include "GaugeBar.h"
#include "GridLayer.h"
#include "GridPathFinder.h"
#include "cocos-ext.h"
#include "GUILayer.h"
#include "TTFLabelBuilder.h"
#include "SpriteFactory.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

const string chaseTarget = "chaseTarget";
unordered_set<GridUnit*> GridUnit::onMove;

#pragma region Class

GridUnit* GridUnit::create(int scale) {
	GridUnit* ret = new GridUnit(scale);
	if (!ret || !ret->init()) {
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
	return ret;
}

bool GridUnit::init() {
	if (!Sprite::init()) {
		return false;
	}

	setAnchorPoint(Vec2::ZERO);
	autorelease();

	setScale(SCALE);
	hpBar = make_shared<GaugeBar>();

	status = Status();

	hpBar->setMaxValue(status.getMaxHP());
	hpBar->setValue(status.getHP());


	GUILayer::getInstance()->addGaugeBar(this, hpBar);

	setName("character");

	schedule([this](float) {
		if (isScheduled(chaseTarget)) {
			return;
		}

		if (currentMoveType == MoveType::Move) {
			return;
		}

		for (auto* n : getParent()->getChildren()) {
			auto* c = dynamic_cast<GridUnit*>(n);
			if (c && c != this && isEnemy(c) && c->currentGridPosition.distance(currentGridPosition) <= status.getNoticeRange()) {
				setTarget(c);
			}
		}
	}, 0.1f, "findOther");

	return true;
}

GridUnit::GridUnit(int scale) :SCALE(scale) {}

GridUnit::~GridUnit() {
	Grid::getInstance()->unOccupyArea(this, currentGridPosition);
}

#pragma endregion

#pragma region Grid

GridPosition GridUnit::getCurrentGridPosition() const {
	auto parent = dynamic_cast<const GridUnit*>(getParent());
	if (parent) {
		return currentGridPosition + parent->getCurrentGridPosition();
	}
	return currentGridPosition;
}

void GridUnit::setGridPosition(GridPosition newGridPosition) {
	currentGridPosition = newGridPosition;
	auto size = GridLayer::getInstance()->UNIT_SIZE;
	Node::setPosition(size * currentGridPosition.col, size * currentGridPosition.row);
}

void GridUnit::setPosition(const Vec2 & v) {
	auto grid = GridLayer::getInstance();
	currentGridPosition = grid->vecToGrid(v);
	Sprite::setPosition(grid->gridToPosition(currentGridPosition));
}

Vec2 GridUnit::getCenturalPosition() {
	return getPosition() + getBoundingBox().size / 2;
}

bool GridUnit::isInTrigger(GridUnit * who) const {
	auto size = who->SCALE;
	auto pos = who->getCurrentGridPosition();
	for (int dr = 0; dr < size; ++dr) {
		for (int dc = 0; dc < size; ++dc) {
			auto mine = getCurrentGridPosition();
			auto test = GridPosition(pos.row + dr, pos.col + dc) - mine;
			auto itr = tileTypes.find(test);
			if (itr != tileTypes.cend() && itr->second == TileType::EventTrigger) {
				return true;
			}
		}
	}
	return false;
}

void GridUnit::testTrigger(GridUnit * who) {
	bool isTriggering = isInTrigger(who);
	bool wasTriggering = triggering.find(who) != triggering.cend();

	if (isTriggering && !wasTriggering) {
		triggering.insert(who);
		onTriggerIn(who);
	}

	if (!isTriggering && wasTriggering) {
		triggering.erase(who);
		onTriggerOut(who);
	}
}

void GridUnit::addTile(GridPosition position, TileType tileType, cocos2d::Sprite * tile) {
	tileTypes[position] = tileType;
	if (!tile) {
		return;
	}

	addChild(tile);
	auto size = SpriteFactory::unitSize.width;
	tile->setPosition(size * position.col, size * position.row);

	auto before = tiles[position];
	if (before) {
		before->removeFromParentAndCleanup(true);
	}
	tile->setAnchorPoint(Vec2::ZERO);
	tiles[position] = tile;
}

void GridUnit::addChild(cocos2d::Node * child) {
	addChild(child, 0);
}

void GridUnit::addChild(cocos2d::Node * child, int z) {
	Node::addChild(child, z);
	auto object = dynamic_cast<GridUnit*>(child);
	if (!object) {
		return;
	}
	auto size = SpriteFactory::unitSize.width;
	auto cPos = object->currentGridPosition;
	object->Sprite::setPosition(size * cPos.col, size * cPos.row);
}

void GridUnit::addMoveListener(GridUnit* object) {
	onMove.insert(object);
}

void GridUnit::removeMoveListener(GridUnit* object) {
	onMove.erase(object);
}

TileTypeMap GridUnit::getTileTypes() const {
	return tileTypes;
}

void GridUnit::removeFromParentAndCleanup(bool cleanup) {
	Grid::getInstance()->unOccupyArea(this, currentGridPosition);
	Node::removeFromParentAndCleanup(cleanup);
}

CharacterDirection GridUnit::getCurrentDirection() {
	return currentDirection;
}

#pragma endregion

#pragma region Unit

ChracterCondition GridUnit::getCondition() const{
	return status.getCondition();
}

bool GridUnit::isEnemy(GridUnit *c) {
	return team != c->team;
}

void GridUnit::setTarget(GridUnit * newTarget) {
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

GridUnit * GridUnit::getTarget() {
	return target;
}

void GridUnit::setMoveType(MoveType moveType) {
	currentMoveType = moveType;
}

bool GridUnit::isInAttackRange(GridUnit * who) const {
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

void GridUnit::hit(GridUnit* by, int damage) {

	auto text = TTFLabelBuilder()
		.setTextSize(20)
		.build(to_string(damage));
	
	auto guiLayer = GUILayer::getInstance();
	auto wCharacterPosition = getParent()->convertToWorldSpace(getPosition() + getBoundingBox().size / 2);
	auto textPosition = guiLayer->convertToNodeSpace(wCharacterPosition);
	text->setPosition(textPosition);

	guiLayer->addChild(text, 1);
	text->runAction(Sequence::create(
		MoveBy::create(0.5f, Vec2(0, 30)),
		RemoveSelf::create(),
		nullptr
	));

	status.reduceHP(damage);
	hpBar->setValue(status.getHP());
	if (getCondition() == ChracterCondition::Dead) {
		removeFromParent();
	}
}

void GridUnit::buff(int power, float time, Node* icon) {
	++buffCount;
	status.addMorePower(power);
	CCLOG("power up");
	scheduleOnce(
		[power, this]
	(float) {
		status.addMorePower(-power);
		--buffCount;
		CCLOG("power up done");
	}, time, "buff" + to_string(buffCount));
}

void GridUnit::releaseTarget() {
	if (!target) {
		return;
	}
	target->release();
	target = nullptr;
	if (isScheduled(chaseTarget)) {
		unschedule(chaseTarget);
	}
}

void GridUnit::onAttackBegin() {}

CharacterDirection GridUnit::getNextDirection(GridPosition nextPosition) {
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

void GridUnit::onMoveBegin(GridPosition nextPosition, CharacterDirection nextDirection) {}

constexpr int movingActionTag = 1;

void GridUnit::movePath(float) {
	if (path.empty()) {
		setMoveType(MoveType::Stop);
		return;
	}
	auto next = path.front();

	auto grid = Grid::getInstance();
	grid->unOccupyArea(this, currentGridPosition);
	if (!grid->isOccupiable(next, SCALE)) {
		grid->occupyArea(this, currentGridPosition);
		scheduleOnce(CC_SCHEDULE_SELECTOR(GridUnit::movePath), 1);
		return;
	}

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

	grid->occupyArea(this, currentGridPosition);

	auto afterMove = [this]() {
		setPosition(GridLayer::getInstance()->gridToPosition(currentGridPosition));

		for (const auto& p : GridUnit::onMove) {
			p->testTrigger(this);
		}
		scheduleOnce(CC_SCHEDULE_SELECTOR(GridUnit::movePath), 0);
	};

	runAction(Sequence::create(
		moveTo,
		CallFunc::create(afterMove),
		nullptr))->setTag(movingActionTag);
}

void GridUnit::tryToMove(GridPosition position) {
	if (position == currentGridPosition) {
		return;
	}

	setMoveType(MoveType::Move);

	auto grid = Grid::getInstance();
	grid->unOccupyArea(this, currentGridPosition);
	auto newPath = GridPathFinder().findPath(currentGridPosition, position, SCALE);
	if (!newPath.empty()) {
		newPath.pop();//remove start position
	} else {
	}
	grid->occupyArea(this, currentGridPosition);
	path.swap(newPath);


	if (!isScheduled(CC_SCHEDULE_SELECTOR(GridUnit::movePath))) {
		scheduleOnce(CC_SCHEDULE_SELECTOR(GridUnit::movePath), 0);
	}
}

bool GridUnit::tryToJump(GridPosition position) {
	auto grid = Grid::getInstance();

	if (position == currentGridPosition || !grid->isOccupiable(position, SCALE) || !grid->isMovableTile(position, SCALE)) {
		return false;
	}

	stopMove();
	stopAction(getActionByTag(movingActionTag));

	grid->unOccupyArea(this, currentGridPosition);
	setPosition(GridLayer::getInstance()->gridToPosition(position));
	grid->occupyArea(this, position);
	return true;
}

void GridUnit::stopMove() {
	path.swap(queue<GridPosition>());
}

#pragma endregion
