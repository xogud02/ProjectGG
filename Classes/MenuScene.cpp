#include "MenuScene.h"
#include "CharacterSelectScene.h"
#include "GUISpriteFactory.h"
#include "GridLayer.h"
#include "Player.h"
#include "GUILayer.h"
#include "TileBuilder.h"
#include "ObjectSpriteFactory.h"
#include "GridUnit.h"
#include "Portal.h"

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
	
	grid->addChild(TileBuilder::randomGround(rows, cols, SpriteTileTheme::Brighter, 0.15f));

	auto player = Player::create(CharacterType::Engineer);
	grid->setPlayer(player);

	auto portalSprite = Sprite::create();
	portalSprite->runAction(ObjectSpriteFactory::portal());
	portalSprite->setAnchorPoint(Vec2::ZERO);

	auto portal = Portal::create();
	portal->setGridPosition(GridPosition(rows / 3, cols / 2));
	grid->addChild(portal);

	auto building = TileBuilder::building(rows / 4, cols / 4, SpriteTileTheme::Brighter);
	grid->addChild(building);
	building->setGridPosition(GridPosition(rows / 4, cols / 4));

	player->tryToJump(GridPosition(rows / 2, cols / 2));

	return true;
}
