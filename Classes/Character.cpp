#include "Character.h"
#include "HpBar.h"
#include "Grid.h"

USING_NS_CC;
using namespace std;

Character* Character::create(float unitSize, int scale) {
	Character* ret = new Character(scale);
	if (!ret || !ret->initWithFile("Characters/Warrior.png", CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 16, 16)))) {
		CC_SAFE_DELETE(ret);
		return nullptr;
	}

	ret->setAnchorPoint(Vec2::ZERO);

	string img = "Characters/Warrior.png";
	ret->directions.push_back(make_pair([](float angle) {return -135 < angle && angle <= -45; }, createMoveAction(img, 0)));//downWard
	ret->directions.push_back(make_pair([](float angle) {return 135 < angle || angle <= -135; }, createMoveAction(img, 1)));//leftWard
	ret->directions.push_back(make_pair([](float angle) {return -45 < angle && angle <= 45; }, createMoveAction(img, 2)));//rightWard
	ret->directions.push_back(make_pair([](float angle) {return 45 < angle && angle <= 135; }, createMoveAction(img, 3)));//upWard

	ret->autorelease();
	float characterSize = unitSize * ret->SCALE;

	ret->setScale(characterSize / 16.f);
	HPBar* hpBar = HPBar::createWithColor(Color3B::GREEN);

	Size size = ret->getContentSize();
	float width = size.width;
	hpBar->setContentSize(Size(width, width / 6));
	hpBar->setHP(1);
	hpBar->setPosition(Vec2(width / 2, 0));
	ret->addChild(hpBar);
	ret->currentAction = nullptr;

	auto draw = DrawNode::create();
	//draw->setContentSize(ret->getContentSize());
	draw->drawRect(Vec2::ZERO, Vec2(size.width, size.height), Color4F::RED);
	ret->addChild(draw);

	return ret;
}

Action* Character::createMoveAction(string file, int r)
{
	auto anim = Animation::create();
	anim->setDelayPerUnit(0.3f);
	for (int i = 0; i < 4; ++i) {
		auto frame = SpriteFrame::create(file, CC_RECT_PIXELS_TO_POINTS(Rect(16 * i, 16 * r, 16, 16)));
		anim->addSpriteFrame(frame);
	}


	auto ret = RepeatForever::create(Animate::create(anim));
	ret->retain();
	return ret;
}

Character::Character(int scale) :SCALE(scale)
{
}

Character::~Character()
{
	for (auto direction : directions) {
		direction.second->release();
	}
}

Grid * Character::getGrid()
{
	return (Grid*)getParent();
}

void Character::move(float dt) {
	auto currentPos = getPosition();
	auto direction = (nextPos - currentPos);
	auto normalized = direction.getNormalized();

	float moveDist = speed * dt;

	if (direction.length() < moveDist) {
		unschedule(CC_SCHEDULE_SELECTOR(Character::move));
		stopAction(currentAction);
		currentAction = nullptr;
		return;
	}
	setPosition(currentPos + normalized * moveDist);
}

void Character::moveTo(Vec2 position)
{
	Vec2 direction = position - getPosition();
	if (currentAction != nullptr) {
		stopAction(currentAction);
	}

	float angle = CC_RADIANS_TO_DEGREES(direction.getAngle());


	for (auto direction : directions) {
		if (direction.first(angle)) {
			currentAction = runAction(direction.second);
			break;
		}
	}

	nextPos = position;
	getGrid()->occupyArea(currentGridPosition, SCALE);

	this->schedule(CC_SCHEDULE_SELECTOR(Character::move));
}

void Character::tryToMove(GridPosition position)
{
	currentGridPosition = position;
	moveTo(getGrid()->gridToPosition(position));
}
