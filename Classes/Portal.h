#pragma once
#include "GridObject.h"

class Portal : public GridObject {

public:
	static Portal* create();
	virtual void onTriggerIn(Character* who) override;
};
