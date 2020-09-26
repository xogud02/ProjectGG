#include "Status.h"
#include "cocos2d.h"

USING_NS_CC;

Status::Status(int hp, int maxHp, int level, int power): level(level),maxHP(maxHp), power(power){

}

int Status::getHP() const{
	return hp;
}

void Status::reduceHP(int damage) {
	CCASSERT(damage >= 0, "damage lower than 0");
	hp -= damage;
	if (hp < 0) {
		hp = 0;
	} else if (hp > maxHP) {
		hp = maxHP;
	}
}

int Status::getMaxHP() const{
	return maxHP;
}

int Status::getLevel() const {
	return level;
}

int Status::getSpeed() const {
	return speed;
}

int Status::getNoticeRange() const {
	return noticeRange;
}

float Status::getAttackInterval() const {
	return attackInterval;
}

int upgrade(int value, float ratio) {
	auto newValue = static_cast<int>(value * ratio);
	if (newValue == value) {
		++newValue;
	}
	return newValue;
}

void Status::levelUp() {
	++level;
	auto newHP = upgrade(maxHP, 1.01f);
	maxHP = newHP;
	hp = maxHP;

	power = upgrade(power, 1.05f);
}

int Status::getDamage() {
	return random(power * 0.5f, power * 1.5f);
}

ChracterCondition Status::getCondition() {
	if (hp <= 0) {
		return ChracterCondition::Dead;
	}

	return ChracterCondition::Alive;
}
