#pragma once

class Status {
private:
	int level = 1;
	int maxHP = 100;
	int hp = maxHP;
	int power = 10;
public:

	int getHP() const;
	void reduceHP(int);
	int getMaxHP() const;
	int getLevel() const;
	void levelUp();
	int getDamage();

};
