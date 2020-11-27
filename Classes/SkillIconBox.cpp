#include "SkillIconBox.h"
#include "GUIBoxCreator.h"
#include "TTFLabelBuilder.h"
#include "SpriteFactory.h"
#include <cmath>

using namespace std;
USING_NS_CC;

SkillIconBox * SkillIconBox::create(GUIBoxCreator preset) {
	auto ret = new SkillIconBox();
	if (!ret) {
		return nullptr;
	}

	preset.create(ret);
	ret->iconSize = preset.size.width;
	ret->innerIconSize = ret->iconSize - preset.edgeThickness * 2;

	
	if (!ret->init()) {
		CC_SAFE_DELETE(ret);
		return nullptr;
	}

	return ret;
}

bool SkillIconBox::init() {
	auto iconSize = getContentSize().width;
	draw = DrawNode::create();
	label = TTFLabelBuilder().setTextSize(30).build("");
	addChild(label, 3);
	label->setPosition(Vec2::ONE * iconSize / 2);
	addChild(draw, 2);

	return true;
}

void SkillIconBox::attachIcon(Node * newIcon) {
	if (icon) {
		icon->removeFromParent();
	}
	icon = newIcon;
	newIcon->setPosition(Vec2::ONE * iconSize / 2);
	newIcon->setScale(SpriteFactory::getUnitScale(innerIconSize));
	addChild(newIcon, 1);
}

void SkillIconBox::setCooldown(float newCooldown) {
	cooldown = newCooldown;
}

class LinearEquation {
	float dx, dy, slope;//y-dy = slope(x-dx)
public:
	LinearEquation(float dx, float dy, float slope):dx(dx),dy(dy),slope(slope){}

	Vec2 SolveOnX(float x) {
		return Vec2(x, slope * (x - dx) + dy);
	}

	Vec2 SolveOnY(float y) {
		return Vec2((y - dy) / slope + dx, y);
	}
};


string key = "drawCooldown";
void SkillIconBox::startCooldown() {
	schedule([this, time = cooldown, remain = cooldown, size = getContentSize().width](float delta)mutable 
	{
		draw->clear();

		auto degree = 360 - 360 * remain / time;
		auto edgeDirectionAngle = 450 - degree;
		edgeDirectionAngle = std::fmod(edgeDirectionAngle, 360);

		label->setString(to_string((int)remain));
		auto mid = size / 2;

		vector<Vec2> points;
		Vec2 center(mid, mid);
		Vec2 topLeft(0, size);
		Vec2 topMiddle(mid, size);
		Vec2 topRight(size, size);
		Vec2 bottomLeft(0, 0);
		Vec2 bottomRight(size, 0);

		auto eq = LinearEquation(mid, mid, tan(CC_DEGREES_TO_RADIANS(edgeDirectionAngle)));
		if (45 < edgeDirectionAngle && edgeDirectionAngle <= 90) {//topright
			points = { center, eq.SolveOnY(size), topRight, bottomRight, bottomLeft, topLeft, topMiddle };
		} else if (90 < edgeDirectionAngle && edgeDirectionAngle <= 135) {//topleft
			points = { eq.SolveOnY(size), topMiddle, center };
		} else if (135 < edgeDirectionAngle && edgeDirectionAngle <= 225) {//left
			points = { topLeft, topMiddle, center, eq.SolveOnX(0) };
		} else if (225 < edgeDirectionAngle && edgeDirectionAngle <= 315) {//bottom
			points = { topLeft, topMiddle, center, eq.SolveOnY(0), bottomLeft };
		} else {//right
			points = { center, eq.SolveOnX(size), bottomRight, bottomLeft ,topLeft, topMiddle };
		}
		draw->drawSolidPoly(points.data(), points.size(), Color4F(0, 0, 1, 0.5f));

		remain -= delta;
		if (remain < 0) {
			unschedule(key);
			label->setString("");
			draw->clear();
			return;
		}
	}, key);
}

bool SkillIconBox::isCoolingDown() {
	return isScheduled(key);
}
