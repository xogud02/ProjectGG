#include "GaugeBar.h"

USING_NS_CC;

bool GaugeBar::init() {
	if (!DrawNode::init()) {
		return false;
	}
	fill = DrawNode::create();
	addChild(fill, 1);

	barcode = DrawNode::create();
	addChild(barcode, 2);

	return true;
}

GaugeBar * GaugeBar::create(Size size,int value) {
	auto ret = new GaugeBar();
	if (!ret || !ret->init()) {
		CC_SAFE_DELETE(ret);
		return ret;
	}

	ret->autorelease();
	ret->setContentSize(size);
	ret->drawSolidRect(Vec2::ZERO, Vec2(size.width, size.height), Color4F::GRAY);
	
	ret->setMaxValue(value);
	ret->setValue(value);

	return ret;
}

void GaugeBar::setValue(int value) {
	this->value = value;
	updateBar();
}

void GaugeBar::setMaxValue(int value) {
	maxValue = value;
	updateBar();
}

void GaugeBar::updateBar() {
	const Size& cs = getContentSize();
	float newWidth = cs.width * clampf(static_cast<float>(value) / maxValue, 0, 1);
	fill->clear();
	fill->drawSolidRect(Vec2::ZERO, Vec2(newWidth, cs.height), Color4F::GREEN);

	barcode->clear();
	auto size = getContentSize();
	int tens = maxValue / 10;
	float gap = size.width / tens;
	for (auto x = 0.f; x < newWidth; x += gap) {
		barcode->drawLine(Vec2(x, 0), Vec2(x, size.height), Color4F::BLACK);
	}
	if (value == maxValue) {
		barcode->drawLine(Vec2(cs.width, 0), Vec2(cs.width, cs.height), Color4F::BLACK);
	}
}
