#include "PowerUp.h"
#include "Character.h"

bool PowerUp::onTarget(Character* to) {
	to->buff(10, 3);
	return true;
}
