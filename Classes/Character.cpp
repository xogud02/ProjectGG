#include "Character.h"
#include "HpBar.h"
#include "Grid.h"
#include "GridPathFinder.h"
#include "SpriteFactory.h"

USING_NS_CC;
using namespace std;

Character* Character::create(float unitSize, int scale) {
	Character* ret = new Character(scale);
	if (!ret || !ret->initCharacter(unitSize)) {
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
	return ret;
}


bool Character::initCharacter(float unitSize)
{
	setAnchorPoint(Vec2::ZERO);
	autorelease();

	float characterSize = unitSize * SCALE;

	setScale(SpriteFactory::getUnitScale(characterSize));
	HPBar* hpBar = HPBar::createWithColor(Color3B::GREEN);

	Size size = getContentSize();
	float width = size.width;
	hpBar->setContentSize(Size(width, width / 6));
	hpBar->setHP(1);
	hpBar->setPosition(Vec2(width / 2, 0));
	addChild(hpBar);

	auto draw = DrawNode::create();
	draw->drawRect(Vec2::ZERO, Vec2(size.width, size.height), Color4F::RED);
	addChild(draw);

	return true;
}


Character::Character(int scale) :SCALE(scale)
{
}

Character::~Character()
{

}

Grid * Character::getGrid()
{
	if (!grid) {
		grid = dynamic_cast<Grid*>(getParent());
	}
	return grid;
}

void Character::move(float dt) {
	auto currentPos = getPosition();
	auto direction = (nextPos - currentPos);
	auto normalized = direction.getNormalized();

	auto grid = getGrid();
	float moveDist = speed * dt * grid->UNIT_SIZE;

	if (direction.length() < moveDist) {
		unschedule(CC_SCHEDULE_SELECTOR(Character::move));
		return;
	}
	setPosition(currentPos + normalized * moveDist);
	auto winSize = Director::getInstance()->getWinSize();
	auto centerOffset = Vec2(winSize / 2) - grid->convertToWorldSpace(getPosition());
	auto newPosition = grid->getPosition() + centerOffset;
	auto gridSize = grid->getContentSize();

	auto dw = winSize.width - gridSize.width;
	newPosition.x = clampf(newPosition.x, min(0.f, dw), max(0.f, dw));
	auto dh = winSize.height - gridSize.height;
	newPosition.y = clampf(newPosition.y, min(0.f, dh), max(0.f, dh));

	grid->setPosition(newPosition);
}

void Character::movePath(float) {
	if (path.empty()) {
		unschedule(CC_SCHEDULE_SELECTOR(Character::movePath));
		return;
	}
	if (isScheduled(CC_SCHEDULE_SELECTOR(Character::move))) {
		return;
	}
	auto&& nextGridPosition = path.front();
	auto&& grid = getGrid();
	grid->occupyArea(currentGridPosition, SCALE, false);
	currentGridPosition = nextGridPosition;
	moveTo(grid->gridToPosition(nextGridPosition));
	path.pop();
}


void Character::moveTo(Vec2 position)
{

	nextPos = position;
	getGrid()->occupyArea(currentGridPosition, SCALE);

	this->schedule(CC_SCHEDULE_SELECTOR(Character::move));
}

void Character::tryToMove(GridPosition position)
{
	getGrid()->occupyArea(currentGridPosition, SCALE, false);
	path.swap(GridPathFinder().findPath(getGrid(), currentGridPosition, position, SCALE));
	schedule(CC_SCHEDULE_SELECTOR(Character::movePath));
}
