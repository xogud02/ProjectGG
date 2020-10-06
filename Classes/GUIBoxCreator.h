#pragma once
#include "cocos2d.h"
#include "SpriteTileEnums.h"

class GUIBoxCreator {
	cocos2d::Node* ret = nullptr;
public:
	GUIFrameColor color;
	bool filled;
	float edgeThickness;
	cocos2d::Size size;

	GUIBoxCreator(GUIFrameColor color, bool filled, float edgeThickness, cocos2d::Size size);
	float getTergetWidth() const;
	float getTargetHeight() const;
private:
	void attachPart(GUIFramePart part, cocos2d::Vec2 position, cocos2d::Size targetSize);
	void attachHorizontalEdge(GUIFramePart part, float yPosition);
	void attachVirticalEdge(GUIFramePart part, float xPosition);
	void attachCorner(GUIFramePart part, cocos2d::Vec2 position);
	void attachFill();
public:
	cocos2d::Node* create();
};