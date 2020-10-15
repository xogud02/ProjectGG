#include "GridLayer.h"
#include "FightScene.h"
#include "Player.h"
#include "SpriteFactory.h"
#include "GridObject.h"

using namespace std;
USING_NS_CC;

GridLayer * GridLayer::create(const int rows, const int cols) {
	
	auto gridUnitSize = Director::getInstance()->getOpenGLView()->getFrameSize().width / 32;
	GridLayer* grid = new GridLayer(rows, cols, gridUnitSize);
	grid->initWithColor(Color4B(255, 255, 255, 64));
	grid->setContentSize(Size(gridUnitSize * cols, gridUnitSize * rows));
	grid->autorelease();

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(GridLayer::onTouch, grid);
	grid->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, grid);

	if (COCOS2D_DEBUG) {
		grid->showGrid();
	}

	return grid;
}

void GridLayer::touched(Character * who) {
	if (who != player) {
		player->setTarget(who);
	}
}

bool GridLayer::isValidPosition(const GridPosition position) const {
	return isValidPosition(position.row, position.col);
}

bool GridLayer::isValidPosition(const int row, const int col) const {
	return 0 <= row && row < getRows() && 0 <= col && col < getCols();
}

void GridLayer::setPlayer(Player* newPlayer) {
	if (player) {
		player->release();
	}
	player = newPlayer;
	if (!player) {
		return;
	}
	player->retain();
	string tracePlayer = "tracePlayer";
	if (isScheduled(tracePlayer)) {
		unschedule(tracePlayer);
	}
	schedule(
		[this, lastPos = Vec2(-1,-1)]
	(float) mutable {
		auto currentPos = player->getPosition();// +player->SCALE * UNIT_SIZE * Vec2::ONE / 2;
		if (currentPos == lastPos) {
			return;
		}
		lastPos = currentPos;
		focusTo(convertToWorldSpace(lastPos));
	},0, tracePlayer);
	addChild(player, 1);
}


void GridLayer::addObject(GridObject * gridObject, GridPosition position) {
	addChild(gridObject);
	gridObject->setPosition(gridToPosition(position));
	for (auto p : gridObject->getTiles()) {
		tiles[position + p.first] = TileType::Block;
	}
}

GridPosition GridLayer::vecToGrid(Vec2 position) const {
	auto iunitSize = static_cast<int>(UNIT_SIZE);
	int row = static_cast<int>(position.y) / iunitSize;
	int col = static_cast<int>(position.x) / iunitSize;
	return GridPosition(row, col);
}

Vec2 GridLayer::gridToPosition(const GridPosition rowCol) const {
	float x = (rowCol.col) * UNIT_SIZE;
	float y = (rowCol.row) * UNIT_SIZE;
	return Vec2(x, y);
}

int GridLayer::getRows() const {
	return occupiedGrid.size();
}

int GridLayer::getCols() const {
	return occupiedGrid[0].size();
}

GridLayer::GridLayer(const int rows, const int cols, const float unitSize) :UNIT_SIZE(unitSize), row(rows), coloum(cols) {
	occupiedGrid = vector<vector<bool>>(rows, vector<bool>(cols, false));
	visibleArea = Director::getInstance()->getWinSize();
	visibleAreaOffset = Vec2::ZERO;
}

void GridLayer::showGrid() {
	int rows = getRows();
	int cols = getCols();

	auto debugGrid = DrawNode::create();
	auto gridLines = DrawNode::create();
	LayerColor::addChild(debugGrid, 1);
	Color4F gridColor = Color4F(0, 1, 0, 0.5f);
	Size contentSize = getContentSize();
	for (int r = 1; r < rows; ++r) {
		gridLines->drawLine(Vec2(0, r*UNIT_SIZE), Vec2(contentSize.width, r*UNIT_SIZE), gridColor);
	}

	float height = getContentSize().height;
	for (int c = 1; c < cols; ++c) {
		gridLines->drawLine(Vec2(c*UNIT_SIZE, 0), Vec2(c*UNIT_SIZE, contentSize.height), gridColor);
	}
	const string debug = "debug";
	debugGrid->addChild(gridLines, 1);
	schedule([debugGrid, this, &debug](float) {
		if (!player) {
			unschedule(debug);
		}
		debugGrid->clear();
		const int rows = getRows();
		const int cols = getCols();
		for (int r = 0; r < rows; ++r) {
			for (int c = 0; c < cols; ++c) {
				if (!isMovable(r, c)) {
					auto&& rectOrigin = gridToPosition(GridPosition(r, c));
					debugGrid->drawSolidRect(rectOrigin, rectOrigin + Vec2::ONE * UNIT_SIZE, Color4F::RED - Color4F(0, 0, 0, 0.5f));
				} else if (occupiedGrid[r][c]) {
					auto&& rectOrigin = gridToPosition(GridPosition(r, c));
					debugGrid->drawSolidRect(rectOrigin, rectOrigin + Vec2::ONE * UNIT_SIZE, Color4F::BLUE - Color4F(0, 0, 0, 0.5f));
				}
			}
		}
		auto bounding = player->getBoundingBox();
		auto minX = bounding.getMinX(), minY = bounding.getMinY();
		debugGrid->drawRect(Vec2(minX, minY), Vec2(bounding.getMaxX(), bounding.getMaxY()), Color4F::BLUE);
	}, debug);
}

GridLayer::~GridLayer() {
	if (player) {
		player->release();
	}
}

bool GridLayer::isMovable(int row, int col, int size) const {
	for (int dr = 0; dr < size; ++dr) {
		for (int dc = 0; dc < size; ++dc) {
			auto current = GridPosition(row + dr, col + dc);
			if (!isValidPosition(current)) {
				return false;
			}

			const auto tileItr = tiles.find(current);
			if (tileItr != tiles.cend() && tileItr->second != TileType::Floor) {
				return false;
			}
		}
	}
	return true;
}

bool GridLayer::isMovable(GridPosition gridPosition, int size) const {
	return isMovable(gridPosition.row, gridPosition.col, size);
}

bool GridLayer::onTouch(const Touch * t, const Event * e) {
	auto&& touchedPosition = t->getLocation();
	if (!getBoundingBox().containsPoint(touchedPosition)) {
		return false;
	}

	int scale = player->SCALE;

	Vec2 leftBottomOffset = -Vec2::ONE * (scale / 2.f - 0.5f) * UNIT_SIZE;
	Vec2 vLeftBottom = touchedPosition + leftBottomOffset - getPosition();
	auto gridPosition = vecToGrid(vLeftBottom);

	player->setTarget(nullptr);
	player->tryToMove(gridPosition);

	return true;
}

void GridLayer::occupyArea(const GridPosition position, const int size, const bool occupy) {
	const int row = position.row;
	const int col = position.col;

	for (int dr = 0; dr < size; ++dr) {
		for (int dc = 0; dc < size; ++dc) {
			int r = row + dr, c = col + dc;
			if (!isValidPosition(r, c)) {
				return;
			}
			//CCASSERT(isValidPosition(r, c), "ccassert at void Grid::occupyArea() : position must be valid");
			occupiedGrid[r][c] = occupy;
		}
	}
}

bool GridLayer::isOccupied(const GridPosition position, const int size) {//FIXME occurs out of range
	int r = position.row;
	int c = position.col;
	for (int dr = 0; dr < size; ++dr) {
		for (int dc = 0; dc < size; ++dc) {
			if (occupiedGrid[r + dr][c + dc]) {
				return true;
			}
		}
	}
	return false;
}

void GridLayer::setVisibleArea(Size area) {
	visibleArea = area;
}

void GridLayer::setVisibleAreaOffset(Vec2 offset) {
	visibleAreaOffset = offset;
}

void GridLayer::focusTo(Vec2 position) {

	auto centerOffset = Vec2(visibleArea / 2) - position;
	auto newPosition = getPosition() + centerOffset;
	auto gridSize = getContentSize();

	auto dw = visibleArea.width - gridSize.width;
	newPosition.x = clampf(newPosition.x, min(0.f, dw), max(0.f, dw));
	auto dh = visibleArea.height - gridSize.height;
	newPosition.y = clampf(newPosition.y, min(0.f, dh), max(0.f, dh));
	setPosition(newPosition + visibleAreaOffset);
}

void GridLayer::addChild(Node* node) {
	addChild(node, 0);
}

void GridLayer::addChild(Node* node, int zOrder) {
	LayerColor::addChild(node, zOrder);
	node->setScale(node->getScale() * SpriteFactory::getUnitScale(UNIT_SIZE));
}
