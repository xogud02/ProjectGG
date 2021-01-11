#pragma once

enum class SpriteTilePosition {
	TopLeft, Top, TopRight,
	VirticalTop,
	Single = VirticalTop + 2,
	Left = Single + 2, Center, Right, 
	VirticalCenter, 
	HorizontalLeft, HorizontalCenter, HorizontalRight,
	BottomLeft, Bottom, BottomRight, 
	VirticalBottom
};

enum class SpriteTileTheme {
	Brighter,
	Bright,
	Dark,
	Darker
};

enum class SpriteTileType {
	BrickLoop, Grass, Bush,
	Dirt, WoodFloor, Sand,
	Mound
};

enum class CharacterDirection {
	DOWN = 0,
	LEFT,
	RIGHT,
	UP
};

enum class ArrowDirection {
	Up,
	Right,
	Down,
	Left
};

enum class DoorType {
	Wood, Iron, IronJail, GoldJail
};

enum class DoorLockType {
	None, SilverLock, GoldLock
};

enum class DoorPosition {
	Horizontal, Virtical
};

enum class WallType {
	Brick, Wood, Mine,
	YellowOre, GrayOre, RedOre,
	BlueLine, YellowLine, GreenLine,
	BrownPlane, GrayPlane, Ice
};

enum class WallPosition {
	TopLeft, Horizontal, TopRight, Plane, TopEmpty,
	Virtical = TopEmpty + 2, BottomCorner, LeftEmpty = BottomCorner + 2, Cross, RightEmpty,
	BottomLeft, BottomRight = BottomLeft + 2, BottomEmpty = BottomRight + 2
};

enum class PitContentType {
	Empty,
	Skyblue,
	Blue,
	Green
};

enum class PitWallType {
	Brick,
	Dirt,
	Solid
};

enum class PitPositionType {
	TopLeft, Top, TopRight,
	Left, CenterOrBottom, Right,
	VirticalTop, BottomLeft, VirticalBottom, BottomRight, VirticalMiddle
};

enum class LiquidPitType {
	Lava,
	Ocean,
	Swamp
};

enum class GUIFramePart {
	TopLeft, Top, TopRight,
	Left, Center, Right,
	BottomLeft, Bottom, BottomRight,
	Single
};

enum class GUIFrameColor {
	Red, Ivory,
	Blue, Gray,
	Green, Brown,
	Yellow, DarkBrown
};
