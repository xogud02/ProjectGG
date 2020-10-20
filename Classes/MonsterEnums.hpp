#pragma once

enum class MonsterCategory {
	Aquatic,
	Avian,
	Cat,
	Demon,
	Dog,
	Elemental,
	Humanoid,
	Misc,
	Pest,
	Plant,
	Player,
	Quadraped,
	Reptile,
	Rodent,
	Slime,
	Undead
};


enum class FishType {
	BrownTutle,
	RedTutle,

	BlueFlyingFish,
	GreemFlyingFish,

	Piranha,

	Robster,

	JellyFish,
	DarkerJellyFish,

	Tuna,
	DarkerDuna,
	OrangeTuna,

	Whale,
	DarkerWhale,
	BrownWhale,

	OrangeSnake,
	OrangeSnakeTail,
	BlackSnake,
	BlackSnakeTail,

	Planaria,
	BrighterPlanaria,
	GreenPlanaria,

	Squeed,

	Mermaid,

	HardBug,

	BabyTod,
	Tod
};

enum class BirdType {
	YellowPigeon, BigYellowPigeon,
	GreenPigeon, BigGreenPigeon,
	WhiatePigeon, BigWhitePigeon,
	BlackPigeon, BigBlackPigeon,

	BlackSparrow, BigBlackSparrow,
	GraySparrow, BigGraySparrow,
	BlueSparrow, BigBlueSparrow,
	OrangeSparrow, BigSparrow,
	DrakBlueSparrow, BigDrakBlueSparrow,
	BurningSparrow, BigBurningSparrow,
	BrownSparrow, BigBrownSparrow,

	BluePenguin, BigBluePenguin,
	EmperorPenguin, BigEmperorPenguin,

	Pelicun, BigPelicun,

	Duck, BigDuck,

	Bird0, BigBird0,
	Swan, BigSwan,
	Seagull, BigSeagull,
	Vulture, BigVulture,
	Parrot0, BigParrot0, Parrot1, BigParrot1, Parrot2, BigParrot2,
	Bird1, BigBird1,
	Eagle0, BigEagle0, Eagle1, BigEagle1, Eagle2, BigEagle2, BigEagle3, Eagle3,
	Bird2, BigBird2, Bird3, BigBird3, Kiwi, BigKiwi,
	Bird4, MiddleBird4, BigBird4, Peacock, MiddlePeacock, BigPeacock,
	Gryphon, BigGryPhon, WhiteGryphon, BigWhiteGryphon,
	Owl, BigOwl, WhiteOwl, BigWhiteOwl,
	BrownBat0, BigBrownBat0, BigBlackBat, BigBrownBat1, BrownBat1,
	Bird5, BigBird5,
};

enum class CatType {};

enum class DemonType {};

enum class DogType {};

enum class ElementalType {};

enum class HumanoidType {};

enum class MiscType {};

enum class PestType {};

enum class HumanType {};

enum class MammalType {};

enum class ReptileType {};

enum class RotentType {};

enum class SlimeType {
	BlurSpore,
	BrownSpore,
	WhiteSpore,
	GreenSpore,
	OrangeSpore,
	PurpleSpore,

	Baloon,

	BrownEyes,

	Drak,

	PinkEye,

	BlueEye,

	OrangeEyes,

	BlueBread,
	MashmelloBread,
	OrangeBread,

	GreenWorm,
	BlueWorm
};

enum class UndeadType {};

template<>
constexpr MonsterCategory MonsterEnums::categoryOf<FishType>(FishType) { return MonsterCategory::Aquatic; }

template<>
constexpr MonsterCategory MonsterEnums::categoryOf<SlimeType>(SlimeType) { return MonsterCategory::Slime; }

