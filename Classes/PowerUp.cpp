#include "PowerUp.h"
#include "Unit.h"
#include "GUILayer.h"

bool PowerUp::onTarget(Unit* to) {
	to->buff(10, 3, getIcon());
	return true;
}
