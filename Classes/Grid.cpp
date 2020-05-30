#include "Grid.h"
#include "FightScene.h"

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
	auto player = Character::createCharacter(unitSize);
	ret->setPlayer(player);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(Grid::onTouch, ret);
	ret->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, ret);

	if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) {
		ret->showGrid();
	}

	return ret;
}

void Grid::setPlayer(Character* player) {
	this->player = player;
	addChild(player);
	player->setPosition(getContentSize() / 2);
}

GridPosition Grid::vecToGrid(Vec2 position, bool isOddScale)
{
	int row = (position.y + (isOddScale ? 0 : 0.5f)) / UNIT_SIZE;
	int col = (position.x + (isOddScale ? 0 : 0.5f)) / UNIT_SIZE;
	return GridPosition(row, col);
}

Vec2 Grid::gridToPosition(GridPosition rowCol, bool isOddScale)
{
	Vec2 ret = Vec2(rowCol.second, rowCol.first) * (UNIT_SIZE);
	if (isOddScale) {
		ret += Vec2::ONE * (UNIT_SIZE / 2);
	}
	return ret;
}

int Grid::getRows()
{
	return movableGrid.size();
}

int Grid::getCols()
{
	return movableGrid[0].size();
}

Grid::Grid(int rows, int cols, float unitSize) :UNIT_SIZE(unitSize)
{
	movableGrid = vector<vector<bool>>(rows, vector<bool>(cols, true));
}

void Grid::showGrid() {
	int rows = movableGrid.size();
	int cols = movableGrid[0].size();

	auto draw = DrawNode::create();
	addChild(draw, -1);
	Color4F gridColor = Color4F(0, 1, 0, 0.5f);
	Size contentSize = getContentSize();
	for (int r = 1; r < rows; ++r) {
		draw->drawLine(Vec2(0, r*UNIT_SIZE), Vec2(contentSize.width, r*UNIT_SIZE), gridColor);
	}

	float height = getContentSize().height;
	for (int c = 1; c < cols; ++c) {
		draw->drawLine(Vec2(c*UNIT_SIZE, 0), Vec2(c*UNIT_SIZE, contentSize.height), gridColor);
	}
}

bool Grid::isMovable(int row, int col)
{
	int rows = getRows(), cols = getCols();
	if (row < 0 || row >= rows || col < 0 || col >= cols) {
		return false;
	}

	return movableGrid[row][col];
}

void Grid::movePlayer(int row, int col) {

}

bool Grid::onTouch(Touch * t, Event * e)
{
	if (!getBoundingBox().containsPoint(t->getLocation())) {
		return false;
	}

	int scale = player->SCALE;
	bool isOddScale = scale % 2 == 1;

	Vec2 touchedPosition = t->getLocation();
	Vec2 movePosition;
	if (isOddScale) {
		int row = touchedPosition.y / UNIT_SIZE;
		int col = touchedPosition.x / UNIT_SIZE;
		movePosition = Vec2(col, row) * (UNIT_SIZE)+Vec2::ONE * (UNIT_SIZE / 2);
	}
	else {
		int newX = (int)(touchedPosition.x + UNIT_SIZE / 2) / (int)UNIT_SIZE * UNIT_SIZE;
		int newY = (int)(touchedPosition.y + UNIT_SIZE / 2) / (int)UNIT_SIZE * UNIT_SIZE;
		movePosition = Vec2(newX, newY);
	}
	
	Vect offset = -getPosition() / 2;
	player->move(movePosition + offset);

	return true;
}
