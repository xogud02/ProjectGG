#include "GUIBoxCreator.h"
#include "SpriteFactory.h"

USING_NS_CC;

GUIBoxCreator::GUIBoxCreator(GUIFrameColor color, bool filled, float edgeThickness, Size size) :
	color(color),
	filled(filled),
	edgeThickness(edgeThickness),
	size(size)
{}

float GUIBoxCreator::getTergetWidth() const {
	return size.width - edgeThickness * 2;
}

float GUIBoxCreator::getTargetHeight() const {
	return size.height - edgeThickness * 2;
}

void GUIBoxCreator::attachPart(GUIFramePart part, Vec2 position, Size targetSize) {
	auto partSprite = Sprite::createWithSpriteFrame(SpriteFactory::GUIFrame(color, part, filled));
	partSprite->setScaleX(targetSize.width / partSprite->getContentSize().width);
	partSprite->setScaleY(targetSize.height / partSprite->getContentSize().height);
	partSprite->setAnchorPoint(Vec2::ZERO);
	partSprite->setPosition(position);
	ret->addChild(partSprite);
}

void GUIBoxCreator::attachHorizontalEdge(GUIFramePart part, float yPosition) {
	attachPart(part, Vec2(edgeThickness, yPosition), Size(getTergetWidth(), edgeThickness));
}

void GUIBoxCreator::attachVirticalEdge(GUIFramePart part, float xPosition) {
	attachPart(part, Vec2(xPosition, edgeThickness), Size(edgeThickness, getTargetHeight()));
}

void GUIBoxCreator::attachCorner(GUIFramePart part, Vec2 position) {
	attachPart(part, position, Size(edgeThickness, edgeThickness));
}

void GUIBoxCreator::attachFill() {
	attachPart(GUIFramePart::Center, Vec2(edgeThickness, edgeThickness), Size(getTergetWidth(), getTargetHeight()));
}

Node * GUIBoxCreator::create() {
	ret = Node::create();
	ret->setContentSize(size);

	const float rightX = size.width - edgeThickness;
	const float topY = size.height - edgeThickness;
	attachHorizontalEdge(GUIFramePart::Top, topY);
	attachHorizontalEdge(GUIFramePart::Bottom, 0);
	attachVirticalEdge(GUIFramePart::Left, 0);
	attachVirticalEdge(GUIFramePart::Right, rightX);
	attachCorner(GUIFramePart::TopLeft, Vec2(0, topY));
	attachCorner(GUIFramePart::TopRight, Vec2(rightX, topY));
	attachCorner(GUIFramePart::BottomLeft, Vec2(0, 0));
	attachCorner(GUIFramePart::BottomRight, Vec2(rightX, 0));
	attachFill();
	return ret;
}
