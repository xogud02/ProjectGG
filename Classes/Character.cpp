#include "Character.h"
#include "HpBar.h"

USING_NS_CC;
using namespace std;

Character* Character::createCharacter(float unitSize, int scale) {
	Character* ret = new Character(scale);
	if (!ret || !ret->initWithFile("Characters/Warrior.png", CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 16, 16)))) {
		CC_SAFE_DELETE(ret);
		return nullptr;
	}

	string img = "Characters/Warrior.png";
	ret->moveDown = createMoveAction(img, 0);
	ret->moveLeft = createMoveAction(img, 1);
	ret->moveRight = createMoveAction(img, 2);
	ret->moveUp = createMoveAction(img, 3);

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

void Character::tick(float dt) {
	auto currentPos = getPosition();
	auto direction = (nextPos - currentPos);
	auto normalized = direction.getNormalized();

	float moveDist = speed * dt;

	if (direction.length() < moveDist) {
		unschedule(CC_SCHEDULE_SELECTOR(Character::tick));
		CCLOG("arrive at (%f, %f)", currentPos.x, currentPos.y);
		return;
	}
	setPosition(currentPos + normalized * moveDist);
}

void Character::move(Vec2 position)
{
	Vec2 direction = position - getPosition();
	float angle = CC_RADIANS_TO_DEGREES(direction.getAngle());
	if (currentAction != nullptr) {
		stopAction(currentAction);
	}
	CCLOG("angle : %f", angle);
	if (-45 < angle && angle <= 45) {
		currentAction = runAction(moveRight);
	}
	else if (45 < angle && angle <= 135) {
		currentAction = runAction(moveUp);
	}
	else if (-135< angle && angle <= -45) {
		currentAction = runAction(moveDown);
	}
	else {
		currentAction = runAction(moveLeft);
	}

	nextPos = position;
	if (this->isScheduled(CC_SCHEDULE_SELECTOR(Character::tick))) {
		CCLOG("moving!!");
	}
	this->schedule(CC_SCHEDULE_SELECTOR(Character::tick));
}
