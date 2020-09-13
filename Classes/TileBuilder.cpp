#include "TileBuilder.h"
#include "SpriteFactory.h"
#include <unordered_set>
#include <bitset>

using namespace std;
USING_NS_CC;

using Pii = pair<int, int>;
using GrassFloor = set<Pii>;


SpriteTilePosition getTypeOfTile(const GrassFloor& grassFloor, int r, int c) {
	const auto cend = grassFloor.cend();
	const auto top = grassFloor.find(Pii(r - 1, c)) == cend;
	const auto bottom = grassFloor.find(Pii(r + 1, c)) == cend;
	const auto left = grassFloor.find(Pii(r, c - 1)) == cend;
	const auto right = grassFloor.find(Pii(r, c + 1)) == cend;
	bitset<4> bits;
	bits[3] = top, bits[2] = bottom, bits[1] = left, bits[0] = right;
	using STT = SpriteTilePosition;
	//		__00					__01 :r					__10 : l			__11 : lr
	SpriteTilePosition typeArr[] = {
		STT::Center,			STT::Right,				STT::Left,			STT::VirticalCenter,//00__
		STT::Bottom,			STT::BottomRight,		STT::BottomLeft,	STT::VirticalBottom,//01__ : b
		STT::Top,				STT::TopRight,			STT::TopLeft,		STT::VirticalTop,	//10__ : t
		STT::HorizontalCenter,	STT::HorizontalRight,	STT::HorizontalLeft,STT::Single			//11__ : tb
	};

	return typeArr[bits.to_ulong()];
}

Node * TileBuilder::randomFloor(int rows, int cols, float gridSize, float grassRatio) {
	auto ret = Node::create();
	auto grassFloor = GrassFloor();
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
				frame = SpriteFactory::floorFrame(SpriteTileType::Dirt, SpriteTileTheme::Bright, SpriteTilePosition::Center);
			} else {
				frame = SpriteFactory::floorFrame(SpriteTileType::Grass, SpriteTileTheme::Bright, getTypeOfTile(grassFloor, r, c));
			}
			auto s = Sprite::createWithSpriteFrame(frame);
			ret->addChild(s);
			s->setPosition(Vec2(c*gridSize, (rows - r - 1) * gridSize));
			s->setScale(s->getScale() * SpriteFactory::getUnitScale(gridSize));
			s->setAnchorPoint(Vec2::ZERO);
		}
	}
	return ret;
}
