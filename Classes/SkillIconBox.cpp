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

void SkillIconBox::startCooldown() {
	draw->schedule([this, time = cooldown, remain = cooldown, size = getContentSize().width](float delta)mutable {
		draw->clear();
		auto degree = 360 - 360 * remain / time;
		auto edgeDirectionAngle = 450 - degree;
		while (edgeDirectionAngle > 360) {
			edgeDirectionAngle -= 360;
		}
		txt->setString(to_string((int)remain));
		auto center = size / 2;
		//y - c = tan(d)(x-c)
		//y = tan(d)(x-c) + c
		//x = (y-c)/tan(d) + c

		vector<Vec2> points;
		Vec2 c(center, center), t(center, size), tl(0, size), tr(size, size), bl(0, 0), br(size, 0);
		auto tan = ::tan(CC_DEGREES_TO_RADIANS(edgeDirectionAngle));
		if (45 < edgeDirectionAngle && edgeDirectionAngle < 90) {//topright
			points = { c, Vec2(center / tan + center,size), tr, br, bl, tl, t };
		} else if (edgeDirectionAngle == 90) {
			points = { tl, tr, br, bl };
		} else if (90 < edgeDirectionAngle && edgeDirectionAngle <= 135) {//topleft
			points = { Vec2(center / tan + center,size) ,t,c };
		} else if (135 < edgeDirectionAngle && edgeDirectionAngle <= 225) {//left
			Vec2 v(0, center);
			if (edgeDirectionAngle != 180) {
				v.y = -tan * center + center;
			}
			points = { tl, t, c, v };
		} else if (225 < edgeDirectionAngle && edgeDirectionAngle <= 315) {//bottom
			Vec2 v(center, 0);
			if (edgeDirectionAngle != 270) {
				v.x = -center / tan + center;
			}
			points = { tl, t, c, v, bl };
		} else {//right
			Vec2 v(size, center);
			if (edgeDirectionAngle != 0) {
				v.y = tan * center + center;
			}
			points = { c, v, br, bl ,tl, t };
		}
		draw->drawSolidPoly(points.data(), points.size(), Color4F(0, 0, 1, 0.5f));

		remain -= delta;
		if (remain < 0) {
			unschedule("draw");
			draw->clear();
			return;
		}
	}, "draw");
}
