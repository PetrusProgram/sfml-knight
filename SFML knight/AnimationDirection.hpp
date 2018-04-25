#pragma once
#include "AnimationBase.hpp"
#include "Direction.hpp"
class AnimationDirection : public AnimationBase
{
protected:
	void FrameStep();
	void CropSprite();
	void ReadIn(std::stringstream& stream);
};

