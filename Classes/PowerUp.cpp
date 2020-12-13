#include "PowerUp.h"
#include "Character.h"
#include "GUILayer.h"

bool PowerUp::onTarget(Character* to) {
	to->buff(10, 3, getIcon());
	return true;
}
