#include "Grid.h"
#include "FightScene.h"
#include "Character.h"

using namespace std;
USING_NS_CC;

Grid * Grid::createGrid(int rows, int cols)
{
	auto unitSize = Director::getInstance()->getWinSize().width / 32 / 2;
	Grid* ret = new Grid(rows, cols, unitSize);
	ret->initWithColor(Color4B(255, 255, 255, 128));
	ret->setContentSize(Size(unitSize * cols, unitSize * rows));
	ret->setIgnoreAnchorPointForPosition(false);
	ret->setAnchorPoint(Vec2::ONE / 2);
	ret->autorelease();
	auto player = Character::create(unitSize);//TODO 분리
	ret->setPlayer(player);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(Grid::onTouch, ret);
	ret->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, ret);

	//if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) {
		ret->showGrid();
	//}

	return ret;
}

void Grid::setPlayer(Character* player) {
	this->player = player;
	addChild(player);
	player->setPosition(getContentSize() / 2);
}

Vec2 Grid::getPositionOffset()
{
	return -getPosition()/2;
}

GridPosition Grid::vecToGrid(Vec2 position)
{
	int row = (int)position.y / (int)UNIT_SIZE;
	int col = (int)position.x / (int)UNIT_SIZE;
	return GridPosition(row, col);
}

Vec2 Grid::gridToPosition(GridPosition rowCol)
{
	float x = (rowCol.second) * UNIT_SIZE;
	float y = (rowCol.first) * UNIT_SIZE;
	return Vec2(x, y);
}

int Grid::getRows()
{
	return movableGrid.size();
}

int Grid::getCols()
{
	return movableGrid[0].size();
}

Grid::Grid(int rows, int cols, float unitSize) :UNIT_SIZE(unitSize), row(rows), coloum(cols)
{
	movableGrid = vector<vector<bool>>(rows, vector<bool>(cols, true));
}

void Grid::showGrid() {
	int rows = getRows();
	int cols = getCols();

	debugGrid = DrawNode::create();
	addChild(debugGrid, -1);
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

bool Grid::isMovable(int row, int col)
{
	if (row < 0 || row >= getRows() || col < 0 || col >= getCols()) {
		return false;
	}

	return movableGrid[row][col];
}

bool Grid::onTouch(Touch * t, Event * e)
{
	auto&& touchedPosition = t->getLocation();
	if (!getBoundingBox().containsPoint(touchedPosition)) {
		return false;
	}

	touchedPosition += getPositionOffset();

	int scale = player->SCALE;

	Vec2 leftBottomOffset = -Vec2::ONE * (scale / 2.f - 0.5f) * UNIT_SIZE;
	Vec2 vLeftBottom = touchedPosition + leftBottomOffset;
	auto gridPosition = vecToGrid(vLeftBottom);
	player->tryToMove(gridPosition);

	return true;
}

void Grid::occupyArea(const GridPosition position, const int size)
{
	const int row = position.first;
	const int col = position.second;
	for (int r = row; r < row + size; ++r) {
		for (int c = col; c < col + size; ++c) {
			if (r< 0 || r>= getRows() || c < 0 || c >= getCols()) {
				CCLOG("invalid rowcol : %d %d", r, c);
			}
			else {
			movableGrid[r][c] = false;

			}
		}
	}

	auto child = DrawNode::create();
	child->drawSolidRect(Vec2::ZERO, Vec2::ONE * UNIT_SIZE * size, Color4F::RED);
	child->setPosition(gridToPosition(position));
	debugGrid->addChild(child, 0);
}
