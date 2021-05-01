#include "mainHook.h"
#include <d3d9.h>

namespace DXHook {
	EndScene oldFunc;
	void* d3d9Device[119];


	HRESULT __stdcall EndSceneHook(LPDIRECT3DDEVICE9 pDevice) {
        D3DRECT test = { 0, 0, 150, rand() % 55 };
        pDevice->Clear(1, &test, D3DCLEAR_TARGET, D3DCOLOR_ARGB(125, 255, 0, 0), 0.5f, 0);


        HRESULT result = oldFunc(pDevice);



        return result;
	}
}