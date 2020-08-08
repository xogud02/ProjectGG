#include "Grid.h"
#include "FightScene.h"
#include "Player.h"
#include "SpriteFactory.h"
#include "Monster.h"

using namespace std;
USING_NS_CC;

Grid * Grid::create(const int rows, const int cols) {
	auto gridUnitSize = Director::getInstance()->getWinSize().width / 32;
	Grid* ret = new Grid(rows, cols, gridUnitSize);
	ret->initWithColor(Color4B(255, 255, 255, 64));
	ret->setContentSize(Size(gridUnitSize * cols, gridUnitSize * rows));
	ret->autorelease();

	for (int r = 0; r < rows; ++r) {
		for (int c = 0; c < cols; ++c) {
			Sprite* s = Sprite::createWithSpriteFrame(SpriteFactory::grassFrame());
			s->getTexture()->setAliasTexParameters();
			s->setAnchorPoint(Vec2::ZERO);
			ret->addChild(s);
			s->setPosition(ret->gridToPosition(GridPosition(r, c)));
		}
	}

	Sprite* s = Sprite::createWithSpriteFrame(SpriteFactory::worriorFrame());

	s->runAction(SpriteFactory::tree());
	ret->addChild(s);
	s->setAnchorPoint(Vec2::ZERO);
	auto treeGPosition = GridPosition(rows / 2, cols / 2);
	s->setPosition(ret->gridToPosition(treeGPosition));

	ret->tiles[treeGPosition] = TileType::Block;

	auto player = Player::create();//TODO 분리
	ret->setPlayer(player);


	auto monster = Monster::create(1);
	ret->addChild(monster);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(Grid::onTouch, ret);
	ret->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, ret);


	if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) {
		ret->showGrid();
	}

	return ret;
}

void Grid::touched(Character * who) {
	if (who != player) {
		player->attack(who);
	}
}

bool Grid::isValidPosition(const GridPosition position) const {
	return isValidPosition(position.row, position.col);
}

bool Grid::isValidPosition(const int row, const int col) const {
	return 0 <= row && row < getRows() && 0 <= col && col < getCols();
}

void Grid::setPlayer(Player* player) {
	this->player = player;
	addChild(player, 1);
}

GridPosition Grid::vecToGrid(Vec2 position) const {
	auto iunitSize = static_cast<int>(UNIT_SIZE);
	int row = static_cast<int>(position.y) / iunitSize;
	int col = static_cast<int>(position.x) / iunitSize;
	return GridPosition(row, col);
}

Vec2 Grid::gridToPosition(const GridPosition rowCol) const {
	float x = (rowCol.col) * UNIT_SIZE;
	float y = (rowCol.row) * UNIT_SIZE;
	return Vec2(x, y);
}

int Grid::getRows() const {
	return occupiedGrid.size();
}

int Grid::getCols() const {
	return occupiedGrid[0].size();
}

Grid::Grid(const int rows, const int cols, const float unitSize) :UNIT_SIZE(unitSize), row(rows), coloum(cols) {
	occupiedGrid = vector<vector<bool>>(rows, vector<bool>(cols, false));
}

void Grid::showGrid() {
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
	debugGrid->addChild(gridLines, 1);
	schedule([debugGrid, this](float) {
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
	}, "debug");
}

bool Grid::isMovable(int row, int col, int size) const {
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

bool Grid::isMovable(GridPosition gridPosition, int size) const {
	return isMovable(gridPosition.row, gridPosition.col, size);
}

bool Grid::onTouch(const Touch * t, const Event * e) {
	auto&& touchedPosition = t->getLocation();
	if (!getBoundingBox().containsPoint(touchedPosition)) {
		return false;
	}

	int scale = player->SCALE;

	Vec2 leftBottomOffset = -Vec2::ONE * (scale / 2.f - 0.5f) * UNIT_SIZE;
	Vec2 vLeftBottom = touchedPosition + leftBottomOffset - getPosition();
	auto gridPosition = vecToGrid(vLeftBottom);
	player->tryToMove(gridPosition);

	return true;
}

void Grid::occupyArea(const GridPosition position, const int size, const bool occupy) {
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

bool Grid::isOccupied(const GridPosition position, const int size) {
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

void Grid::focusTo(Vec2 position) {
	auto winSize = Director::getInstance()->getWinSize();
	auto centerOffset = Vec2(winSize / 2) - convertToWorldSpace(position);
	auto newPosition = getPosition() + centerOffset;
	auto gridSize = getContentSize();

	auto dw = winSize.width - gridSize.width;
	newPosition.x = clampf(newPosition.x, min(0.f, dw), max(0.f, dw));
	auto dh = winSize.height - gridSize.height;
	newPosition.y = clampf(newPosition.y, min(0.f, dh), max(0.f, dh));
	setPosition(newPosition);
}

void Grid::addChild(Sprite* sprite) {
	addChild(sprite, 0);
}

void Grid::addChild(Sprite* sprite, int zOrder) {
	LayerColor::addChild(sprite, zOrder);

	sprite->setScale(sprite->getScale() * SpriteFactory::getUnitScale(UNIT_SIZE));

}
