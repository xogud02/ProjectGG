#include "HpBar.h"

USING_NS_CC;

HPBar * HPBar::createWithColor(Color3B color)
{
	auto ret = new HPBar();
	if (!ret) {
		CC_SAFE_DELETE(ret);
		return ret;
	}
	ret->autorelease();

	ret->fill = Sprite::create();
	ret->fill->setColor(color);
	ret->fill->setAnchorPoint(Vec2::ZERO);
	ret->addChild(ret->fill);
	return ret;
}

void HPBar::setHP(float ratio){
	const Size& cs = getContentSize();
	float newWidth = cs.width * clampf(ratio, 0, 1);
	fill->setContentSize(Size(newWidth, cs.height));
}
