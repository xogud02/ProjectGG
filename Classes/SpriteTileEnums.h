#pragma once

enum class SpriteTilePosition {
	TopLeft, Top, TopRight,
	Left, Center, Right,
	BottomLeft, Bottom, BottomRight,
	VirticalTop,
	VirticalCenter,
	VirticalBottom,
	HorizontalLeft, HorizontalCenter, HorizontalRight,
	Single,
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

enum class GUIFramePosition {
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
