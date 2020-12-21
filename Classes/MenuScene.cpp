#include "MenuScene.h"
#include "CharacterSelectScene.h"
#include "GUISpriteFactory.h"
#include "GridLayer.h"
#include "Player.h"
#include "GUILayer.h"
#include "TileBuilder.h"
#include "ObjectSpriteFactory.h"

USING_NS_CC;
using namespace std;

bool MenuScene::init() {
	if (!Scene::init()) {
		return false;
	}

	int rows = 25, cols = 35;
	auto grid = GridLayer::create(rows, cols);
	addChild(grid);
	
	auto gui = GUILayer::create();
	addChild(gui, 2);
	
	grid->addChild(TileBuilder::randomFloor(rows, cols, SpriteTileTheme::Brighter, 0.15f));

	auto player = Player::create(CharacterType::Engineer);
	grid->setPlayer(player);

	auto door = Sprite::create();
	door->runAction(ObjectSpriteFactory::portal());
	door->setAnchorPoint(Vec2::ZERO);
	grid->addChild(door, 1);

	player->tryToJump(GridPosition(rows / 2, cols / 2));

	return true;
}
