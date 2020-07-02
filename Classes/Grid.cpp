#include "Grid.h"
#include "FightScene.h"
#include "Character.h"
#include "SpriteFactory.h"

using namespace std;
USING_NS_CC;

Grid * Grid::createGrid(int rows, int cols)
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
			s->setScale(SpriteFactory::getUnitScale(gridUnitSize));
			s->setAnchorPoint(Vec2::ZERO);
			ret->addChild(s);
			s->setPosition(ret->gridToPosition(GridPosition(r, c)));
		}
	}


	auto player = Character::create(gridUnitSize);//TODO 분리
	ret->setPlayer(player);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(Grid::onTouch, ret);
	ret->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, ret);


	

	//if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) {
		//ret->showGrid();
	//}

	return ret;
}

void Grid::setPlayer(Character* player) {
	this->player = player;
	addChild(player);
}

Vec2 Grid::getPositionOffset()
{
	return -getPosition();
}

GridPosition Grid::vecToGrid(Vec2 position)
{
	int row = (int)position.y / (int)UNIT_SIZE;
	int col = (int)position.x / (int)UNIT_SIZE;
	return GridPosition(row, col);
}

Vec2 Grid::gridToPosition(GridPosition rowCol)
{
	float x = (rowCol.col) * UNIT_SIZE;
	float y = (rowCol.row) * UNIT_SIZE;
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

Grid::Grid(int rows, int cols, float unitSize) :UNIT_SIZE(unitSize), row(rows), coloum(cols), debugGrid(nullptr)
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

bool Grid::isMovable(GridPosition gridPosition)
{
	return isMovable(gridPosition.row, gridPosition.col);
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

void Grid::occupyArea(const GridPosition position, const int size, bool occupy)
{
	const int row = position.row;
	const int col = position.col;
	for (int r = row; r < row + size; ++r) {
		for (int c = col; c < col + size; ++c) {
			if (r< 0 || r>= getRows() || c < 0 || c >= getCols()) {
				CCLOG("invalid rowcol : %d %d", r, c);
			}
			else {
			movableGrid[r][c] = !occupy;
			}
		}
	}

	if (debugGrid == nullptr) {
		return;
	}

	debugGrid->removeAllChildren();
	auto child = DrawNode::create();
	for (int r = 0; r < getRows(); ++r) {
		for (int c = 0; c < getCols(); ++c) {
			if (!isMovable(r, c)) {
				auto&& rectOrigin = gridToPosition(GridPosition(r, c));
				child->drawSolidRect(rectOrigin, rectOrigin + Vec2::ONE * UNIT_SIZE, Color4F::RED - Color4F(0, 0, 0, 0.5f));
			}
		}
	}
	debugGrid->addChild(child, 0);
}
