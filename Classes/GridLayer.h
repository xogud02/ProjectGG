#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"
#include "GridPosition.h"
#include "Grid.h"
#include <vector>
#include "CCMap.h"

using UINT = unsigned int;

class Character;
class GridObject;
class Player;
class GridLayer : public cocos2d::LayerColor {
	static GridLayer* instance;
	~GridLayer();
	Grid grid;


	cocos2d::Size visibleArea;
	cocos2d::Vec2 visibleAreaOffset;

	bool isDragging = false;
	Character* draggingCharacter = nullptr;

public:
	static GridLayer* getInstance();
	Player* player = nullptr;
	const float UNIT_SIZE;

	GridPosition vecToGrid(const cocos2d::Vec2 position) const;
	cocos2d::Vec2 gridToPosition(const GridPosition gridPosition) const;
	GridLayer(const int rows, const int cols, const float unitSize);
	void showGrid();

	void onSingleTouch(const cocos2d::Vec2& touched);
	void onSingleTouchCanceled();
	void onDoubleTouch(const cocos2d::Vec2& touched);
	void onDrag(const cocos2d::Vec2& startPosition, const cocos2d::Vec2& currentPosition);
	void onDragEnded(const cocos2d::Vec2& startPosition, const cocos2d::Vec2& endedPosition);

	void setVisibleArea(cocos2d::Size area);
	void setVisibleAreaOffset(cocos2d::Vec2 offset);
	void focusTo(cocos2d::Vec2 position);

	virtual void addChild(cocos2d::Node* node) override;
	virtual void addChild(cocos2d::Node* node, int zOrder) override;

	static GridLayer* create(const int rows, const int cols);

	void setPlayer(Player* character);
};
