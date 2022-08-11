#include "pch.h"
#include "StairsBuilder.h"
#include "BuildStairs.h"

//--------------------------------------------------------------------

StairsBuilder::StairsBuilder(AviUtl::EditHandle* editp, AviUtl::FilterPlugin* fp, int command)
{
	m_editp = editp;
	m_fp = fp;
	m_command = command;
}

BOOL StairsBuilder::playVoice()
{
	LPCTSTR voice = 0;

	switch (m_command)
	{
	case Check::BuildStairs: voice = _T("BuildStairs"); break;
	case Check::MoveToBegin: voice = _T("MoveToBegin"); break;
	case Check::MoveToEnd: voice = _T("MoveToEnd"); break;
	case Check::MoveBegin: voice = _T("MoveBegin"); break;
	case Check::MoveEnd: voice = _T("MoveEnd"); break;
	case Check::SetBegin: voice = _T("SetBegin"); break;
	case Check::SetEnd: voice = _T("SetEnd"); break;
	}

	if (!voice)
		return FALSE;

	// フォルダ名を取得する。
	TCHAR folderName[MAX_PATH] = {};
	::GetModuleFileName(m_fp->dll_hinst, folderName, MAX_PATH);
	::PathRemoveExtension(folderName);
	MY_TRACE_TSTR(folderName);

	// wav ファイルのパスを取得する。
	TCHAR wavFileName[MAX_PATH] = {};
	::StringCbPrintf(wavFileName, sizeof(wavFileName), _T("%s\\%s.wav"), folderName, voice);
	MY_TRACE_TSTR(wavFileName);

	// ファイルが存在するなら
	if (::GetFileAttributes(wavFileName) != INVALID_FILE_ATTRIBUTES)
	{
		// wav ファイルを再生する。
		::PlaySound(wavFileName, 0, SND_FILENAME | SND_ASYNC);
	}

	return TRUE;
}

BOOL StairsBuilder::getSelectObjects()
{
	MY_TRACE(_T("StairsBuilder::getSelectObjects()\n"));

	// 選択オブジェクトの数を取得する。
	int c = g_auin.GetSelectedObjectCount();
	MY_TRACE_INT(c);

	// 選択オブジェクトが存在しないなら
	if (!c)
	{
		// カレントオブジェクトのインデックスを取得する。
		int objectIndex = g_auin.GetCurrentObjectIndex();
		MY_TRACE_INT(objectIndex);

		if (objectIndex < 0)
			return FALSE;

		m_selectObjects.push_back(objectIndex);

		return TRUE;
	}

	for (int i = 0; i < c; i++)
	{
		// 選択オブジェクトのインデックスを取得する。
		int objectIndex = g_auin.GetSelectedObject(i);
		MY_TRACE_INT(objectIndex);

		m_selectObjects.push_back(objectIndex);
	}

	return TRUE;
}

BOOL StairsBuilder::createMoverMap()
{
	MY_TRACE(_T("StairsBuilder::createMoverMap(%d)\n"), m_command);

	if (!getSelectObjects())
	{
		// エラーメッセージを出して終了する。
		TCHAR text[MAX_PATH] = {};
		::StringCbPrintf(text, sizeof(text), _T("選択アイテムがありません"));
		::MessageBox(m_fp->hwnd, text, _T("BuildStairs"), MB_OK | MB_ICONWARNING);

		return FALSE;
	}

	int stairFrame = m_fp->track[Track::Frame];
	int stairStep = 0;

	for (int objectIndex : m_selectObjects)
	{
		MY_TRACE_INT(objectIndex);
		if (objectIndex < 0) continue;

		// 選択オブジェクトを取得する。
		ExEdit::Object* object = g_auin.GetObject(objectIndex);
		MY_TRACE_HEX(object);
		if (!object) continue;

		if (m_moverMap.find(object) != m_moverMap.end())
			continue; // すでに mover が作成済みならスキップする。

		int midptLeader = object->index_midpt_leader;
		MY_TRACE_INT(midptLeader);

		// 中間点があるなら
		if (midptLeader >= 0)
		{
			objectIndex = midptLeader; // 中間点元のオブジェクト ID を取得

			while (objectIndex >= 0)
			{
				// オブジェクトを取得する。
				ExEdit::Object* object = g_auin.GetObject(objectIndex);
				MY_TRACE_HEX(object);
				if (!object) break;

				int midptLeader2 = object->index_midpt_leader;
				MY_TRACE_INT(midptLeader2);
				if (midptLeader2 != midptLeader) break;

				addMover(objectIndex, object, stairFrame * stairStep);

				objectIndex = g_auin.GetNextObjectIndex(objectIndex);
			}
		}
		// 中間点がないなら
		else
		{
			addMover(objectIndex, object, stairFrame * stairStep);
		}

		stairStep++;
	}

	return TRUE;
}

BOOL StairsBuilder::addMover(int objectIndex, ExEdit::Object* object, int frame_offset)
{
	int currentFrame = g_auin.GetExEditCurrentFrame();

	int frame_begin = object->frame_begin;
	int frame_end = object->frame_end;

	switch (m_command)
	{
	case Check::BuildStairs:
		{
			frame_begin += frame_offset;
			frame_end += frame_offset;

			break;
		}
	case Check::MoveBegin:
		{
			frame_begin = currentFrame;
			frame_end = currentFrame + (object->frame_end - object->frame_begin);

			break;
		}
	case Check::MoveEnd:
		{
			frame_end = currentFrame - 1;
			frame_begin = currentFrame - 1 - (object->frame_end - object->frame_begin);

			break;
		}
	case Check::SetBegin:
		{
			frame_begin = currentFrame;

			break;
		}
	case Check::SetEnd:
		{
			frame_end = currentFrame - 1;

			break;
		}
	}

	if (frame_begin >= frame_end)
		return FALSE;

	m_moverMap[object] = Mover(objectIndex, object, frame_begin, frame_end);

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

BOOL StairsBuilder::applyMoverMap()
{
	MY_TRACE(_T("StairsBuilder::applyMoverMap()\n"));

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

	playVoice();

	// Undo を作成する。

	g_auin.PushUndo();

	for (auto& value : m_moverMap)
	{
		const Mover& mover = value.second;
		int objectIndex = mover.m_objectIndex;

		g_auin.CreateUndo(objectIndex, 8);
	}

	// 実際にオブジェクトを動かし、レイヤーを再計算する。

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

	// AviUtl を再描画する。
	::PostMessage(m_fp->hwnd, AviUtl::FilterPlugin::WindowMessage::Command, 0, 0);

	return TRUE;
}

//--------------------------------------------------------------------

BOOL StairsBuilder::move()
{
	MY_TRACE(_T("StairsBuilder::move(%d)\n"), m_command);

	// オブジェクトを動かしたあとの状態を構築する。
	createMoverMap();

	// 実際にオブジェクトを動かす。
	return applyMoverMap();
}

BOOL StairsBuilder::moveTo()
{
	MY_TRACE(_T("StairsBuilder::moveTo(%d)\n"), m_command);

	if (!getSelectObjects())
	{
		// エラーメッセージを出して終了する。
		TCHAR text[MAX_PATH] = {};
		::StringCbPrintf(text, sizeof(text), _T("選択アイテムがありません"));
		::MessageBox(m_fp->hwnd, text, _T("BuildStairs"), MB_OK | MB_ICONWARNING);

		return FALSE;
	}

	int frame = -1;

	for (int objectIndex : m_selectObjects)
	{
		MY_TRACE_INT(objectIndex);
		if (objectIndex < 0) continue;

		// 選択オブジェクトを取得する。
		ExEdit::Object* object = g_auin.GetObject(objectIndex);
		MY_TRACE_HEX(object);
		if (!object) continue;

		if (m_command == Check::MoveToBegin)
		{
			if (frame < 0)
				frame = object->frame_begin;
			else
				frame = std::min(frame, object->frame_begin);
		}
		else if (m_command == Check::MoveToEnd)
		{
			if (frame < 0)
				frame = object->frame_end + 1;
			else
				frame = std::max(frame, object->frame_end + 1);
		}
	}

	// フレームが無効なら
	if (frame < 0)
	{
		// エラーメッセージを出して終了する。
		TCHAR text[MAX_PATH] = {};
		::StringCbPrintf(text, sizeof(text), _T("現在フレームを動かせませんでした"));
		::MessageBox(m_fp->hwnd, text, _T("BuildStairs"), MB_OK | MB_ICONWARNING);

		return FALSE;
	}

	playVoice();

	g_auin.SetExEditCurrentFrame(frame);
	::InvalidateRect(g_auin.GetExEditWindow(), 0, FALSE);
	m_fp->exfunc->set_frame(m_editp, frame);
	::PostMessage(m_fp->hwnd, AviUtl::FilterPlugin::WindowMessage::Command, 0, 0);

	return TRUE;
}

//--------------------------------------------------------------------
