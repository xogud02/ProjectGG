#pragma once

class Status {
private:
	int level = 1;
	int maxHP = 100;
	int hp = maxHP;
	int power = 10;
public:
	Status() = default;
	Status(int hp, int maxHp, int level, int power);
	int getHP() const;
	void reduceHP(int);
	int getMaxHP() const;
	int getLevel() const;
	void levelUp();
	int getDamage();

};
