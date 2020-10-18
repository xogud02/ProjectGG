#include "CharacterSpriteFactory.h"
#include "Player.h"
#include "SpriteFactory.h"

using namespace std;
USING_NS_CC;

namespace std {
	size_t hash<CharacterType>::operator()(CharacterType characterType) const {
		return hash<int>()(static_cast<int>(characterType));
	}
}

const string characters = "Characters/";
const unordered_map<CharacterType, string> CharacterSpriteFactory::characterPaths = {
	{CharacterType::Engineer,characters + "Engineer"},
	{CharacterType::Mage, characters + "Mage"},
	{CharacterType::Paladin, characters + "Paladin"},
	{CharacterType::Rogue, characters + "Rogue"},
	{CharacterType::Template, characters + "Template"},
	{CharacterType::Warrior, characters + "Warrior"},
};

SpriteFrame * CharacterSpriteFactory::characterFrame(CharacterType characterType) {
	return SpriteFactory::createFrame(getCharacterFileName(characterType), 0, 0);
}

const string CharacterSpriteFactory::getCharacterFileName(CharacterType type) {
	return characterPaths.find(type)->second;
}

Action * CharacterSpriteFactory::characterMoveAction(CharacterType characterType, CharacterDirection characterDirection) {
	auto anim = Animation::create();
	anim->setDelayPerUnit(0.3f);
	for (int i = 0; i < 4; ++i) {
		auto frame = SpriteFactory::createFrame(getCharacterFileName(characterType), i, static_cast<int>(characterDirection));
		anim->addSpriteFrame(frame);
	}

	auto ret = RepeatForever::create(Animate::create(anim));
	ret->retain();
	return ret;
}

