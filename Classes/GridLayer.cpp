#include "GridLayer.h"
#include "FightScene.h"
#include "Player.h"
#include "SpriteFactory.h"
#include "GridUnit.h"
#include "GridTouchListener.h"

using namespace std;
USING_NS_CC;

GridLayer* GridLayer::instance = nullptr;

GridLayer* GridLayer::getInstance() {
	return instance;
}

GridLayer* GridLayer::create(const int rows, const int cols) {
	auto gridUnitSize = Director::getInstance()->getOpenGLView()->getFrameSize().width / 32;
	GridLayer* ret = new GridLayer(rows, cols, gridUnitSize);
	ret->initWithColor(Color4B(255, 255, 255, 64));
	ret->setContentSize(Size(gridUnitSize * cols, gridUnitSize * rows));
	ret->autorelease();

	auto listener = GridTouchListener::create(ret);
	listener->onSingleTouch = CC_CALLBACK_1(GridLayer::onSingleTouch, ret);
	listener->onSingleTouchCanceled = CC_CALLBACK_0(GridLayer::onSingleTouchCanceled, ret);
	listener->onDoubleTouch = CC_CALLBACK_1(GridLayer::onDoubleTouch, ret);
	listener->onDrag = CC_CALLBACK_2(GridLayer::onDrag, ret);
	listener->onDragEnded = CC_CALLBACK_2(GridLayer::onDragEnded, ret);

	if (COCOS2D_DEBUG) {
		ret->showGrid();
	}

	return ret;
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
		[this, lastPos = Vec2(-1, -1)]
	(float) mutable {
		auto currentPos = player->getPosition();
		if (currentPos == lastPos) {
			return;
		}
		lastPos = lastPos.lerp(currentPos, 0.1f);
		focusTo(convertToWorldSpace(lastPos));
	}, 0, tracePlayer);
	addChild(player, 1);
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

GridLayer::GridLayer(const int rows, const int cols, const float unitSize) : grid(rows, cols), UNIT_SIZE(unitSize) {
	instance = this;
	visibleArea = Director::getInstance()->getWinSize();
	visibleAreaOffset = Vec2::ZERO;
}

void GridLayer::showGrid() {

	auto debugGrid = DrawNode::create();
	auto gridLines = DrawNode::create();
	LayerColor::addChild(debugGrid, 1);
	Color4F gridColor = Color4F(0, 1, 0, 0.5f);
	Size contentSize = getContentSize();
	for (int r = 1; r < grid.rows; ++r) {
		gridLines->drawLine(Vec2(0, r*UNIT_SIZE), Vec2(contentSize.width, r*UNIT_SIZE), gridColor);
	}

	float height = getContentSize().height;
	for (int c = 1; c < grid.cols; ++c) {
		gridLines->drawLine(Vec2(c*UNIT_SIZE, 0), Vec2(c*UNIT_SIZE, contentSize.height), gridColor);
	}
	const string debug = "debug";
	debugGrid->addChild(gridLines, 1);

	auto drawSingleRect = [this, debugGrid](GridPosition pos, Color4F color) {
		auto&& rectOrigin = gridToPosition(pos);
		debugGrid->drawSolidRect(rectOrigin, rectOrigin + Vec2::ONE * UNIT_SIZE, color - Color4F(0, 0, 0, 0.5f));
	};

	schedule([debugGrid, this, &debug, drawSingleRect](float) {
		if (!player) {
			unschedule(debug);
		}
		debugGrid->clear();

		for (int r = 0; r < grid.rows; ++r) {
			for (int c = 0; c < grid.cols; ++c) {
				auto currentPosition = GridPosition(r, c);
				if (!grid.isMovableTile(currentPosition)) {
					drawSingleRect(currentPosition, Color4F::RED);
				} else if (!grid.isOccupiable(currentPosition)) {
					drawSingleRect(currentPosition, Color4F::BLUE);
				} else if (grid.isTrigger(currentPosition)) {
					drawSingleRect(currentPosition, Color4F::GREEN);
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

void GridLayer::onSingleTouch(const Vec2 & touched) {
	auto touchedCharacter = Grid::getInstance()->getOccupiedCharacter(vecToGrid(touched));
	if (touchedCharacter) {
		player->setFocused(touchedCharacter);
		return;
	}

	auto scale = player->SCALE;
	Vec2 leftBottomOffset = -Vec2::ONE * (scale / 2.f - 0.5f) * UNIT_SIZE;
	auto gridPosition = vecToGrid(touched + leftBottomOffset);
	player->setTarget(nullptr);
	player->tryToMove(gridPosition);
}

void GridLayer::onSingleTouchCanceled() {
	player->setTarget(nullptr);
	player->stopMove();
}

void GridLayer::onDoubleTouch(const Vec2 & touched) {
	auto touchedCharacter = grid.getOccupiedCharacter(vecToGrid(touched));
	if (touchedCharacter) {
		player->getCommand()->doubleTapTarget(touchedCharacter);
		return;
	}

	auto scale = player->SCALE;
	Vec2 leftBottomOffset = -Vec2::ONE * (scale / 2.f - 0.5f) * UNIT_SIZE;
	player->getCommand()->doubleTapGround(touched + leftBottomOffset);
}

void GridLayer::onDrag(const Vec2 & startPosition, const Vec2 & currentPosition) {
	if (!isDragging) {
		isDragging = true;
		draggingCharacter = Grid::getInstance()->getOccupiedCharacter(vecToGrid(startPosition));
	}

	if (draggingCharacter == player) {
		player->getCommand()->draggingPlayer(player->getCenturalPosition(), currentPosition);
	}
}

void GridLayer::onDragEnded(const Vec2 & startPosition, const Vec2 & endedPosition) {

	if (draggingCharacter && draggingCharacter != player) {
		auto playerToMonsterDirection = draggingCharacter->getPosition() - player->getPosition();
		auto draggedDirection = endedPosition - startPosition;
		auto betweenAngle = CC_RADIANS_TO_DEGREES(Vec2::angle(playerToMonsterDirection, draggedDirection));
		float interActionAngleTolerance = 45 / 2.0f;
		if (betweenAngle > 180 - interActionAngleTolerance) {
			player->getCommand()->pullCharacter(draggingCharacter);
		}
	} else if (draggingCharacter && draggingCharacter == player) {
		player->getCommand()->draggedPlayer(endedPosition);
	}

	isDragging = false;
	draggingCharacter = nullptr;
}

void GridLayer::setVisibleArea(Size area) {
	visibleArea = area;
}

void GridLayer::setVisibleAreaOffset(Vec2 offset) {
	visibleAreaOffset = offset;
}

void GridLayer::focusTo(Vec2 position) {

	auto centerOffset = Vec2(visibleArea / 2) - position;
	auto currentPosition = getPosition();
	auto newPosition = currentPosition + centerOffset;
	auto gridSize = getContentSize();

	auto dw = visibleArea.width - gridSize.width;
	newPosition.x = clampf(newPosition.x, min(0.f, dw), max(0.f, dw));
	auto dh = visibleArea.height - gridSize.height;
	newPosition.y = clampf(newPosition.y, min(0.f, dh), max(0.f, dh));

	newPosition += visibleAreaOffset;


	setPosition(newPosition);
}

void GridLayer::addChild(Node* node) {
	addChild(node, 0);
}

void GridLayer::addChild(Node* node, int zOrder) {
	Node::addChild(node, zOrder);
	node->setScale(node->getScale() * SpriteFactory::getUnitScale(UNIT_SIZE));
	auto object = dynamic_cast<GridUnit*>(node);
	if (!object) {
		return;
	}
	auto cPos = object->getCurrentGridPosition();
	object->Node::setPosition(UNIT_SIZE * cPos.col, UNIT_SIZE * cPos.row);
}
