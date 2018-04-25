#include "AnimationBase.hpp"
#include "SpriteSheet.hpp"

void AnimationBase::SetFrame(Frame frame) {
	if ((frame >= m_frameStart && frame <= m_frameEnd) ||
		(frame >= m_frameEnd && frame <= m_frameStart))
	{
		m_frameCurrent = frame;
	}
}

bool AnimationBase::IsInAction() {
	if (m_frameActionStart == -1 || m_frameActionEnd == -1) { return true; }
	return (m_frameCurrent >= m_frameActionStart && m_frameCurrent <= m_frameActionEnd);
}

void AnimationBase::Reset() {
	m_frameCurrent = m_frameStart;
	m_elapsedTime = 0.0f;
	CropSprite();
}

void AnimationBase::Update(float dT) {
	if (!m_playing)  
		return; 

	m_elapsedTime += dT;
	if (m_elapsedTime < m_frameTime)  
		return; 

	FrameStep();
	CropSprite();
	m_elapsedTime = 0.f;
}