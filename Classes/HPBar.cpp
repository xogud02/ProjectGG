#include "HpBar.h"

USING_NS_CC;

HPBar * HPBar::create(Size size)
{
	auto ret = new HPBar();
	if (!ret || !ret->init()) {
		CC_SAFE_DELETE(ret);
		return ret;
	}
	
	ret->fill = DrawNode::create();

	ret->autorelease();
	ret->setContentSize(size);
	ret->drawSolidRect(Vec2::ZERO, Vec2(size.width, size.height), Color4F::GRAY);
	ret->fill->setAnchorPoint(Vec2::ZERO);
	ret->addChild(ret->fill, 1);
	return ret;
}

void HPBar::setHP(float ratio){
	const Size& cs = getContentSize();
	float newWidth = cs.width * clampf(ratio, 0, 1);
	fill->clear();
	fill->drawSolidRect(Vec2::ZERO, Vec2(newWidth, cs.height), Color4F::GREEN);
}
