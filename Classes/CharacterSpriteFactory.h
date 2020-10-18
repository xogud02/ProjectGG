#pragma once

#include "cocos2d.h"
#include "SpriteTileEnums.h"

enum class CharacterType;

namespace std {
	template<>
	struct hash<CharacterType> {
		size_t operator()(CharacterType characterType) const;
	};
}

class CharacterSpriteFactory {
	static const std::unordered_map<CharacterType, std::string> characterPaths;
	CharacterSpriteFactory() = default;
public:

	static cocos2d::SpriteFrame* characterFrame(CharacterType);
	static const std::string getCharacterFileName(CharacterType);
	static cocos2d::Action* characterMoveAction(CharacterType, CharacterDirection);

};
