#pragma once
#include <functional>
#include "cocos2d.h"

enum class ChracterCondition {
	Alive,
	Dead
};

class Status {
private:
	int speed = 5;
	float noticeRange = 5;
	float attackInterval = 1;
	bool _isAttackReady = true;

	int level = 1;
	int maxHP = 100;
	int hp = maxHP;
	int power = 10;
	int morePower = 0;
public:
	Status() = default;
	Status(int hp, int maxHp, int level, int power);
	int getHP() const;
	void reduceHP(int);
	int getMaxHP() const;
	int getLevel() const;
	int getSpeed() const;
	int getNoticeRange() const;
	float getAttackInterval() const;
	bool isAttackReady() const;
	void waitForAttack(cocos2d::Node* caller, std::function<void()> onReady = nullptr);

	void addMorePower(int delta);

	void levelUp();
	int getDamage();
	ChracterCondition getCondition() const;
};
