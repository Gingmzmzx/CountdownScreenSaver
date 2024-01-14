#include <Windows.h>

// 定义全局变量来保存最后一次输入时间
LASTINPUTINFO g_lastInputInfo;

// 定时器回调函数
VOID CALLBACK TimerCallback(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	// 获取最后一次输入时间
	g_lastInputInfo.cbSize = sizeof(LASTINPUTINFO);
	GetLastInputInfo(&g_lastInputInfo);
	
	// 计算当前时间与最后一次输入时间的差值（以毫秒为单位）
	DWORD elapsedTime = GetTickCount() - g_lastInputInfo.dwTime;
	
	// 如果超过30分钟（30分钟 = 30 * 60 * 1000 毫秒）
	if (elapsedTime > 30 * 60 * 1000)
	{
		system("D:\\electron\\electron.exe D:\\electron\\ScreenSaver");
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	char pathProgram[MAX_PATH];
	DWORD lengthA = GetModuleFileName(NULL, pathProgram, MAX_PATH);
	if (lengthA > 0 && lengthA < MAX_PATH)
	{
		
	}
	else
	{
		return 0;
	}
	
	// 获取转换后的字符串长度
	int length = MultiByteToWideChar(CP_UTF8, 0, pathProgram, -1, NULL, 0);
	
	// 分配内存保存转换后的字符串
	wchar_t* wstr = new wchar_t[length];
	
	// 进行字符串转换
	MultiByteToWideChar(CP_UTF8, 0, pathProgram, -1, wstr, length);
	
	// 使用转换后的字符串
	LPCWSTR path = wstr;
	
	// 释放内存
	delete[] wstr;
	
	HKEY hKey;
	LPCSTR subKey = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";
	LPCSTR valueName = L"CountdownScreenSaver";
//	LPCWSTR path = pathProgram;
	
	// 打开注册表项
	if (RegOpenKeyEx(HKEY_CURRENT_USER, subKey, 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS)
	{
		// 设置注册表值
		RegSetValueEx(hKey, valueName, 0, REG_SZ, (BYTE*)path, (wcslen(path) + 1) * sizeof(wchar_t))
		
		// 关闭注册表项
		RegCloseKey(hKey);
	}
	
	// 创建一个定时器，每隔一分钟触发一次
	SetTimer(NULL, 0, 60 * 1000, TimerCallback);
	
	// 消息循环
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return 0;
}
