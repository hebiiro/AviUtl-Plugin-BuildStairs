#pragma once

//--------------------------------------------------------------------

struct Mover
{
	int m_stairStep;
	int m_objectIndex;
	ExEdit::Object* m_object;
	int m_frame_begin;
	int m_frame_end;
};

//--------------------------------------------------------------------

class StairsBuilder
{
private:

	AviUtl::EditHandle* m_editp = 0;
	AviUtl::FilterPlugin* m_fp = 0;

	std::map<ExEdit::Object*, Mover> m_moverMap;

public:

	StairsBuilder(AviUtl::EditHandle* editp, AviUtl::FilterPlugin* fp);

	BOOL buildStairs(int command);
	BOOL playVoice(int voice);
	BOOL createMoverMap(int command);
	int checkMoverMap();
};

//--------------------------------------------------------------------
