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
#include "GridLayer.h"
#include "Monster.h"
#include "TileBuilder.h"
#include "GridObject.h"
#include "SpriteTileEnums.h"
#include "GUIBoxCreator.h"
#include "CharacterSpriteFactory.h"
#include "TileSpriteFactory.h"
#include "GUILayer.h"
#include "ThrowWeapon.h"
#include "Blink.h"
#include "FireBall.h"

USING_NS_CC;
using namespace std;

// on "init" you need to initialize your instance
bool FightScene::init() {
	//////////////////////////////
	// 1. super init first
	if (!Scene::init()) {
		return false;
	}

	return true;
}

void testTmps(FightScene* ret, GridLayer* grid, int rows, int cols) {
	auto tmpLava = TileBuilder::randomLiquidPit(rows / 3, cols / 3, LiquidPitType::Lava);
	//auto tmpLava = TileBuilder::randomTestPit(rows / 3, cols / 3);
	grid->addObject(tmpLava, GridPosition(rows / 3, cols / 12));

	auto tmpPit = TileBuilder::randomPit(rows / 3, cols / 3, PitContentType::Green, PitWallType::Dirt);
	grid->addObject(tmpPit, GridPosition(rows / 3, cols * 7 / 12));

	Sprite* tmpTree = Sprite::create();
	tmpTree->runAction(TileSpriteFactory::tree());
	grid->addChild(tmpTree, 1);
	tmpTree->setAnchorPoint(Vec2::ZERO);
	auto treeGPosition = GridPosition(rows / 2, cols / 2);
	tmpTree->setPosition(grid->gridToPosition(treeGPosition));

	tmpTree->schedule([grid](float) {
		auto tmp = Monster::create(1);
		grid->addChild(tmp, 1);
	}, 5.0f, "zen");
}



FightScene * FightScene::create(SpriteTileTheme theme, CharacterType character) {
	auto ret = new FightScene();
	if (!ret || !ret->init()) {
		CC_SAFE_DELETE(ret);
		return nullptr;
	}

	auto guiLayer = GUILayer::create();
	auto player = Player::create(character);

	guiLayer->setPlayer(player);
	ret->addChild(guiLayer,1234);

	constexpr int rows = 30, cols = 30;
	auto gridLayer = GridLayer::create(rows, cols);
	gridLayer->addChild(TileBuilder::randomFloor(rows, cols, theme, 0.6f));

	ret->addChild(gridLayer);
	gridLayer->setPlayer(player);

	testTmps(ret, gridLayer, rows, cols);

	auto size = ret->getContentSize();

	auto bottomUISize = Size(size.width, size.height / 5);
	guiLayer->createBottomUI(bottomUISize, character);
	gridLayer->setVisibleArea(Size(size.width, size.height - bottomUISize.height));
	gridLayer->setVisibleAreaOffset(Vec2(0, bottomUISize.height));



	auto testSkill = make_shared<ThrowWeapon>();
	testSkill->setOwner(player);
	player->getCommand()->addSkill(CommandType::DoubleTabTarget, testSkill);
	auto blink = make_shared <::Blink>();
	blink->setOwner(player);
	player->getCommand()->addSkill(CommandType::DoubleTabGround, blink);
	auto fireBall = make_shared <FireBall>();
	player->getCommand()->addSkill(CommandType::DragPlayer, fireBall);
	fireBall->setOwner(player);

	return ret;
}
