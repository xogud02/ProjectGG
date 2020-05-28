#include "Grid.h"
#include "FightScene.h"

using namespace std;
USING_NS_CC;

Grid::Grid()
{
	Size winSize = Director::getInstance()->getWinSize();
	float rows = winSize.height / UNIT_SIZE;
	float cols = winSize.width / UNIT_SIZE;
	movableGrid = vector<vector<bool>>(rows, vector<bool>(cols, true));
}

float Grid::UNIT_SIZE = -1;

void Grid::showGrid(Scene* scene) {
	int zOrder = -1;
	auto winSize = Director::getInstance()->getWinSize();
	float thick = 4.f;
	int rows = movableGrid.size();
	auto draw = DrawNode::create();
	scene->addChild(draw);
	Color4F gridColor = Color4F(0, 1, 0, 0.5f);
	for (int r = 1; r < rows; ++r) {
		draw->drawLine(Vec2(0, r*UNIT_SIZE), Vec2(winSize.width, r*UNIT_SIZE), gridColor);
	}


	int cols = movableGrid[0].size();
	for (int c = 1; c < cols; ++c) {
		draw->drawLine(Vec2(c*UNIT_SIZE, 0), Vec2(c*UNIT_SIZE, winSize.height), gridColor);
	}
}
