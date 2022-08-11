﻿#include "pch.h"
#include "BuildStairs.h"
#include "StairsBuilder.h"

//--------------------------------------------------------------------

// デバッグ用コールバック関数。デバッグメッセージを出力する。
void ___outputLog(LPCTSTR text, LPCTSTR output)
{
	::OutputDebugString(output);
}

//--------------------------------------------------------------------

AviUtlInternal g_auin;

//--------------------------------------------------------------------

BOOL onCommand(int commandIndex, AviUtl::EditHandle* editp, AviUtl::FilterPlugin* fp)
{
	MY_TRACE(_T("onCommand(%d)\n"), commandIndex);

	if (commandIndex == Check::BuildStairs) onBuildStairs(editp, fp);
	else if (commandIndex == Check::MoveToBegin) onMoveToBegin(editp, fp);
	else if (commandIndex == Check::MoveToEnd) onMoveToEnd(editp, fp);
	else if (commandIndex == Check::MoveBegin) onMoveBegin(editp, fp);
	else if (commandIndex == Check::MoveEnd) onMoveEnd(editp, fp);
	else if (commandIndex == Check::SetBegin) onSetBegin(editp, fp);
	else if (commandIndex == Check::SetEnd) onSetEnd(editp, fp);

	return FALSE;
}

BOOL onBuildStairs(AviUtl::EditHandle* editp, AviUtl::FilterPlugin* fp)
{
	MY_TRACE(_T("onBuildStairs()\n"));

	StairsBuilder builder(editp, fp, Check::BuildStairs);

	return builder.move();
}

BOOL onMoveToBegin(AviUtl::EditHandle* editp, AviUtl::FilterPlugin* fp)
{
	MY_TRACE(_T("onMoveToBegin()\n"));

	StairsBuilder builder(editp, fp, Check::MoveToBegin);

	return builder.moveTo();
}

BOOL onMoveToEnd(AviUtl::EditHandle* editp, AviUtl::FilterPlugin* fp)
{
	MY_TRACE(_T("onMoveToEnd()\n"));

	StairsBuilder builder(editp, fp, Check::MoveToEnd);

	return builder.moveTo();
}

BOOL onMoveBegin(AviUtl::EditHandle* editp, AviUtl::FilterPlugin* fp)
{
	MY_TRACE(_T("onMoveBegin()\n"));

	StairsBuilder builder(editp, fp, Check::MoveBegin);

	return builder.move();
}

BOOL onMoveEnd(AviUtl::EditHandle* editp, AviUtl::FilterPlugin* fp)
{
	MY_TRACE(_T("onMoveEnd()\n"));

	StairsBuilder builder(editp, fp, Check::MoveEnd);

	return builder.move();
}

BOOL onSetBegin(AviUtl::EditHandle* editp, AviUtl::FilterPlugin* fp)
{
	MY_TRACE(_T("onSetBegin()\n"));

	StairsBuilder builder(editp, fp, Check::SetBegin);

	return builder.move();
}

BOOL onSetEnd(AviUtl::EditHandle* editp, AviUtl::FilterPlugin* fp)
{
	MY_TRACE(_T("onSetEnd()\n"));

	StairsBuilder builder(editp, fp, Check::SetEnd);

	return builder.move();
}

//--------------------------------------------------------------------
