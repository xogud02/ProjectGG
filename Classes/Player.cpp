#include "Player.h"
#include "SpriteFactory.h"
#include "Weapon.h"
#include "CharacterSpriteFactory.h"
#include "GUILayer.h"

using namespace std;
USING_NS_CC;

bool Player::init() {
	if (!Character::init()) {
		return false;
	}

	for (int i = 0; i < 4; ++i) {
		auto dir = static_cast<CharacterDirection>(i);
		directions[dir] = CharacterSpriteFactory::characterMoveAction(characterType, dir);
	}

	setName("player");
	runAction(directions[currentDirection]);
	weapon = Weapon::create();
	addChild(weapon, 1);
	return true;
}

void Player::onAttackBegin() {
	weapon->swing(status.getAttackInterval() / 2);
}

void Player::onMoveBegin(GridPosition next, CharacterDirection nextDirection) {
	if (nextDirection == currentDirection) {
		return;
	}
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

void Player::setFocused(Character* newFocused) {
	if (newFocused == focused && focused != dynamic_cast<Character*>(this)) {
		setTarget(focused);
		return;
	}
	focused = newFocused;
	setTarget(nullptr);
}

Character * Player::getFocused() const {
	return focused;
}

void Player::setTarget(Character * target) {
	Character::setTarget(target);
	weapon->setTarget(target);
}

Weapon * Player::getWeapon() {
	return weapon;
}

shared_ptr<SkillCommand> Player::getCommand() {
	return command;
}

void Player::buff(int power, float time, cocos2d::Node * icon) {
	Character::buff(power, time, icon);
	GUILayer::getInstance()->addBuff(icon, time);
}

Player::Player(CharacterType characterType, int scale) :Character(scale),characterType(characterType),command(make_shared<SkillCommand>()){}

Player::~Player() {
	for (auto direction : directions) {
		direction.second->release();
	}
}
