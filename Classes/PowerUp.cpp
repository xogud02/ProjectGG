#include "PowerUp.h"
#include "GridUnit.h"
#include "GUILayer.h"

bool PowerUp::onTarget(GridUnit* to) {
	to->buff(10, 3, getIcon());
	return true;
}
