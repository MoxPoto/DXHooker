#ifndef MAINHOOK_H
#define MAINHOOK_H

#include <d3d9.h>
#include "GarrysMod/Lua/Interface.h"
#include <imgui.h>
#include <Windows.h>
#include <vector>

namespace DXHook {
	typedef HRESULT(__stdcall* EndScene)(LPDIRECT3DDEVICE9);

	extern EndScene oldFunc;
	extern void* d3d9Device[119];
	extern HRESULT __stdcall EndSceneHook(LPDIRECT3DDEVICE9 pDevice);

	extern HWND window;
	extern bool gotDevice;
	extern LPDIRECT3DDEVICE9 device;
	extern LONG_PTR originalWNDPROC;

	extern BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam);
	extern HWND GetProcessWindow();
	extern bool GetD3D9Device(void** pTable, size_t Size);
	extern std::vector<int> keyCodes;

	extern inline void error(GarrysMod::Lua::ILuaBase* LUA, const char* str);
	extern int Initialize(GarrysMod::Lua::ILuaBase* LUA); // Used for setting up dummy device, and endscene hook
	extern int Cleanup(GarrysMod::Lua::ILuaBase* LUA); // Used for restoring the EndScene
	extern void UpdateImGUI();
}

#endif MAINHOOK_H