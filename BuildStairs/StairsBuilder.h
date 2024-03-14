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
	int32_t fps = 0;
	int32_t bpm = 0;
	double frame_per_time = 0;
	BOOL use_current_frame = FALSE;

public:

	StairsBuilder(AviUtl::EditHandle* editp, AviUtl::FilterPlugin* fp, int command);

	//
	// 指定されたフレーム番号を修正して返します。
	//
	int32_t fix_frame(int32_t frame)
	{
		auto right_frame = frame_per_time;
		auto wrong_frame = ceil(right_frame);

		return (int32_t)ceil(frame * right_frame / wrong_frame);
	}

	BOOL playVoice();
	BOOL getSelectObjects();
	BOOL createMoverMap();
	BOOL addMover(int objectIndex, ExEdit::Object* object, int frame_offset);
	int checkMoverMap();
	BOOL applyMoverMap();
	BOOL moveBeginInternal(BOOL isMove);
	BOOL moveEndInternal(BOOL isMove);

	BOOL move();
	BOOL moveCurrentFrame();
	BOOL moveBegin();
	BOOL moveEnd();
	BOOL setBegin();
	BOOL setEnd();
	BOOL deleteMidPt();
	BOOL fixBPM();
};

//--------------------------------------------------------------------
