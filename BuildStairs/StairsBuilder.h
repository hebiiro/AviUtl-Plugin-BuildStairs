#pragma once

//--------------------------------------------------------------------

struct Mover
{
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
	int m_command = -1;

	std::map<ExEdit::Object*, Mover> m_moverMap;
	std::vector<int> m_selectObjects;

public:

	StairsBuilder(AviUtl::EditHandle* editp, AviUtl::FilterPlugin* fp, int command);

	BOOL playVoice();
	BOOL getSelectObjects();
	BOOL createMoverMap();
	BOOL addMover(int objectIndex, ExEdit::Object* object, int frame_offset);
	int checkMoverMap();
	BOOL applyMoverMap();

	BOOL move();
	BOOL moveTo();
	BOOL deleteMidPt();
};

//--------------------------------------------------------------------
