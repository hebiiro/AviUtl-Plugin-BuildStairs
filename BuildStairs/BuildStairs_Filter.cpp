#include "pch.h"
#include "BuildStairs.h"

//--------------------------------------------------------------------

BOOL func_init(AviUtl::FilterPlugin* fp)
{
	MY_TRACE(_T("func_init()\n"));

	g_auin.initExEditAddress();

	fp->exfunc->add_menu_item(fp, "選択アイテムをずらす", fp->hwnd, Check::BuildStairs, 0, AviUtl::ExFunc::AddMenuItemFlag::None);
	fp->exfunc->add_menu_item(fp, "現在位置を選択アイテムの先頭に移動", fp->hwnd, Check::MoveToBegin, 0, AviUtl::ExFunc::AddMenuItemFlag::None);
	fp->exfunc->add_menu_item(fp, "現在位置を選択アイテムの最後に移動", fp->hwnd, Check::MoveToEnd, 0, AviUtl::ExFunc::AddMenuItemFlag::None);
	fp->exfunc->add_menu_item(fp, "選択アイテムを左に詰める", fp->hwnd, Check::MoveBegin, 0, AviUtl::ExFunc::AddMenuItemFlag::None);
	fp->exfunc->add_menu_item(fp, "選択アイテムを右に詰める", fp->hwnd, Check::MoveEnd, 0, AviUtl::ExFunc::AddMenuItemFlag::None);
	fp->exfunc->add_menu_item(fp, "選択アイテムの左に伸ばす", fp->hwnd, Check::SetBegin, 0, AviUtl::ExFunc::AddMenuItemFlag::None);
	fp->exfunc->add_menu_item(fp, "選択アイテムの右に伸ばす", fp->hwnd, Check::SetEnd, 0, AviUtl::ExFunc::AddMenuItemFlag::None);
	fp->exfunc->add_menu_item(fp, "現在位置の近くにある中間点を削除", fp->hwnd, Check::DeleteMidPt, 0, AviUtl::ExFunc::AddMenuItemFlag::None);

	return TRUE;
}

BOOL func_exit(AviUtl::FilterPlugin* fp)
{
	MY_TRACE(_T("func_exit()\n"));

	return TRUE;
}

BOOL func_WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, AviUtl::EditHandle* editp, AviUtl::FilterPlugin* fp)
{
//	MY_TRACE(_T("func_WndProc(0x%08X, 0x%08X, 0x%08X)\n"), message, wParam, lParam);

	switch (message)
	{
	case AviUtl::FilterPlugin::WindowMessage::Init:
		{
			MY_TRACE(_T("func_WndProc(Init, 0x%08X, 0x%08X)\n"), wParam, lParam);

			break;
		}
	case AviUtl::FilterPlugin::WindowMessage::Exit:
		{
			MY_TRACE(_T("func_WndProc(Exit, 0x%08X, 0x%08X)\n"), wParam, lParam);

			break;
		}
	case AviUtl::FilterPlugin::WindowMessage::Command:
		{
			MY_TRACE(_T("func_WndProc(Command, 0x%08X, 0x%08X)\n"), wParam, lParam);

			if (wParam == 0 && lParam == 0) return TRUE;

			return onCommand(LOWORD(wParam), editp, fp);
		}
	case WM_COMMAND:
		{
			return onCommand(LOWORD(wParam) - fp->MidFilterButton, editp, fp);
		}
	}

	return FALSE;
}

//--------------------------------------------------------------------

LPCSTR track_name[] =
{
	"ﾌﾚｰﾑ",
};

int track_def[] = {     1 };
int track_min[] = { -6000 };
int track_max[] = { +6000 };

LPCSTR check_name[] =
{
	"選択アイテムをずらす",
	"現在位置を選択アイテムの先頭に移動",
	"現在位置を選択アイテムの最後に移動",
	"選択アイテムを左に詰める",
	"選択アイテムを右に詰める",
	"選択アイテムを左に伸ばす",
	"選択アイテムの右に伸ばす",
	"現在位置の近くにある中間点を削除",
};

int check_def[] = { -1, -1, -1, -1, -1, -1, -1, -1 };

EXTERN_C AviUtl::FilterPluginDLL* WINAPI GetFilterTable()
{
	LPCSTR name = "アイテムずらし";
	LPCSTR information = "アイテムずらし 3.0.1 by 蛇色";

	static AviUtl::FilterPluginDLL filter =
	{
		.flag =
			AviUtl::FilterPluginDLL::Flag::AlwaysActive |
			AviUtl::FilterPluginDLL::Flag::DispFilter |
//			AviUtl::FilterPluginDLL::Flag::WindowThickFrame |
//			AviUtl::FilterPluginDLL::Flag::WindowSize |
			AviUtl::FilterPluginDLL::Flag::ExInformation,
		.name = name,
		.track_n = sizeof(track_name) / sizeof(*track_name),
		.track_name = track_name,
		.track_default = track_def,
		.track_s = track_min,
		.track_e = track_max,
		.check_n = sizeof(check_name) / sizeof(*check_name),
		.check_name = check_name,
		.check_default = check_def,
		.func_init = func_init,
		.func_exit = func_exit,
		.func_WndProc = func_WndProc,
		.information = information,
	};

	return &filter;
}

//--------------------------------------------------------------------
