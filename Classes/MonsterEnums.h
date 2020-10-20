#pragma once

enum class MonsterCategory;


enum class FishType;

enum class BirdType;

enum class CatType;

enum class DemonType;

enum class DogType;

enum class ElementalType;

enum class HumanoidType;

enum class MiscType;

enum class PestType;

enum class HumanType;

enum class MammalType;

enum class ReptileType;

enum class RotentType;

enum class SlimeType;

enum class UndeadType;

class MonsterEnums {
	MonsterEnums() = default;
public:
	template <typename MonsterType>
	constexpr static MonsterCategory categoryOf(MonsterType);
};

#include "MonsterEnums.hpp"
