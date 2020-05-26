#include "Grid.h"
#include "FightScene.h"

using namespace std;
USING_NS_CC;

Grid::Grid(float unitSize) :UNIT_SIZE(unitSize)
{
	Size winSize = Director::getInstance()->getWinSize();
	float rows = winSize.height / unitSize;
	float cols = winSize.width / unitSize;
	movableGrid = vector<vector<bool>>(rows, vector<bool>(cols, true));
}

void Grid::showGrid(Scene* scene) {
	int zOrder = -1;
	auto winSize = Director::getInstance()->getWinSize();
	float thick = 4.f;
	int rows = movableGrid.size();
	for (int r = 1; r < rows; ++r) {
		Sprite* horizontalLine = createGridSquare();
		horizontalLine->setContentSize(Size(winSize.width, thick));
		scene->addChild(horizontalLine, zOrder);
		horizontalLine->setPosition(Vec2(winSize.width / 2, r*UNIT_SIZE));
	}

	int cols = movableGrid[0].size();
	for (int c = 1; c < cols; ++c) {
		Sprite* verticalLine = createGridSquare();
		verticalLine->setContentSize(Size(thick, winSize.height));
		scene->addChild(verticalLine, zOrder);
		verticalLine->setPosition(Vec2(c*UNIT_SIZE, winSize.height / 2));
	}
}

Sprite* Grid::createGridSquare() {
	Sprite* ret = Sprite::create("WhiteSquare.png");
	ret->setOpacity(128);
	ret->setColor(Color3B::GREEN);
	return ret;
}
