#include "GarrysMod/Lua/Interface.h"
#include "mainHook.h"
#include <Windows.h>
#include <iostream>
#include <sstream>
#include "../detours.h"

namespace DXHook {
	inline void error(GarrysMod::Lua::ILuaBase* LUA, const char* str) {
		LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		LUA->GetField(-1, "print");
		LUA->PushString(str);

		LUA->Call(1, 0);
		LUA->Pop();
	}

	int Initialize(GarrysMod::Lua::ILuaBase* LUA) { // Used for setting up dummy device, and endscene hook
        AllocConsole();

        FILE* pFile = nullptr;

        freopen_s(&pFile, "CONOUT$", "w", stdout); // cursed way to redirect stdout to our own console


        HMODULE hDLL;
        hDLL = GetModuleHandleA("d3d9.dll"); // Attempt to locate the d3d9 dll that gmod loaded

        if (hDLL == NULL) {
            error(LUA, "Unable to locate d3d9.dll in the loaded memory?");
            return 0;
        }


        if (GetD3D9Device(d3d9Device, sizeof(d3d9Device)))
        {
            //hook stuff using the dumped addresses


            std::stringstream hexLoc;
            hexLoc.setf(std::ios_base::hex, std::ios_base::basefield);

            hexLoc << std::string("EndScene functions address: ") << (uintptr_t)d3d9Device[42];

            error(LUA, hexLoc.str().c_str()); // it's called "error" but its more of a print

            oldFunc = (EndScene)d3d9Device[42]; // cast our void* address to a function description btw 42 is the EndScene index

            DetourTransactionBegin(); // use MS detours to detour our EndScene 
            DetourUpdateThread(GetCurrentThread());
            DetourAttach(&(PVOID&)oldFunc, EndSceneHook); // if im gon be straight honest i took this from guidedhacking and I don't entirely get
            // the "&(PVOID&)"

            LONG lError = DetourTransactionCommit(); // execute it
            if (lError != NO_ERROR) {
                MessageBox(HWND_DESKTOP, "failed to detour", "puffy", MB_OK);
                return FALSE;
            }
            else {
                error(LUA, "Successfully got EndScene address, converted to function--detoured and ready");
            }

        }
        
        return 0;
	} 

    int Cleanup(GarrysMod::Lua::ILuaBase* LUA) {
        FreeConsole();

        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourDetach(&(PVOID&)oldFunc, EndSceneHook);

        LONG lError = DetourTransactionCommit();
        if (lError != NO_ERROR) {
            MessageBox(HWND_DESKTOP, "failed to revert detour", "puffy", MB_OK);
            return FALSE;
        }
        else {
            error(LUA, "Reverted detour on EndScene..");
        }

        return 0;
    } // Used for restoring the EndScene
}