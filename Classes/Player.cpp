#include "Player.h"
#include "SpriteFactory.h"
#include "HpBar.h"

using namespace std;
USING_NS_CC;

Player * Player::create(float unitSize, int scale)
{
	Player* ret = new Player(scale);
	if (!ret || !ret->initWithSpriteFrame(SpriteFactory::worriorFrame())) {
		CC_SAFE_DELETE(ret);
		return nullptr;
	}

	ret->setAnchorPoint(Vec2::ZERO);

	ret->directions.push_back(make_pair([](float angle) {return -135 < angle && angle <= -45; }, SpriteFactory::worriorMoveAction(CharacterDirection::DOWN)));
	ret->directions.push_back(make_pair([](float angle) {return 135 < angle || angle <= -135; }, SpriteFactory::worriorMoveAction(CharacterDirection::LEFT)));
	ret->directions.push_back(make_pair([](float angle) {return -45 < angle && angle <= 45; }, SpriteFactory::worriorMoveAction(CharacterDirection::RIGHT)));
	ret->directions.push_back(make_pair([](float angle) {return 45 < angle && angle <= 135; }, SpriteFactory::worriorMoveAction(CharacterDirection::UP)));

	ret->autorelease();
	float characterSize = unitSize * ret->SCALE;

	ret->setScale(SpriteFactory::getUnitScale(characterSize));
	
	Size size = ret->getContentSize();
	float width = size.width;
	
	auto draw = DrawNode::create();
	draw->drawRect(Vec2::ZERO, Vec2(size.width, size.height), Color4F::RED);
	ret->addChild(draw);

	return ret;
}

void Player::moveTo(Vec2 position)
{

	Vec2 direction = position - getPosition();
	float angle = CC_RADIANS_TO_DEGREES(direction.getAngle());
	for (auto direction : directions) {
		if (direction.first(angle) && currentAction != direction.second) {
			if (currentAction) {
				stopAction(currentAction);
			}
			currentAction = runAction(direction.second);
			break;
		}
	}
	Character::moveTo(position);
}

Player::Player(int scale) :Character(scale), currentAction(nullptr)
{

}

Player::~Player()
{
	for (auto direction : directions) {
		direction.second->release();
	}
}
