#pragma once
#include <string>


using Frame = unsigned int;

class AnimationBase
{
	friend class SpriteSheet;
public:

	void SetSpriteSheet(SpriteSheet* sheet) { m_spriteSheet = sheet; }
	void SetStartFrame(Frame frame) { m_frameStart = frame; }
	void SetEndFrame(Frame frame) { m_frameEnd = frame; }
	void SetFrameRow(Frame row) { m_frameRow = row; }
	void SetActionStart(Frame frame) { m_frameActionStart = frame; }
	void SetActionEnd(Frame frame) { m_frameActionEnd = frame; }
	void SetFrameTime(float time) { m_frameTime = time; }
	void SetLooping(bool loop) { m_loop = loop; }
	void SetName(const std::string& name) { m_name = name; }
	void SetFrame(Frame frame);

	SpriteSheet* GetSpriteSheet() { return m_spriteSheet; }
	Frame GetFrame() { return m_frameCurrent; }
	Frame GetStartFrame() { return m_frameStart; }
	Frame GetEndFrame() { return m_frameEnd; }
	Frame GetFrameRow() { return m_frameRow; }
	int GetActionStart() { return m_frameActionStart; }
	int GetActionEnd() { return m_frameActionEnd; }
	float GetFrameTime() { return m_frameTime; }
	float GetElapsedTime() { return m_elapsedTime; }
	bool IsLooping() { return m_loop; }
	bool IsPlaying() { return m_playing; }
	std::string GetName() { return m_name; }
	bool IsInAction();

	void Play() { m_playing = true; }
	void Pause() { m_playing = false; }
	void Stop() { m_playing = false; Reset(); }
	void Reset();

	virtual void Update(float dT);

	friend std::stringstream& operator >>(std::stringstream& stream, AnimationBase& a)
	{
		a.ReadIn(stream);
		return stream;
	}
protected:
	virtual void FrameStep() = 0;
	virtual void CropSprite() = 0;
	virtual void ReadIn(std::stringstream& stream) = 0;

	Frame m_frameCurrent{ 0 };
	Frame m_frameStart{ 0 };
	Frame m_frameEnd{ 0 };
	Frame m_frameRow{ 0 };
	int m_frameActionStart{ -1 };
	int m_frameActionEnd{ -1 };
	float m_frameTime{ 0.f };
	float m_elapsedTime{ 0.f };
	bool m_loop{ false };
	bool m_playing{ false };

	std::string m_name;
	SpriteSheet* m_spriteSheet{ nullptr };
};