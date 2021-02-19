#include "TileBuilder.h"
#include "SpriteFactory.h"
#include "TileSpriteFactory.h"
#include "Door.h"
#include "GridUnits.h"
#include <unordered_set>
#include <bitset>

using namespace std;
USING_NS_CC;

using Pii = pair<int, int>;
using PSet = set<Pii>;

unsigned long getPositionArrIndex(const PSet& grassFloor, int r, int c) {
	const auto cend = grassFloor.cend();
	const auto top = grassFloor.find(Pii(r + 1, c)) == cend;
	const auto bottom = grassFloor.find(Pii(r - 1, c)) == cend;
	const auto left = grassFloor.find(Pii(r, c - 1)) == cend;
	const auto right = grassFloor.find(Pii(r, c + 1)) == cend;
	bitset<4> bits;
	bits[3] = top, bits[2] = bottom, bits[1] = left, bits[0] = right;
	return bits.to_ulong();
}

SpriteTilePosition getSpriteTilePosition(const PSet& grassFloor, int r, int c) {
	using STP = SpriteTilePosition;
	//		__00					__01 :r					__10 : l			__11 : lr
	STP typeArr[] = {
		STP::Center,			STP::Right,				STP::Left,			STP::VirticalCenter,//00__
		STP::Bottom,			STP::BottomRight,		STP::BottomLeft,	STP::VirticalBottom,//01__ : b
		STP::Top,				STP::TopRight,			STP::TopLeft,		STP::VirticalTop,	//10__ : t
		STP::HorizontalCenter,	STP::HorizontalRight,	STP::HorizontalLeft,STP::Single			//11__ : tb
	};
	return typeArr[getPositionArrIndex(grassFloor, r, c)];
}

PitPositionType getPitPosition(const PSet& liquidPit, int r, int c) {
	using PPT = PitPositionType;
	//		__00					__01 :r					__10 : l			__11 : lr
	PPT typeArr[] = {
		PPT::CenterOrBottom,	PPT::Right,				PPT::Left,			PPT::VirticalMiddle,//00__
		PPT::CenterOrBottom,	PPT::BottomRight,		PPT::BottomLeft,	PPT::VirticalBottom,//01__ : b
		PPT::Top,				PPT::TopRight,			PPT::TopLeft,		PPT::VirticalTop,	//10__ : t
		PPT::Top,				PPT::TopRight,			PPT::TopLeft,		PPT::CenterOrBottom	//11__ : tb
	};
	return typeArr[getPositionArrIndex(liquidPit, r, c)];
}

void initSingleTile(Sprite* s, int r, int c) {
	s->setPosition(Vec2(c, r) * SpriteFactory::unitSize.width);
	s->setAnchorPoint(Vec2::ZERO);
}

Node* TileBuilder::randomGround(int rows, int cols, SpriteTileTheme theme, float grassRatio) {
	auto ret = Node::create();
	auto grassFloor = PSet();
	for (int r = 0; r < rows; ++r) {
		for (int c = 0; c < cols; ++c) {
			if (rand_0_1() < grassRatio) {
				grassFloor.emplace(r, c);
			}
		}
	}

	for (int r = 0; r < rows; ++r) {
		for (int c = 0; c < cols; ++c) {
			SpriteFrame* frame = nullptr;
			auto itr = grassFloor.find(Pii(r, c));
			if (itr == grassFloor.cend()) {
				frame = TileSpriteFactory::floorFrame(SpriteTileType::Dirt, theme, SpriteTilePosition::Center);
			} else {
				frame = TileSpriteFactory::floorFrame(SpriteTileType::Grass, theme, getSpriteTilePosition(grassFloor, r, c));
			}
			auto s = Sprite::createWithSpriteFrame(frame);
			ret->addChild(s);
			initSingleTile(s, r, c);
		}
	}
	return ret;
}

GridUnits* TileBuilder::building(int rows, int cols, SpriteTileTheme theme) {
	auto ret = GridUnits::create();
	auto tileSpriteType = SpriteTileType::WoodFloor;
	auto floor = TileType::Floor;

	using FF = function<SpriteFrame*(void)>;
	auto addEdgeTiles = [ret](TileType tt, int rFrom, int rTo, int cFrom, int cTo, FF tl, FF tr, FF bl, FF br, FF t, FF b, FF l, FF right, unordered_set<GridPosition>* exception = nullptr){
		auto add = [ret, tt, &exception](int r, int c, FF f) {
			auto newPos = GridPosition(r, c);
			if (exception && exception->find(newPos) != exception->cend()) { return; }
			auto sprite = Sprite::createWithSpriteFrame(f());
			ret->addTile(GridPosition(r, c), tt, sprite);
		};

		add(rTo, cFrom, tl);
		add(rTo, cTo, tr);
		add(rFrom, cFrom, bl);
		add(rFrom, cTo, br);

		for (int c = cFrom + 1; c < cTo; ++c) {
			add(rFrom, c, b);
			add(rTo, c, t);
		}
		for (int r = rFrom + 1; r < rTo; ++r) {
			add(r, cFrom, l);
			add(r, cTo, right);
		}
	};

	auto fLambda = [ft = SpriteTileType::WoodFloor, theme](SpriteTilePosition pos){return [ft, pos, theme]() {return TileSpriteFactory::floorFrame(ft, theme, pos); }; };
	addEdgeTiles(TileType::Floor,
		1, rows - 2, 1, cols - 2,
		fLambda(SpriteTilePosition::TopLeft), fLambda(SpriteTilePosition::TopRight), fLambda(SpriteTilePosition::BottomLeft), fLambda(SpriteTilePosition::BottomRight),
		fLambda(SpriteTilePosition::Top), fLambda(SpriteTilePosition::Bottom), fLambda(SpriteTilePosition::Left), fLambda(SpriteTilePosition::Right));

	for (int r = 2; r < rows - 2; ++r) {
		for (int c = 2; c < cols - 2; ++c) {
			auto sprite = Sprite::createWithSpriteFrame(TileSpriteFactory::floorFrame(tileSpriteType, theme, SpriteTilePosition::Center));
			ret->addTile(GridPosition(r, c), floor, sprite);
		}
	}

	auto wLambda = [wt = WallType::Wood, theme](WallPosition pos){return [wt, pos, theme]() {return TileSpriteFactory::wallFrame(wt, pos, theme); }; };
	auto doorPos = GridPosition(0, (cols - 1) / 2);
	
	auto door = Door::create(DoorPosition::Horizontal);
	door->setGridPosition(doorPos);
	ret->addChild(door);
	

	unordered_set<GridPosition> doorPositions;
	doorPositions.insert(doorPos);
	++doorPos.col;
	doorPositions.insert(doorPos);
	addEdgeTiles(TileType::Block,
		0, rows - 1, 0, cols - 1,
		wLambda(WallPosition::TopLeft), wLambda(WallPosition::TopRight), wLambda(WallPosition::BottomLeft), wLambda(WallPosition::BottomRight),
		wLambda(WallPosition::Horizontal), wLambda(WallPosition::Horizontal), wLambda(WallPosition::Virtical), wLambda(WallPosition::Virtical),
		&doorPositions);
		
	return ret;
}

bool inRange(int val, int inMin, int exMax) {
	return inMin <= val && val < exMax;
}

PSet buildPit(int maxRows, int maxCols) {
	PSet visited, ret;
	queue<Pii> check;
	const int cRow = maxRows / 2, cCol = maxCols / 2;
	Pii center(cRow, cCol);
	check.push(center);
	int dr[] = { -1,1,0,0 }, dc[] = { 0,0,-1,1 };
	while (!check.empty()) {
		Pii current = check.front(); check.pop();
		int r = current.first, c = current.second;
		if (inRange(r, 0, maxRows) && inRange(c, 0, maxCols)) {} else {
			continue;
		}

		if (!visited.insert(current).second) {
			continue;
		}

		float rDist = abs(cRow - r), cDist = abs(cCol - c);
		float farFromCenterRatio = (rDist / cRow + cDist / cCol) / 2;
		if (rand_0_1() * 3 > farFromCenterRatio) {
			ret.insert(current);
			for (int i = 0; i < 4; ++i) {
				check.push(Pii(r + dr[i], c + dc[i]));
			}
		}
	}
	return ret;
}

GridUnits * createPit(int maxRows, int maxCols, function<void(Sprite*, PitPositionType)> setSprite) {
	PSet pit = buildPit(maxRows, maxCols);

	auto ret = GridUnits::create();
	for (auto pii : pit) {
		int r = pii.first, c = pii.second;
		auto s = Sprite::create();
		setSprite(s, getPitPosition(pit, r, c));
		ret->addTile(GridPosition(r, c), TileType::Block, s);
	}
	return ret;
}

GridUnits* TileBuilder::randomTestPit(int rows, int cols) {
	return createPit(rows, cols,
		[](Sprite* s, auto p) {
		s->setSpriteFrame(TileSpriteFactory::testPitPosition(p));
	});
}

GridUnits * TileBuilder::randomLiquidPit(int maxRows, int maxCols, LiquidPitType type) {
	return createPit(maxRows, maxCols,
		[type](Sprite* s, auto position) {
		s->runAction(TileSpriteFactory::liquidPitAction(type, position));
	});
}

GridUnits* TileBuilder::randomPit(int maxRows, int maxCols, PitContentType content, PitWallType wall) {
	return createPit(maxRows, maxCols,
		[content, wall](Sprite* s, auto position) {
		s->runAction(TileSpriteFactory::pitAction(content, wall, position));
	});
}
