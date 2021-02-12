#pragma once
#include "Unit.h"

class Portal : public Unit {

public:
	static Portal* create();
	virtual void onTriggerIn(Unit* who) override;
};
