#include "AtlasContainer.hpp"

AtlasFrame::AtlasFrame(void) :
m_name			(""),
m_textureRect	(0, 0, 0, 0),
m_frameOffset	(0, 0, 0, 0),
m_flipX			(false),
m_flipY			(false),
m_rotated		(false),
m_hasTilt		(false),
m_frameTilt		(0, 0)
{

}

void AtlasFrame::SetTextureRect(int _x, int _y, int _w, int _h)
{
	this->m_textureRect = { _x, _y, _w, _h };
}

AtlasContainer::AtlasContainer(void) :
m_frames ()
{

}


void AtlasContainer::ClearAtlasFrames(void)
{
	this->m_frames.clear();
	this->m_frames.shrink_to_fit();
}

int AtlasContainer::FindAtlasFrameIndex(const std::string& _name)
{
	for (int i = ((int)this->m_frames.size()) - 1; i >= 0; i--)
	{
		AtlasFrame& framePTR = this->GetAtlasFrame(i);
		if (framePTR.m_name == _name)
		{
			return i;
		}
	}
	return -1;
}
bool AtlasContainer::AtlasFrameExists(const std::string& _name)
{
	return FindAtlasFrameIndex(_name) != -1;
}

AtlasFrame& AtlasContainer::GetAtlasFrame(int _index)
{
	return this->m_frames[_index];
}
AtlasFrame& AtlasContainer::GetAtlasFrame(const std::string& _name)
{
	return this->GetAtlasFrame(this->FindAtlasFrameIndex(_name));
}

size_t AtlasContainer::GetAtlasFrameCount(void)
{
	return this->m_frames.size();
}

// AtlasContainer C++ || v0.1