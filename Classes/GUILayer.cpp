#include "GUILayer.h"
#include <algorithm>

USING_NS_CC;
using namespace std;

GUILayer* GUILayer::instance = nullptr;

GUILayer * GUILayer::getInstance() {
	return instance;
}

void removeDied(CharacterBarMap& cb) {
	auto pred = [](auto& p) {return p.first->getCondition() == ChracterCondition::Dead; };
	auto itr = find_if(cb.begin(), cb.end(), pred);
	while (itr != cb.end()) {
		itr->first->release();
		itr = cb.erase(itr);
		itr = find_if(itr, cb.end(), pred);
	}
}

bool GUILayer::init() {
	if (!Node::init()) {
		return false;
	}

	canvas = DrawNode::create();
	if (!canvas) {
		return false;
	}
	addChild(canvas);

	instance = this;

	schedule([this](float) mutable{
		
		removeDied(characterBarMap);
		canvas->clear();
		for (const auto& p : characterBarMap) {
			auto owner = p.first;
			auto pBar = p.second;
			auto characterWorldPosition = owner->getParent()->convertToWorldSpace(owner->getPosition());
			auto drawPosition = convertToNodeSpace(characterWorldPosition);
			auto cSize = owner->getBoundingBox().size;
			drawPosition.y += cSize.height;
			pBar->drawBar(canvas, drawPosition, Size(cSize.width, cSize.height / 6));
		}
	}, "positionBars");
	return true;
}

void GUILayer::addGaugeBar(Character* owner, pBar newBar) {
	if (characterBarMap.find(owner) == characterBarMap.cend()) {
		owner->retain();
	}
	characterBarMap[owner] = newBar;
}
