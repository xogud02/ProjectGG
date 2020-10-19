#include "GaugeBar.h"

USING_NS_CC;

GaugeBar::GaugeBar(Size size, int value) :maxValue(value),value(value), size(size){
}

void GaugeBar::setValue(int value) {
	this->value = value;
}

void GaugeBar::setMaxValue(int value) {
	maxValue = value;
}

void GaugeBar::drawBar(DrawNode * canvas, Vec2 position) {
	float newWidth = size.width * clampf(static_cast<float>(value) / maxValue, 0, 1);
	canvas->drawSolidRect(position, position + Vec2(newWidth, size.height), Color4F::GREEN);

	int tens = maxValue / 10;
	float gap = size.width / tens;
	for (auto x = 0.f; x < newWidth; x += gap) {
		canvas->drawLine(position+ Vec2(x, 0), position + Vec2(x, size.height), Color4F::BLACK);
	}
	if (value == maxValue) {
		canvas->drawLine(position + Vec2(size.width, 0), position + Vec2(size.width, size.height), Color4F::BLACK);
	}
}
