#include "TTFLabelBuilder.h"

using namespace std;
USING_NS_CC;

TTFLabelBuilder::TTFLabelBuilder() :config("fonts/SDS_8x8.ttf") {
}

TTFLabelBuilder& TTFLabelBuilder::setBold(bool bold) {
	config.bold = bold;
	return *this;
}

TTFLabelBuilder& TTFLabelBuilder::setTextSize(float size) {
	config.fontSize = size;
	return *this;
}

Label* TTFLabelBuilder::build(const string& text) {
	return Label::createWithTTF(config, text);
}
