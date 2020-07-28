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


bool Character::init()
{
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
	addChild(hpBar,1);

	setName("character");
	
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [this](Touch* e, auto) ->bool {

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


Character::Character(int scale) :SCALE(scale)
{
}

void Character::hit(int damage)
{
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
