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

class BottomUICreator {
	const Size size;
	const float leftSize;
	GUIBoxCreator boxCreator;
	const CharacterType character;
public:
	BottomUICreator(Size size, CharacterType character) :
		size(size),
		leftSize(size.height),
		boxCreator(GUIBoxCreator(GUIFrameColor::Blue, true, 30, size)),
		character(character) {
	}

	Node* create() {
		auto ret = Node::create();
		ret->addChild(createLeftBox(), 1);
		ret->addChild(createRightBox(), 1);
		return ret;
	}

private:
	Node* createLeftBox() {
		boxCreator.size = Size(leftSize, leftSize);
		auto left = boxCreator.create();
		left->addChild(createThumbnail());
		return left;
	}

	Sprite* createThumbnail() {
		auto thumbNail = Sprite::createWithSpriteFrame(SpriteFactory::characterFrame(character));
		thumbNail->setPosition(Vec2::ONE*leftSize / 2);
		auto targetSize = leftSize - boxCreator.edgeThickness * 2;
		thumbNail->setScale(targetSize / thumbNail->getContentSize().width);
		return thumbNail;
	}

	Node* createRightBox() {
		boxCreator.size = Size(size.width - leftSize, size.height);
		auto right = boxCreator.create();
		right->setPosition(Vec2(leftSize, 0));
		return right;
	}
};

FightScene * FightScene::create(SpriteTileTheme theme, CharacterType character) {
	auto ret = new FightScene();
	if (!ret || !ret->init()) {
		CC_SAFE_DELETE(ret);
		return nullptr;
	}

	constexpr int rows = 30, cols = 30;
	auto grid = Grid::create(rows, cols);
	grid->addChild(TileBuilder::randomFloor(rows, cols, theme, 0.6f));

	ret->addChild(grid);
	auto player = Player::create(character);
	grid->setPlayer(player);


	testTmps(ret, grid, rows, cols);

	auto size = ret->getContentSize();

	auto bottomUISize = Size(size.width, size.height / 5);
	ret->addChild(BottomUICreator(bottomUISize, character).create());

	grid->setVisibleArea(Size(size.width, size.height - bottomUISize.height));
	grid->setVisibleAreaOffset(Vec2(0, bottomUISize.height));

	float targetUISize = size.height / 7;
	Node* targetUI = GUIBoxCreator(GUIFrameColor::Blue, true, 30, Size(targetUISize, targetUISize)).create();
	targetUI->setPosition(Vec2(0, size.height - targetUISize));
	targetUI->setVisible(false);
	ret->addChild(targetUI);
	Sprite* thumbnail = Sprite::create();
	targetUI->addChild(thumbnail);
	ret->schedule([player, currentTarget = player->getTarget(), targetUI, thumbnail = thumbnail](float) mutable{
		auto newTarget = player->getTarget();
		if (newTarget == currentTarget) {
			return;
		}

		currentTarget = newTarget;

		if (!currentTarget) {
			targetUI->setVisible(false);
			return;
		}

		targetUI->setVisible(true);
		targetUI->removeChild(thumbnail);
		
		thumbnail = Sprite::createWithSpriteFrame(newTarget->getSpriteFrame());

		auto size = targetUI->getContentSize();

		targetUI->addChild(thumbnail, 500);
		thumbnail->setPosition(size / 2);
		thumbnail->setScale(size.width * 0.9f / thumbnail->getContentSize().width);
	}, "observePlayer");


	return ret;
}
