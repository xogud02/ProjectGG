#include "WeaponSpriteFactory.h"
#include "SpriteFactory.h"

using namespace std;
USING_NS_CC;

const string WeaponSpriteFactory::MELEE_WEAPON = "Items/MedWep";


SpriteFrame * WeaponSpriteFactory::sword() {
	return SpriteFactory::createFrame(MELEE_WEAPON, 0, 0);
}
