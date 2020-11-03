#include "SkillIconBox.h"
#include "GUIBoxCreator.h"
#include "TTFLabelBuilder.h"
#include "SpriteFactory.h"

using namespace std;
USING_NS_CC;

SkillIconBox * SkillIconBox::create(GUIBoxCreator preset, cocos2d::Node * icon) {
	auto ret = new SkillIconBox();
	if (!ret) {
		return nullptr;
	}

	preset.create(ret);

	auto iconSize = preset.size.width;
	icon->setPosition(Vec2::ONE * iconSize / 2);
	auto innerIconSize = iconSize - preset.edgeThickness * 2;
	icon->setScale(SpriteFactory::getUnitScale(innerIconSize));
	ret->addChild(icon, 1);

	if (!ret->init()) {
		CC_SAFE_DELETE(ret);
		return nullptr;
	}

	return ret;
}

bool SkillIconBox::init() {
	auto iconSize = getContentSize().width;
	draw = DrawNode::create();
	txt = TTFLabelBuilder().setTextSize(30).build("");
	addChild(txt, 3);
	txt->setPosition(Vec2::ONE * iconSize / 2);
	addChild(draw, 2);

	return true;
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

void SkillIconBox::startCooldown() {
	string key = "drawCooldown";
	draw->schedule([this, time = cooldown, remain = cooldown, size = getContentSize().width, key](float delta)mutable {
		draw->clear();
		auto degree = 360 - 360 * remain / time;
		auto edgeDirectionAngle = 450 - degree;
		while (edgeDirectionAngle > 360) {
			edgeDirectionAngle -= 360;
		}
		txt->setString(to_string((int)remain));
		auto center = size / 2;

		vector<Vec2> points;
		Vec2 c(center, center), t(center, size), tl(0, size), tr(size, size), bl(0, 0), br(size, 0);
		auto eq = LinearEquation(center,center, tan(CC_DEGREES_TO_RADIANS(edgeDirectionAngle)));
		if (45 < edgeDirectionAngle && edgeDirectionAngle <= 90) {//topright
			points = { c, eq.SolveOnY(size), tr, br, bl, tl, t };
		}else if (90 < edgeDirectionAngle && edgeDirectionAngle <= 135) {//topleft
			points = { eq.SolveOnY(size), t, c };
		} else if (135 < edgeDirectionAngle && edgeDirectionAngle <= 225) {//left
			points = { tl, t, c, eq.SolveOnX(0) };
		} else if (225 < edgeDirectionAngle && edgeDirectionAngle <= 315) {//bottom
			points = { tl, t, c, eq.SolveOnY(0), bl };
		} else {//right
			points = { c, eq.SolveOnX(size), br, bl ,tl, t };
		}
		draw->drawSolidPoly(points.data(), points.size(), Color4F(0, 0, 1, 0.5f));

		remain -= delta;
		if (remain < 0) {
			unschedule(key);
			draw->clear();
			return;
		}
	}, key);
}
