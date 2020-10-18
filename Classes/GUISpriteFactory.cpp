#include "GUISpriteFactory.h"
#include "SpriteFactory.h"

USING_NS_CC;
using namespace std;

const string GUISpriteFactory::GUI = "GUIs/GUI";

SpriteFrame * GUISpriteFactory::GUIArrowFrame(ArrowDirection direction) {
	return SpriteFactory::createFrame(GUI + '0', 5, 1 + static_cast<int>(direction));
}

const pair<int, int> getGUIColorOffset(GUIFrameColor color, bool filled) {
	auto val = static_cast<int>(color);
	return make_pair((val % 2 * 2 + filled ? 1 : 0) * 4, val / 2 * 3);
}

const pair<int, int>getGUIPositionOffset(GUIFramePart position) {
	if (position == GUIFramePart::Single) {
		return make_pair(0, 0);
	}

	auto val = static_cast<int>(position);
	return make_pair(val % 3 + 1, val / 3);
}

SpriteFrame * GUISpriteFactory::GUIFrame(GUIFrameColor color, GUIFramePart pos, bool filled) {
	auto cOffset = getGUIColorOffset(color, filled);
	auto pOffset = getGUIPositionOffset(pos);
	return SpriteFactory::createFrame(GUI + "0", cOffset.first + pOffset.first, 7 + cOffset.second + pOffset.second);
}
