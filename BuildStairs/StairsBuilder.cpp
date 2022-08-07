#include "pch.h"
#include "StairsBuilder.h"
#include "BuildStairs.h"

//--------------------------------------------------------------------

StairsBuilder::StairsBuilder(AviUtl::EditHandle* editp, AviUtl::FilterPlugin* fp)
{
	m_editp = editp;
	m_fp = fp;
}

BOOL StairsBuilder::playVoice(int voice)
{
	if (!voice)
		return FALSE;

	// フォルダ名を取得する。
	TCHAR folderName[MAX_PATH] = {};
	::GetModuleFileName(m_fp->dll_hinst, folderName, MAX_PATH);
	::PathRemoveExtension(folderName);
	MY_TRACE_TSTR(folderName);

	// wav ファイルのパスを取得する。
	TCHAR wavFileName[MAX_PATH] = {};
	::StringCbPrintf(wavFileName, sizeof(wavFileName), _T("%s\\%d.wav"), folderName, voice);
	MY_TRACE_TSTR(wavFileName);

	// ファイルが存在するなら
	if (::GetFileAttributes(wavFileName) != INVALID_FILE_ATTRIBUTES)
	{
		// wav ファイルを再生する。
		::PlaySound(wavFileName, 0, SND_FILENAME | SND_ASYNC);
	}

	return TRUE;
}

BOOL StairsBuilder::getMoverMap()
{
	int stairFrame = m_fp->track[Track::Frame];

	// 選択オブジェクトの数を取得する。
	int c = g_auin.GetSelectedObjectCount();
	MY_TRACE_INT(c);

	int stairStep = 0;

	for (int i = 0; i < c; i++)
	{
		// 選択オブジェクトのインデックスを取得する。
		int objectIndex = g_auin.GetSelectedObject(i);
		MY_TRACE_INT(objectIndex);
		if (objectIndex < 0) continue;

		// 選択オブジェクトを取得する。
		ExEdit::Object* object = g_auin.GetObject(objectIndex);
		MY_TRACE_HEX(object);
		if (!object) continue;

		int frame_begin = object->frame_begin + stairFrame * stairStep;
		int frame_end = object->frame_end + stairFrame * stairStep;

		m_moverMap[object] = Mover(i, objectIndex, object, frame_begin, frame_end);

		stairStep++;
	}

	return TRUE;
}

int StairsBuilder::checkMoverMap()
{
	int invalidCount = 0;

	for (auto& value : m_moverMap)
	{
		Mover& mover = value.second;

		if (mover.m_frame_begin < 0)
		{
			// mover が範囲外になっている。
			invalidCount++;
			continue;
		}

		// オブジェクトの数を取得する。
		int c = g_auin.GetCurrentSceneObjectCount();
		MY_TRACE_INT(c);

		for (int i = 0; i < c; i++)
		{
			// オブジェクトを取得する。
			ExEdit::Object* object = g_auin.GetSortedObject(i);
			MY_TRACE_HEX(object);
			if (!object) continue;

			// object が mover と同じなら
			if (object == mover.m_object)
				continue; // スキップする。

			// レイヤーが違うなら
			if (object->layer_set != mover.m_object->layer_set)
				continue; // スキップする。

			// オブジェクトのフレームを取得する。
			int frame_begin = object->frame_begin;
			int frame_end = object->frame_end;

			// mover があれば
			auto it = m_moverMap.find(object);
			if (it != m_moverMap.end())
			{
				// mover からフレームを取得する。
				frame_begin = it->second.m_frame_begin;
				frame_end = it->second.m_frame_end;
			}

			if (frame_end <= mover.m_frame_begin)
				continue; // object が mover より完全に前にあるのでスキップする。

			if (mover.m_frame_end <= frame_begin)
				continue; // object が mover より完全に後ろにあるのでスキップする。

			// object と mover は重なっている。
			invalidCount++;
			break;
		}
	}

	return invalidCount;
}

//--------------------------------------------------------------------

BOOL StairsBuilder::buildStairs()
{
	// オブジェクトを動かしたあとの状態を構築する。

	getMoverMap();

	if (m_moverMap.empty())
	{
		// エラーメッセージを出して終了する。
		TCHAR text[MAX_PATH] = {};
		::StringCbPrintf(text, sizeof(text), _T("動かせるアイテムがありません"));
		::MessageBox(m_fp->hwnd, text, _T("BuildStairs"), MB_OK | MB_ICONWARNING);

		return FALSE;
	}

	// オブジェクトを動かせるかチェックする。

	int invalidCount = checkMoverMap();

	// オブジェクトが重なっていて動かせないなら
	if (invalidCount)
	{
		// エラーメッセージを出して終了する。
		TCHAR text[MAX_PATH] = {};
		::StringCbPrintf(text, sizeof(text), _T("%d 個のアイテムの位置が無効で動かせません"), invalidCount);
		::MessageBox(m_fp->hwnd, text, _T("BuildStairs"), MB_OK | MB_ICONWARNING);

		return FALSE;
	}

	// 声で知らせる。

	playVoice(m_fp->track[Track::Voice]);

	// 実際にオブジェクトを動かす。

	g_auin.PushUndo();

	for (auto& value : m_moverMap)
	{
		const Mover& mover = value.second;
		int objectIndex = mover.m_objectIndex;

		g_auin.CreateUndo(objectIndex, 8);
	}

	int flags[100] = {};

	for (auto& value : m_moverMap)
	{
		const Mover& mover = value.second;
		ExEdit::Object* object = mover.m_object;

		object->frame_begin = mover.m_frame_begin;
		object->frame_end = mover.m_frame_end;

		flags[object->layer_set] = 1;
	}

	g_auin.RedrawLayers(flags);

	// AviUtl のプレビューウィンドウを再描画する。
	::PostMessage(m_fp->hwnd, AviUtl::FilterPlugin::WindowMessage::Command, 0, 0);

	return TRUE;
}

//--------------------------------------------------------------------
