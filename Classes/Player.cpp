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

	directions.push_back(make_pair([](float angle) {return -135 < angle && angle <= -45; }, SpriteFactory::characterMoveAction(characterType, CharacterDirection::DOWN)));
	directions.push_back(make_pair([](float angle) {return 135 < angle || angle <= -135; }, SpriteFactory::characterMoveAction(characterType, CharacterDirection::LEFT)));
	directions.push_back(make_pair([](float angle) {return -45 < angle && angle <= 45; }, SpriteFactory::characterMoveAction(characterType, CharacterDirection::RIGHT)));
	directions.push_back(make_pair([](float angle) {return 45 < angle && angle <= 135; }, SpriteFactory::characterMoveAction(characterType, CharacterDirection::UP)));

	setName("player");
	currentAction = runAction(directions[0].second);
	weapon = Weapon::create();
	addChild(weapon, 1);
	return true;
}

void Player::onMoveBegin(GridPosition next) {
	Vec2 delta = getGrid()->gridToPosition(next) - getPosition();
	float angle = CC_RADIANS_TO_DEGREES(delta.getAngle());
	weapon->setRotation(-angle + 45);
	weapon->setPosition((delta.getNormalized() + Vec2::ONE)* getContentSize().width / 2);
	for (auto direction : directions) {
		if (direction.first(angle) && currentAction != direction.second) {
			if (currentAction) {
				stopAction(currentAction);
			}
			currentAction = runAction(direction.second);
			break;
		}
	}
}

Player * Player::create(CharacterType characterType, int scale) {
	Player* ret = new Player(characterType, scale);
	if (!ret || !ret->init()) {
		CC_SAFE_DELETE(ret);
		return nullptr;
	}

	return ret;
}

AttackResult Player::attack(Character * c) {
	auto result = Character::attack(c);
	if (result != AttackResult::None) {
		weapon->swing();
	}
	return result;
}

Player::Player(CharacterType characterType, int scale) :Character(scale),characterType(characterType), currentAction(nullptr) {}

Player::~Player() {
	for (auto direction : directions) {
		direction.second->release();
	}
}
