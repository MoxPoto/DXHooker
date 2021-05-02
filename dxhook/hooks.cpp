#include "mainHook.h"
#include <d3d9.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>
#include <Windows.h>
#include <iostream>

namespace DXHook {
	EndScene oldFunc;
	void* d3d9Device[119];
	LPDIRECT3DDEVICE9 device;
	bool gotDevice = false;

	HRESULT __stdcall EndSceneHook(LPDIRECT3DDEVICE9 pDevice) {
		if (!gotDevice) {
			gotDevice = true;
			device = pDevice;

			ImGui_ImplDX9_Init(device);
		}

		ImGui_ImplWin32_NewFrame();
		ImGui_ImplDX9_NewFrame();

		ImGui::NewFrame();
		DXHook::UpdateImGUI();

		bool showWind = true;

		ImGui::SetNextWindowFocus();

		// test panel
		ImGui::Begin("Shader Modifier");
		ImGui::TextColored(ImVec4(0, 1, 0, 1), "Test Shader Modifier.. dont do shit for now");
		ImGui::End();

		ImGui::EndFrame();

		pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

        HRESULT result = oldFunc(pDevice);
		
        return result;
	}


}