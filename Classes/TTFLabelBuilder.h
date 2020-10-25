#pragma once

#include "cocos2d.h"
#include "string"

class TTFLabelBuilder{
	cocos2d::TTFConfig config;
public:
	TTFLabelBuilder();
	TTFLabelBuilder& setBold(bool bold);
	TTFLabelBuilder& setTextSize(float size);
	cocos2d::Label* build(const std::string& text);
};
