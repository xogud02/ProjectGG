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

GridPosition Grid::vecToGrid(Vec2 position)
{
	int row = (int)position.y / (int)UNIT_SIZE;
	int col = (int)position.x / (int)UNIT_SIZE;
	return GridPosition(row, col);
}

Vec2 Grid::gridToPosition(GridPosition rowCol)
{
	float x = (rowCol.second + 0.5f) * UNIT_SIZE;
	float y = (rowCol.first + 0.5f) * UNIT_SIZE;
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

bool Grid::onTouch(Touch * t, Event * e)
{
	if (!getBoundingBox().containsPoint(t->getLocation())) {
		return false;
	}

	int scale = player->SCALE;

	Vec2 touchedPosition = t->getLocation();
	Vec2 leftBottomOffset = Vec2::ONE * (scale / 2.f - 0.5f) * UNIT_SIZE;
	CCLOG("offset %f %f ", leftBottomOffset.x, leftBottomOffset.y);
	Vec2 vLeftBottom = touchedPosition - leftBottomOffset;

	GridPosition gLeftBottom = vecToGrid(vLeftBottom);
	vLeftBottom = gridToPosition(gLeftBottom);
	Vec2 movePosition = vLeftBottom + leftBottomOffset;
	
	Vec2 offset = -getPosition() / 2;
	player->move(movePosition + offset);
	return true;
}
