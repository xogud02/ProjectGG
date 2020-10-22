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
	Human,
	Mammal,
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

enum class CatType {
	BigFace, Cheetah, Cougar, BigCheetah, Lion,
	Orange0, Orange1, Orange2,
	Mint0, Mint1, Mint2,
	Gray0, Gray1, Gray2,
	OrangeCatman, BlueCatman
};

enum class DemonType {
	Lizard, Naga,Spider, Golem,Snale,Mare,FatBlue,Naga,Siren,
	RedKid,Red0,Red1,DarkRedMuscle,RedMuscle,OrangeMuscle,BlueMuscle,RedBlueMuscle,
	SquirrelMan,MantisMan, ArmorSquirrelMan,BlueSquirrel,TwinHead,FlyMan,Mino,
	WhiteGoblin,BlueGoblin,RedGoblin,
	TinyMan,TinyDevil,TinyDevilTail,
	BlueImp,GoldImp,
	LongBlond,ShortBlond,
	Helmet,NakedHelmet,
	Fly,Tod
};

enum class DogType {
	White0,Crouch0,Crouch1,Dog,White1,Trainer0,
	Dark0,Dark2,Ice0,Ice1,Dark1,Trainer1,
	Brown0, Brown1, Brown2,
	Cobalt0, Cobalt1, Cobalt2,
	Black0, Black1, Black2,
	Hell0, Hell1, Hell2,
};

enum class ElementalType {
	CastleMan,LightMan,Candle,StickMan,Brick,Leaf,Log,Chicken,
	Octopus,Golem,Frost,Iron,Blizzard,Ice,
	Clowd,RedStorm,YellowStorm,BlackStorm,IceStorm,FireStorm,
	Iceman,BlizzardMan,FireMan,PoisonMan,FrostMan,
	PoisonBall,IceBall,FireBall,ChaosBall,
	LightEye,SauronEye,EvelEye,
	BrownIcicle,CobaltIcicle,Icicle,
	SnowKid0, SnowKid1, SnowKid2,
	Mimic,MiniCastle,QuestionMark,
	CeilingTrap,FloorTrap,
	LightBlink,DarkBlink
};

enum class HumanoidType {
	Muscle0, MuscleGymball, Muscle1, MuscleFire0, MuscleJuto, MuscleSword, MuscleTwinDead, MuscleLongHair,
	MuscleMask, MuscleCyclopse,MuscleFire1,
	Which0, Which1, Which2, Which3,Citizen,Stick,Pistol,Prisoner,
	Fairy0, Fairy1, Fairy2, Angel0, Angel1, Angel2,FairyWing,AngelWing,
	Swordman0, Swordman1, Swordman2, SwordmanShield,SwordlessMan, Swordman3,DarkSwordMan,
	Spear0,Spear1,Bag0,Bag1, BlueHat0, BlueHat1,Club,
	Nightcap3, Nightcap0, Nightcap1, Nightcap2, Nightcap4,Crown,
	Weird0, Weird1, Weird2, Weird3,Alien,
	Green0, Green1, Green2, GreenKing,GreenStick,
	BrownMan0,BrownWoman,Farmer0,Farmer1,Miner,
	BrownTiny,TinyBlue0,TinyBlue1,TinyBlueKing,
	BrownRobe,BlueRobe,WhiteRobe,TorchRobe,
	Gentleman,Turban,TurbanBow,Weirdo,
	BrownMan1, Kentauros0, Kentauros1, Kentauros2,
	Bald0,Bald1,Baldking,
	Nerd,SFNurse0,Queen,
	WhiteSoul,BlackSoul,Priest,
	Gargoyle0, Gargoyle1,DarkAngel,
	Jini,GreenAura,RedAura,
	ClubTinyRed0, ClubTinyRed1,TinyRedKing,
	HeadCrap0, HeadCrap1,
	BrownWeird,ClubSnow,
	Yoga,SFNurse1,
	HammerHood0,HammerHood1,
	Mermaid1,Mermaid0,
	BlueMarimo, YellowMarimo,
	Medusa
};

enum class MiscType {
	BlackMouse,WhiteMouse, BostonTerrier,Unknown0, Unknown1,BlueSFDog,YellowSFDog,
	Unknown2,MouseMan0,MouseMan1, Unknown3,
	TinyBlue0,TinyBlue1,TinyBlueRed, TinyBlueYellow,
	Monkey,Gorilla,RedGorilla,
	RedCangaroo,GreenCangaroo,
	DarkAilen,GrayAilen,SmallGrayAlien0,SmallGaryAlien1,
	Parrot0,Parrot1,
	Unknown4
};

enum class BugType {
	TinyBug0, TinyBug1, TinyBug2, TinyBug3, TinyBug4, TinyBug5, TinyBug6, TinyBug7,
	DragonFly0,DragonFly1,Caterpuller,Moth,TinyBug8,Fly,
	Spider0, Spider1, Spider2, Scorpion0, Scorpion1, Scorpion2,
	Worm0,Worm1,Egg,RedWorm0,RedWorm1,
	BrownAnt,BlackAnt,RedAnt,BlueAnt,
	Cricket0, Cricket01,Mantis0,Mantis1,
	Dragoon0, WingWorm,IceBug,
	NakedSnale0, NakedSnale1,Snale0,Snale1,
	Warsp0,Warsp1,
	Centipete0, Centipete01,
	LadyBug,PlaneLadyBug
};

enum class PlantType {
	Tree0, GreenTree1,GreenTree0,Tree1,SnowTree0,SnowTree1,IceTree0,IceTree1,
	Pine0,GreenPine1, GreenPine0, Pine1,SnowPine0,SnowPine1,IcePine0,IcePine1,
	Bracken1, Bracken0, BrackenSpary,
	SnowBracken1, SnowBracken0, SnowBrackenSpary,
	RedBracken1, RedBracken0, RedBrackenSpary,
	BlackBracken1, BlackBracken0, BlackBrackenSpary,
	DemonGrass,PlantMan1,Plantman0,
	Mushroom
};

enum class HumanType {
	Unknown0, Unknown1, Unknown2, Unknown3, Unknown4, Unknown5, Unknown6, Unknown7,
	Unknown8, Unknown9, Unknown10, Unknown11, Unknown12, Unknown13, Unknown14, Unknown15,

	Unknown16, Unknown17, Unknown18, Unknown10, Unknown20, Unknown21, Unknown22, Unknown23,
	Unknown24, Unknown25, Unknown26, Unknown27, Unknown28, Unknown29, Unknown30, Unknown31,
	Unknown32, Unknown33, Unknown34,

	Unknown35, Unknown36, Unknown37, Unknown38, Unknown39, Unknown40, Unknown41, Unknown42,
	Unknown43, Unknown44, Unknown45, Unknown46, Unknown47, Unknown48, Unknown49, Unknown50,

	Unknown51, Unknown52, Unknown53, Unknown54, Unknown55, Unknown56, Unknown57, Unknown58,
	Unknown59, Unknown60, Unknown61, Unknown62, Unknown63, Unknown64, Unknown65, Unknown66,
	Unknown67, Unknown68,
	Unknown69, Unknown70, Unknown71, Unknown72, Unknown73, Unknown74, Unknown75, Unknown76
};

enum class MammalType {
	M0, M1, M2, M3, M4, M5, M6, M7,
	M8, M9, M10, M11, M12, M13, M14,
	M15, M16, M17, M18, M19, M20,
	M21, M22, M23, M24, M25, M26,
	M27, M28, M29, M30,
	M31, M32, M33, M34,
	M35, M36, M37,
	M38, M39, M40,
	M41, M42,
	M43, M44,
	M45, M46,
	M47, M48,
};

enum class ReptileType {
	R0, R1, R2, R3, R4, R5, R6, R7,
	R8, R9, R10, R11, R12, R13, R14, R15,
	R16, R17, R18, R19, R20, R21, R22, R23,
	R24, R25, R26, R27, R28, R29,
	R30, R31, R32, R33, R34, R35, R36, R37,
	R38, R39, R40, R41, R42, R43, R44, R45,
	R46, R47,
	R48, R49, R50, R51,
	R52, R53, R54,
	R55, R56,
	R57, R58, R59, R60,
	R61, R62, R63,
	Platino
};

enum class RodentType {
	R0, R1, R2, R3, R4,
	R5, R6, R7, R8,
	R9, R10, R11,
	R12,
};

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

enum class UndeadType {
	U0, U1, U2, U3, U4, U5, U6, U7,
	U8, U9, U10, U11, U12, U13, U14, U15,
	U16, U17, U18, U19, U20, U21, U22, U23,
	U24, U25, U26, U27,
	U28, U29, U30, U31,
	U32, U33, U34,
	U35, U36, U37,
	U38, U39, U40,
	U41, U42,
	U43, U44,
};

template<>
constexpr MonsterCategory MonsterEnums::categoryOf<FishType>(FishType) { return MonsterCategory::Aquatic; }

template<>
constexpr MonsterCategory MonsterEnums::categoryOf<BirdType>(BirdType) { return MonsterCategory::Avian; }

template<>
constexpr MonsterCategory MonsterEnums::categoryOf<CatType>(CatType) { return MonsterCategory::Cat; }

template<>
constexpr MonsterCategory MonsterEnums::categoryOf<DemonType>(DemonType) { return MonsterCategory::Demon; }

template<>
constexpr MonsterCategory MonsterEnums::categoryOf<DogType>(DogType) { return MonsterCategory::Dog; }

template<>
constexpr MonsterCategory MonsterEnums::categoryOf<ElementalType>(ElementalType) { return MonsterCategory::Elemental; }

template<>
constexpr MonsterCategory MonsterEnums::categoryOf<HumanoidType>(HumanoidType) { return MonsterCategory::Humanoid; }

template<>
constexpr MonsterCategory MonsterEnums::categoryOf<MiscType>(MiscType) { return MonsterCategory::Misc; }

template<>
constexpr MonsterCategory MonsterEnums::categoryOf<BugType>(BugType) { return MonsterCategory::Pest; }

template<>
constexpr MonsterCategory MonsterEnums::categoryOf<PlantType>(PlantType) { return MonsterCategory::Plant; }

template<>
constexpr MonsterCategory MonsterEnums::categoryOf<HumanType>(HumanType) { return MonsterCategory::Human; }

template<>
constexpr MonsterCategory MonsterEnums::categoryOf<MammalType>(MammalType) { return MonsterCategory::Mammal; }

template<>
constexpr MonsterCategory MonsterEnums::categoryOf<ReptileType>(ReptileType) { return MonsterCategory::Reptile; }

template<>
constexpr MonsterCategory MonsterEnums::categoryOf<RodentType>(RodentType) { return MonsterCategory::Rodent; }

template<>
constexpr MonsterCategory MonsterEnums::categoryOf<SlimeType>(SlimeType) { return MonsterCategory::Slime; }

template<>
constexpr MonsterCategory MonsterEnums::categoryOf<UndeadType>(UndeadType) { return MonsterCategory::Undead; }
