#include "Player.h"
#include "SpriteFactory.h"
#include "Grid.h"

using namespace std;
USING_NS_CC;

bool Player::init() {
	if (!Character::init()) {
		return false;
	}
	directions.push_back(make_pair([](float angle) {return -135 < angle && angle <= -45; }, SpriteFactory::worriorMoveAction(CharacterDirection::DOWN)));
	directions.push_back(make_pair([](float angle) {return 135 < angle || angle <= -135; }, SpriteFactory::worriorMoveAction(CharacterDirection::LEFT)));
	directions.push_back(make_pair([](float angle) {return -45 < angle && angle <= 45; }, SpriteFactory::worriorMoveAction(CharacterDirection::RIGHT)));
	directions.push_back(make_pair([](float angle) {return 45 < angle && angle <= 135; }, SpriteFactory::worriorMoveAction(CharacterDirection::UP)));

	setName("player");
	weapon = Sprite::createWithSpriteFrame(SpriteFactory::sword());
	weapon->setName("weapon");
	addChild(weapon, 1);
	weapon->setAnchorPoint(Vec2(1, 0));
	return true;
}

void Player::move(float dt) {
	Character::move(dt);
	getGrid()->focusTo(getPosition());
}

Player * Player::create(int scale) {
	Player* ret = new Player(scale);
	if (!ret || !ret->init()) {
		CC_SAFE_DELETE(ret);
		return nullptr;
	}

	return ret;
}

void Player::moveTo(Vec2 position) {

	Vec2 delta = position - getPosition();
	float angle = CC_RADIANS_TO_DEGREES(delta.getAngle());
	weapon->setRotation(-angle + 45);
	weapon->setPosition((delta.getNormalized() +Vec2::ONE )* getContentSize().width / 2);
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

void Player::attack(Character * c)
{
	auto bounding = getBoundingBox();//TODO implement attack range
	if (c->getPosition().distance(getPosition()) < bounding.getMaxX() - bounding.getMinX()) {
		weapon->runAction(RotateBy::create(0.5f, Vec3(0, 0, 360)));
		c->hit(random(10, 20));
		
	}
}

Player::Player(int scale) :Character(scale), currentAction(nullptr) {

}

Player::~Player() {
	for (auto direction : directions) {
		direction.second->release();
	}
}
