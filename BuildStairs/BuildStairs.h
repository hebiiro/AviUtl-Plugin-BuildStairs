#pragma once

//--------------------------------------------------------------------

struct Track
{
	static const int32_t Frame = 0;
};

struct Check
{
	static const int32_t BuildStairs = 0;
	static const int32_t MoveToBegin = 1;
	static const int32_t MoveToEnd = 2;
	static const int32_t MoveBegin = 3;
	static const int32_t MoveEnd = 4;
	static const int32_t SetBegin = 5;
	static const int32_t SetEnd = 6;
};

//--------------------------------------------------------------------

extern AviUtlInternal g_auin;

//--------------------------------------------------------------------

BOOL onCommand(int commandIndex, AviUtl::EditHandle* editp, AviUtl::FilterPlugin* fp);
BOOL onBuildStairs(AviUtl::EditHandle* editp, AviUtl::FilterPlugin* fp);
BOOL onMoveToBegin(AviUtl::EditHandle* editp, AviUtl::FilterPlugin* fp);
BOOL onMoveToEnd(AviUtl::EditHandle* editp, AviUtl::FilterPlugin* fp);
BOOL onMoveBegin(AviUtl::EditHandle* editp, AviUtl::FilterPlugin* fp);
BOOL onMoveEnd(AviUtl::EditHandle* editp, AviUtl::FilterPlugin* fp);
BOOL onSetBegin(AviUtl::EditHandle* editp, AviUtl::FilterPlugin* fp);
BOOL onSetEnd(AviUtl::EditHandle* editp, AviUtl::FilterPlugin* fp);

//--------------------------------------------------------------------
