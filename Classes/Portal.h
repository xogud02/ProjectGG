#pragma once
#include "GridUnit.h"

class Portal : public GridUnit {

public:
	static Portal* create();
	virtual void onTriggerIn(GridUnit* who) override;
};
