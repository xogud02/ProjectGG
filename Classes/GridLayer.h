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

	Player* player = nullptr;

	cocos2d::Size visibleArea;
	cocos2d::Vec2 visibleAreaOffset;
	cocos2d::Map<std::string, cocos2d::SpriteBatchNode*> batches;

public:
	static GridLayer* getInstance();
	const float UNIT_SIZE;

	GridPosition vecToGrid(const cocos2d::Vec2 position) const;
	cocos2d::Vec2 gridToPosition(const GridPosition gridPosition) const;
	GridLayer(const int rows, const int cols, const float unitSize);
	void showGrid();
	bool onTouch(const cocos2d::Touch* t, const cocos2d::Event* e);

	void setVisibleArea(cocos2d::Size area);
	void setVisibleAreaOffset(cocos2d::Vec2 offset);
	void focusTo(cocos2d::Vec2 position);

	virtual void addChild(cocos2d::Node* node) override;
	virtual void addChild(cocos2d::Node* node, int zOrder) override;

	static GridLayer* create(const int rows, const int cols);

	void touched(Character* who);
	void setPlayer(Player* character);

	void addObject(GridObject*, GridPosition);
};
