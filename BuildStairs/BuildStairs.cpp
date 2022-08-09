#include "pch.h"
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
	else if (commandIndex == Check::Arrange) onArrange(editp, fp);

	return FALSE;
}

BOOL onBuildStairs(AviUtl::EditHandle* editp, AviUtl::FilterPlugin* fp)
{
	MY_TRACE(_T("onBuildStairs()\n"));

	StairsBuilder builder(editp, fp);

	return builder.buildStairs(Check::BuildStairs);
}

BOOL onArrange(AviUtl::EditHandle* editp, AviUtl::FilterPlugin* fp)
{
	MY_TRACE(_T("onArrange()\n"));

	StairsBuilder builder(editp, fp);

	return builder.buildStairs(Check::Arrange);
}

//--------------------------------------------------------------------
