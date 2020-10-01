#include "Player.h"
#include "SpriteFactory.h"
#include "Grid.h"
#include "Weapon.h"

using namespace std;
USING_NS_CC;

bool Player::init() {
	if (!Character::init()) {
		return false;
	}

	for (int i = 0; i < 4; ++i) {
		auto dir = static_cast<CharacterDirection>(i);
		directions[dir] = SpriteFactory::characterMoveAction(characterType, dir);
	}

	setName("player");
	runAction(directions[currentDirection]);
	weapon = Weapon::create();
	addChild(weapon, 1);
	return true;
}

void Player::onAttackBegin() {
	weapon->runAction(RotateBy::create(status.getAttackInterval() / 2, Vec3(0, 0, 360)));
}

void Player::onMoveBegin(GridPosition next, CharacterDirection nextDirection) {
	if (nextDirection == currentDirection) {
		return;
	}
	Vec2 delta = getGrid()->gridToPosition(next) - getPosition();
	float angle = CC_RADIANS_TO_DEGREES(delta.getAngle());
	weapon->setRotation(-angle + 45);
	weapon->setPosition((delta.getNormalized() + Vec2::ONE)* getContentSize().width / 2);
	stopAction(directions[currentDirection]);
	runAction(directions[nextDirection]);
}

Player * Player::create(CharacterType characterType, int scale) {
	Player* ret = new Player(characterType, scale);
	if (!ret || !ret->init()) {
		CC_SAFE_DELETE(ret);
		return nullptr;
	}

	return ret;
}

Player::Player(CharacterType characterType, int scale) :Character(scale),characterType(characterType){}

Player::~Player() {
	for (auto direction : directions) {
		direction.second->release();
	}
}
