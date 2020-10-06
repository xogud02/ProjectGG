/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "FightScene.h"
#include "Character.h"
#include "Grid.h"
#include "SpriteFactory.h"
#include "Monster.h"
#include "TileBuilder.h"
#include "GridObject.h"
#include "SpriteTileEnums.h"
#include "GUIBoxCreator.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename) {
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool FightScene::init() {
	//////////////////////////////
	// 1. super init first
	if (!Scene::init()) {
		return false;
	}

	return true;
}

void testTmps(FightScene* ret, Grid* grid, int rows, int cols) {
	auto tmpLava = TileBuilder::randomLiquidPit(rows / 3, cols / 3, LiquidPitType::Lava);
	//auto tmpLava = TileBuilder::randomTestPit(rows / 3, cols / 3);
	grid->addObject(tmpLava, GridPosition(rows / 3, cols / 12));

	auto tmpPit = TileBuilder::randomPit(rows / 3, cols / 3, PitContentType::Green, PitWallType::Dirt);
	grid->addObject(tmpPit, GridPosition(rows / 3, cols * 7 / 12));

	Sprite* tmpTree = Sprite::create();
	tmpTree->runAction(SpriteFactory::tree());
	grid->addChild(tmpTree, 1);
	tmpTree->setAnchorPoint(Vec2::ZERO);
	auto treeGPosition = GridPosition(rows / 2, cols / 2);
	tmpTree->setPosition(grid->gridToPosition(treeGPosition));

	tmpTree->schedule([grid](float) {
		auto tmp = Monster::create(1);
		grid->addChild(tmp, 1);
	}, 5.0f, "zen");
}



FightScene * FightScene::create(SpriteTileTheme theme, CharacterType characterType) {
	auto ret = new FightScene();
	if (!ret || !ret->init()) {
		CC_SAFE_DELETE(ret);
		return nullptr;
	}

	constexpr int rows = 30, cols = 30;
	auto grid = Grid::create(rows, cols);
	grid->addChild(TileBuilder::randomFloor(rows, cols, theme, 0.6f));

	ret->addChild(grid);
	grid->setPlayer(Player::create(characterType));

	testTmps(ret, grid, rows, cols);

	auto size = ret->getContentSize();

	float thickness = 30;
	GUIBoxCreator bottomUICreator(GUIFrameColor::Blue, true, thickness, Size(size.width, size.height / 5));
	ret->addChild(bottomUICreator.create(), 50);

	auto thumbNail = Sprite::createWithSpriteFrame(SpriteFactory::characterFrame(characterType));
	ret->addChild(thumbNail, 100);
	thumbNail->setPosition(size.width / 2, size.height / 3);

	return ret;
}
