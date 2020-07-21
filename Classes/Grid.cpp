#include "Grid.h"
#include "FightScene.h"
#include "Player.h"
#include "SpriteFactory.h"
#include "Monster.h"

using namespace std;
USING_NS_CC;

Grid * Grid::create(const int rows, const int cols)
{
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

	auto player = Player::create(gridUnitSize);//TODO 분리
	ret->setPlayer(player);

	auto monster = Monster::create(gridUnitSize, 1);
	ret->addChild(monster);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(Grid::onTouch, ret);
	ret->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, ret);

	if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) {
		ret->showGrid();
	}

	return ret;
}

bool Grid::isValidPosition(const GridPosition position) const
{
	return isValidPosition(position.row, position.col);
}

bool Grid::isValidPosition(const int row, const int col) const
{
	return 0 <= row && row < getRows() && 0 <= col && col < getCols();
}

void Grid::setPlayer(Character* player) {
	this->player = player;
	addChild(player,1);
}

GridPosition Grid::vecToGrid(Vec2 position) const
{
	auto iunitSize = static_cast<int>(UNIT_SIZE);
	int row = static_cast<int>(position.y) / iunitSize;
	int col = static_cast<int>(position.x) / iunitSize;
	return GridPosition(row, col);
}

Vec2 Grid::gridToPosition(const GridPosition rowCol) const
{
	float x = (rowCol.col) * UNIT_SIZE;
	float y = (rowCol.row) * UNIT_SIZE;
	return Vec2(x, y);
}

int Grid::getRows() const
{
	return occupiedGrid.size();
}

int Grid::getCols() const
{
	return occupiedGrid[0].size();
}

Grid::Grid(const int rows, const int cols, const float unitSize) :UNIT_SIZE(unitSize), row(rows), coloum(cols), debugGrid(nullptr)
{
	occupiedGrid = vector<vector<bool>>(rows, vector<bool>(cols, false));
}

void Grid::showGrid() {
	int rows = getRows();
	int cols = getCols();

	debugGrid = DrawNode::create();
	addChild(debugGrid, 1);
	Color4F gridColor = Color4F(0, 1, 0, 0.5f);
	Size contentSize = getContentSize();
	for (int r = 1; r < rows; ++r) {
		debugGrid->drawLine(Vec2(0, r*UNIT_SIZE), Vec2(contentSize.width, r*UNIT_SIZE), gridColor);
	}

	float height = getContentSize().height;
	for (int c = 1; c < cols; ++c) {
		debugGrid->drawLine(Vec2(c*UNIT_SIZE, 0), Vec2(c*UNIT_SIZE, contentSize.height), gridColor);
	}

}

bool Grid::isMovable(int row, int col, int size) const
{
	if (size > 1) {
		for (int r = row; r < row + size; ++r) {
			for (int c = col; c < col + size; ++c) {
				if (!isMovable(r, c)) {
					return false;
				}
			}
		}
		return true;
	}

	if (!isValidPosition(row, col)) {
		return false;
	}

	const auto tileItr = tiles.find(GridPosition(row, col));

	return !occupiedGrid[row][col] && (tileItr == tiles.cend() || (*tileItr).second == TileType::Floor);
}

bool Grid::isMovable(GridPosition gridPosition, int size) const
{
	return isMovable(gridPosition.row, gridPosition.col, size);
}

bool Grid::onTouch(const Touch * t, const Event * e)
{
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

void Grid::occupyArea(const GridPosition position, const int size, const bool occupy)
{
	const int row = position.row;
	const int col = position.col;
	for (int r = row; r < row + size; ++r) {
		for (int c = col; c < col + size; ++c) {
			if (isValidPosition(r, c)) {
				occupiedGrid[r][c] = occupy;
			}
			else {
				CCLOG("invalid rowcol : %d %d", r, c);
			}
		}
	}

	if (debugGrid == nullptr) {
		return;
	}

	debugGrid->removeAllChildren();
	const auto child = DrawNode::create();
	const int rows = getRows();
	const int cols = getCols();
	for (int r = 0; r < rows; ++r) {
		for (int c = 0; c < cols; ++c) {
			if (!isMovable(r, c)) {
				auto&& rectOrigin = gridToPosition(GridPosition(r, c));
				child->drawSolidRect(rectOrigin, rectOrigin + Vec2::ONE * UNIT_SIZE, Color4F::RED - Color4F(0, 0, 0, 0.5f));
			}
		}
	}
	debugGrid->addChild(child, -1);
}

void Grid::focusTo(Vec2 position){
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

void Grid::addChild(cocos2d::Node * node)
{
	addChild(node, 0);
}

void Grid::addChild(cocos2d::Node * node, int zOrder)
{
	LayerColor::addChild(node, zOrder);
	node->setScale(4);//tmp code
}
