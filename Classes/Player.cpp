#include "Player.h"
#include "SpriteFactory.h"
#include "Grid.h"

using namespace std;
USING_NS_CC;

bool Player::initCharacter(float unitSize) {
	if (!Character::initCharacter(unitSize)) {
		return false;
	}
	directions.push_back(make_pair([](float angle) {return -135 < angle && angle <= -45; }, SpriteFactory::worriorMoveAction(CharacterDirection::DOWN)));
	directions.push_back(make_pair([](float angle) {return 135 < angle || angle <= -135; }, SpriteFactory::worriorMoveAction(CharacterDirection::LEFT)));
	directions.push_back(make_pair([](float angle) {return -45 < angle && angle <= 45; }, SpriteFactory::worriorMoveAction(CharacterDirection::RIGHT)));
	directions.push_back(make_pair([](float angle) {return 45 < angle && angle <= 135; }, SpriteFactory::worriorMoveAction(CharacterDirection::UP)));

	return true;
}

void Player::move(float dt) {
	Character::move(dt);
	getGrid()->focusTo(getPosition());
}

Player * Player::create(float unitSize, int scale) {
	Player* ret = new Player(scale);
	if (!ret || !ret->initCharacter(unitSize)) {
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
	return ret;
}

void Player::moveTo(Vec2 position) {

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

Player::Player(int scale) :Character(scale), currentAction(nullptr) {

}

Player::~Player() {
	for (auto direction : directions) {
		direction.second->release();
	}
}
