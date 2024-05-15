#pragma once

// my includes
#include <Libraries/Dumper.hpp>
#include <Libraries/imgui/imgui.h>
#include <Core/Variables.h>
#include <Utils/SDK.h>
#include <Utils/Themes.h>
#include <Utils/Utils.h>
 
using namespace Variables;

namespace Fns {

	bool RenderESPSnapline(ImColor color, Vector2 origin) {

		ImVec2 screenPos = ImVec2(origin.x, origin.y);
		ImVec2 dest;

		switch (CheatMenuVariables::PlayersSnaplineType)
		{
		case 0:
			dest = ImVec2(System::ScreenSize.x / 2, System::ScreenSize.y);
			break;
		case 1:
			dest = ImVec2(System::ScreenSize.x / 2, System::ScreenSize.y / 2);
			break;
		case 2:
			dest = ImVec2(System::ScreenSize.x / 2, 0);
			break;
		}
		ImGui::GetForegroundDrawList()->AddLine(dest, screenPos, color, 1.5f);

		return true;
	}

	bool RenderESPBox(ImColor color, Vector2 bottom, Vector2 top) {

		const float height = bottom.y - top.y;
		const float width = height * 0.2f;
		const auto left = static_cast<int>(top.x - width);
		const auto right = static_cast<int>(top.x + width);

		if (CheatMenuVariables::PlayersBoxFilled) {
			color.Value.w = 0.3f;
			ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(left, top.y), ImVec2(right, bottom.y), color);
			color.Value.w = 1;
			ImGui::GetForegroundDrawList()->AddRect(ImVec2(left, top.y), ImVec2(right, bottom.y), color);
		}
		else
		{
			ImGui::GetForegroundDrawList()->AddRect(ImVec2(left, top.y), ImVec2(right, bottom.y), color, 0.0f, 0, 1.0f);
		}
		
		/*
		float boxWidth = 30;
		float boxHeight = 45;

		ImVec2 boxTopLeft(origin.x - boxWidth / 2, origin.y - boxHeight / 2);
		ImVec2 boxBottomRight(origin.x + boxWidth / 2, origin.y + boxHeight / 2);

		if (CheatMenuVariables::PlayersBoxFilled) {
			color.Value.w = 0.3f;
			ImGui::GetBackgroundDrawList()->AddRectFilled(boxTopLeft, boxBottomRight, color);
			color.Value.w = 1;
			ImGui::GetBackgroundDrawList()->AddRect(boxTopLeft, boxBottomRight, color);
		}
		else {
			ImGui::GetBackgroundDrawList()->AddRect(boxTopLeft, boxBottomRight, color);
		}*/

		return true;
	}


}

void DrawMouse() {
	if(!CheatMenuVariables::ShowMouse) return;

	ImColor color = CheatMenuVariables::RainbowMouse ? CheatVariables::RainbowColor : CheatMenuVariables::MouseColor;

	switch (CheatMenuVariables::MouseType) {
	case 0:
		ImGui::GetForegroundDrawList()->AddCircleFilled(ImGui::GetMousePos(), 4, color);
		break;
	case 1:
		Utils::DrawOutlinedTextForeground(gameFont, ImVec2(System::MousePos.x, System::MousePos.y), 13.0f, color, false, "X");
		break;
	case 2:
		if (!ImGui::GetIO().MouseDrawCursor) {
			ImGui::GetIO().MouseDrawCursor = true;
		}
		break;
	}
}

void DrawCrosshair() {
	if (CheatMenuVariables::Crosshair) {
		ImColor color = CheatMenuVariables::RainbowCrosshair ? CheatVariables::RainbowColor : CheatMenuVariables::CrosshairColor;
		switch (CheatMenuVariables::CrosshairType)
		{
		case 0:
			ImGui::GetForegroundDrawList()->AddLine(ImVec2(System::ScreenCenter.x - CheatMenuVariables::CrosshairSize, System::ScreenCenter.y), ImVec2((System::ScreenCenter.x - CheatMenuVariables::CrosshairSize) + (CheatMenuVariables::CrosshairSize * 2), System::ScreenCenter.y), color, 1.2f);
			ImGui::GetForegroundDrawList()->AddLine(ImVec2(System::ScreenCenter.x, System::ScreenCenter.y - CheatMenuVariables::CrosshairSize), ImVec2(System::ScreenCenter.x, (System::ScreenCenter.y - CheatMenuVariables::CrosshairSize) + (CheatMenuVariables::CrosshairSize * 2)), color, 1.2f);
			break;
		case 1:
			ImGui::GetForegroundDrawList()->AddCircle(ImVec2(System::ScreenCenter.x, System::ScreenCenter.y), CheatMenuVariables::CrosshairSize, color, 100, 1.2f);
			break;
		}
	}
}

void DrawMenu()
{
	Themes::ImGuiThemeKio(true);

	if (CheatMenuVariables::ShowInspector) {
		Utils::DrawInspector();
	}

	if (ImGui::Begin(Prefix.c_str(), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings))
	{
		ImGui::SetWindowPos(ImVec2(500, 500), ImGuiCond_Once);
		ImGui::SetWindowSize(ImVec2(600, 300), ImGuiCond_Once);
		static MenuTab tabIndex = TAB_VISUALS;
		ImGui::SameLine();
		if (ImGui::Button("Visual"))
		{
			tabIndex = TAB_VISUALS;
		}
		ImGui::SameLine();
		if (ImGui::Button("Exploits"))
		{
			tabIndex = TAB_EXPLOITS;
		}
		ImGui::SameLine();
		if (ImGui::Button("Misc"))
		{
			tabIndex = TAB_MISC;
		}
		if (DEBUG) {
			ImGui::SameLine();
			if (ImGui::Button("Developer"))
			{
				tabIndex = TAB_DEV;
			}
		}
		ImGui::Separator();
		ImGui::Spacing();
		switch (tabIndex) {
			case TAB_VISUALS: {

				{ // ESP
					ImGui::Checkbox("Ghost Snapline", &CheatMenuVariables::PlayersSnapline);
					ImGui::SameLine();
					ImGui::ColorEdit3("##PlayersSnaplineColor", (float*)&CheatMenuVariables::PlayersSnaplineColor, ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoInputs);
					if(ImGui::IsItemHovered()) ImGui::SetTooltip("Color of the Ghost snapline");
					ImGui::SameLine();
					ImGui::Checkbox("##RGB3", &CheatMenuVariables::RainbowPlayersSnapline);
					if (ImGui::IsItemHovered()) ImGui::SetTooltip("Toggle rainbow color on the Ghost snapline");
					ImGui::Text("Snapline Type");
					ImGui::SameLine();
					ImGui::SliderInt("##PlayersSnaplineType", &CheatMenuVariables::PlayersSnaplineType, 0, 2);
				}

				{ // Players Box
					ImGui::Checkbox("Ghost Box", &CheatMenuVariables::PlayersBox);
					ImGui::SameLine();
					ImGui::ColorEdit3("##PlayersBoxColor", (float*)&CheatMenuVariables::PlayersBoxColor, ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoInputs);
					if (ImGui::IsItemHovered()) ImGui::SetTooltip("Color of the ghost box");
					ImGui::SameLine();
					ImGui::Checkbox("##RGB5", &CheatMenuVariables::RainbowPlayersBox);
					if (ImGui::IsItemHovered()) ImGui::SetTooltip("Toggle rainbow color on the Ghost box");
					ImGui::SameLine();
					ImGui::Checkbox("##Filled", &CheatMenuVariables::PlayersBoxFilled);
					if (ImGui::IsItemHovered()) ImGui::SetTooltip("Fill the Ghost box");
				}				
				
				{ // Items Snapline
					ImGui::Checkbox("Items Snapline", &CheatMenuVariables::ItemsSnapline);
					ImGui::SameLine();
					ImGui::ColorEdit3("##ItemsSnaplineColor", (float*)&CheatMenuVariables::ItemsSnaplineColor, ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoInputs);
					if (ImGui::IsItemHovered()) ImGui::SetTooltip("Color of the Items snapline");
					ImGui::SameLine();
					ImGui::Checkbox("##RGB12", &CheatMenuVariables::RainbowItemsSnapline);
					if (ImGui::IsItemHovered()) ImGui::SetTooltip("Toggle rainbow color on the Items snapline");
				} 
				
				{ // Items Box
					ImGui::Checkbox("Items Box", &CheatMenuVariables::ItemsBox);
					ImGui::SameLine();
					ImGui::ColorEdit3("##ItemsBoxColor", (float*)&CheatMenuVariables::ItemsBoxColor, ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoInputs);
					if (ImGui::IsItemHovered()) ImGui::SetTooltip("Color of the Items box");
					ImGui::SameLine();
					ImGui::Checkbox("##RGB57", &CheatMenuVariables::RainbowItemsBox);
					if (ImGui::IsItemHovered()) ImGui::SetTooltip("Toggle rainbow color on the Items box");
				}

			}
			case TAB_EXPLOITS: {

				ImGui::Text("Game timescale");
				ImGui::SliderFloat("##timescale", &CheatMenuVariables::GameSpeed, 0.0f, 100.0f);
				ImGui::SameLine(); 
				if(ImGui::Button("Set Timescale")) {
					GameFunctions::UnityEngine_Time__set_timeScale(CheatMenuVariables::GameSpeed);
				}	
				ImGui::SameLine(); Utils::HelpMarker("Change the game speed");
				if(ImGui::Button("Reset Timescale")) {
					CheatMenuVariables::GameSpeed = 1.0f;
					GameFunctions::UnityEngine_Time__set_timeScale(CheatMenuVariables::GameSpeed);
				}

				break;
			}
			case TAB_MISC: {

				{ // Render Things
					ImGui::Checkbox("Show Watermark", &CheatMenuVariables::Watermark);
				}

				{ // Mouse things
					ImGui::Checkbox("Draw mouse", &CheatMenuVariables::ShowMouse);
					ImGui::SameLine();
					ImGui::ColorEdit3("##MouseColor", (float*)&CheatMenuVariables::MouseColor, ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoInputs);
					if (ImGui::IsItemHovered()) ImGui::SetTooltip("Color of the mouse");
					ImGui::SameLine();
					ImGui::Checkbox("##RGB1", &CheatMenuVariables::RainbowMouse);
					if (ImGui::IsItemHovered()) ImGui::SetTooltip("Toggle rainbow color on the mouse");
					ImGui::Text("Mouse Type");
					ImGui::SameLine();
					ImGui::SliderInt("##Mouse type", &CheatMenuVariables::MouseType, 0, 1);
				}

				{ // Crosshair
					ImGui::Checkbox("Crosshair", &CheatMenuVariables::Crosshair);
					ImGui::SameLine();
					ImGui::ColorEdit3("##CrosshairColor", (float*)&CheatMenuVariables::CrosshairColor, ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoInputs);
					if (ImGui::IsItemHovered()) ImGui::SetTooltip("Color of the crosshair");
					ImGui::SameLine();
					ImGui::Checkbox("##RGB2", &CheatMenuVariables::RainbowCrosshair);
					if (ImGui::IsItemHovered()) ImGui::SetTooltip("Toggle rainbow color on the crosshair");
					ImGui::Text("Crosshair Size");
					ImGui::SameLine();
					ImGui::SliderFloat("##Crosshair Size", &CheatMenuVariables::CrosshairSize, 0.1f, 10.0f);
					ImGui::Text("Crosshair Type");
					ImGui::SameLine();
					ImGui::SliderInt("##Crosshair type", &CheatMenuVariables::CrosshairType, 0, 1);
				}

				{ // Camera fov
					ImGui::Checkbox("Camera Fov Changer", &CheatMenuVariables::CameraFovChanger);
					ImGui::Text("Camera Custom FOV");
					ImGui::SameLine();
					ImGui::SliderFloat("##Camera Custom FOV", &CheatMenuVariables::CameraCustomFOV, 0.1f, 300.0f);
				}

				break;
			}
			case TAB_DEV: {

				ImGui::Checkbox("Enable Developer Options", &CheatMenuVariables::EnableDeveloperOptions);

				if (CheatMenuVariables::EnableDeveloperOptions)
				{
					ImGui::Indent();
					ImGui::Checkbox("Show Inspector", &CheatMenuVariables::ShowInspector);
					ImGui::Checkbox("Show Lua Editor", &Lua::ShowEditor);
					ImGui::Spacing();

					{ // test things
						ImGui::Text("Test Objects");
						ImGui::SameLine();
						ImGui::InputTextWithHint("##SearchObject", "Name of a component...", CheatVariables::TestObjects::Name, 200);

						ImGui::Checkbox("Test Objects Chams", &CheatVariables::TestObjects::Chams);
						ImGui::SameLine();
						ImGui::Checkbox("Test Objects Snapline", &CheatVariables::TestObjects::Snapline);
						ImGui::Checkbox("Test Objects Box", &CheatVariables::TestObjects::Box);
						ImGui::SameLine();
						ImGui::Checkbox("Test Objects Aimbot", &CheatVariables::TestObjects::Aimbot);
					}
					ImGui::Unindent();

				}
				break;
			}
		}
		ImGui::End();
	}
}

void CheatsLoop()
{
	DWORD currentTime = GetTickCount64(); 

	if (!Variables::System::InitIL2Cpp) return;


	if (CheatMenuVariables::CameraFovChanger)
	{
		Unity::CCamera* CameraMain = Unity::Camera::GetMain();
		if (CameraMain != nullptr) {
			CameraMain->CallMethodSafe<void*>("set_fieldOfView", CheatMenuVariables::CameraCustomFOV);
		}
	}

	if (CheatMenuVariables::EnableDeveloperOptions)	{

		for (int i = 0; i < CheatVariables::TestObjects::List.size(); i++) {
			Unity::CGameObject* curObject = CheatVariables::TestObjects::List[i];
			if (!curObject) continue;

			Unity::CTransform* objectTransform = curObject->GetTransform();
			if(!objectTransform) continue;

			Unity::Vector3 objectPos = objectTransform->GetPosition();

			Unity::Vector3 headPos = objectPos; // HEAD OF THE PLAYER <-- THIS TRICK IS USELESS IF YOU KNOW THE HEAD POSITION
			headPos.y -= CheatMenuVariables::FakeHeadPosDiff;
			Unity::Vector3 feetPos = objectPos; // FEET OF THE PLAYER <-- THIS TRICK IS USELESS IF YOU KNOW THE FEET POSITION
			feetPos.y += CheatMenuVariables::FakeHeadPosDiff;

			Vector2 top, bottom;
			if (!Utils::WorldToScreen(headPos, bottom)) continue;

			if (CheatVariables::TestObjects::Snapline)
			{
				ImColor color = CheatVariables::TestObjects::SnaplineColor;

				Fns::RenderESPSnapline(color, bottom);
			}

			if (CheatVariables::TestObjects::Box) {
				ImColor color = CheatVariables::TestObjects::BoxColor;

				if (!Utils::WorldToScreen(headPos, top)) continue;
				Fns::RenderESPBox(color, bottom, top);
			}

		}
	}

	for (int i = 0; i < CheatVariables::CursedItems.size(); i++)
	{
		Unity::CGameObject* curPlayer = CheatVariables::CursedItems[i];
		if (!curPlayer) continue;

		Unity::CTransform* playerTransform = curPlayer->GetTransform();
		if (!playerTransform) continue;

		Unity::Vector3 playerPos = playerTransform->GetPosition();

		Unity::Vector3 headPos = playerPos; // HEAD OF THE PLAYER <-- THIS TRICK IS USELESS IF YOU KNOW THE HEAD POSITION
		headPos.y -= 10;
		Unity::Vector3 feetPos = playerPos; // FEET OF THE PLAYER <-- THIS TRICK IS USELESS IF YOU KNOW THE FEET POSITION
		feetPos.y += 10;

		Vector2 top, bottom;
		if (!Utils::WorldToScreen(headPos, bottom)) continue;

		if (CheatMenuVariables::ItemsSnapline)
		{
			ImColor color = CheatMenuVariables::RainbowItemsSnapline ? CheatVariables::RainbowColor : CheatMenuVariables::ItemsSnaplineColor;

			Fns::RenderESPSnapline(color, bottom);
		}

		if (CheatMenuVariables::ItemsBox) {
			ImColor color = CheatMenuVariables::RainbowItemsBox ? CheatVariables::RainbowColor : CheatMenuVariables::ItemsBoxColor;

			if (Utils::WorldToScreen(headPos, top)) {
				Fns::RenderESPBox(color, bottom, top);
			}
		}
	}

	for (int i = 0; i < CheatVariables::Ghosts.size(); i++)
	{
		Unity::CGameObject* curPlayer = CheatVariables::Ghosts[i];
		if (!curPlayer) continue;

		Unity::CTransform* playerTransform = curPlayer->GetTransform();
		if (!playerTransform) continue;

		Unity::Vector3 playerPos = playerTransform->GetPosition();

		Unity::Vector3 headPos = playerPos; // HEAD OF THE PLAYER <-- THIS TRICK IS USELESS IF YOU KNOW THE HEAD POSITION
		headPos.y -= CheatMenuVariables::FakeHeadPosDiff;
		Unity::Vector3 feetPos = playerPos; // FEET OF THE PLAYER <-- THIS TRICK IS USELESS IF YOU KNOW THE FEET POSITION
		feetPos.y += CheatMenuVariables::FakeFeetPosDiff;

		Vector2 top, bottom;
		if (!Utils::WorldToScreen(headPos, bottom)) continue;

		if (CheatMenuVariables::PlayersSnapline)
		{
			ImColor color = CheatMenuVariables::RainbowPlayersSnapline ? CheatVariables::RainbowColor : CheatMenuVariables::PlayersSnaplineColor;

			Fns::RenderESPSnapline(color, bottom);
		}

		if (CheatMenuVariables::PlayersBox) {
			ImColor color = CheatMenuVariables::RainbowPlayersBox ? CheatVariables::RainbowColor : CheatMenuVariables::PlayersBoxColor;

			color = CheatVariables::TargetPlayer == curPlayer ? CheatVariables::TargetPlayerColor : color;

			if (Utils::WorldToScreen(headPos, top)) {
				Fns::RenderESPBox(color, bottom, top);
			}
		}

	}

	if (currentTime - CheatVariables::LastTick > 5)
	{
		CheatVariables::LastTick = currentTime;
	}
}

void CacheManager()
{
	while (true)
	{
		if (!Variables::System::InitIL2Cpp)
			continue;

		void* m_pThisThread = IL2CPP::Thread::Attach(IL2CPP::Domain::Get());
		if (!m_pThisThread) continue;

		// here maybe you can check for a localplayer, or look the Objects Cache method

		try {
			if (CheatMenuVariables::EnableDeveloperOptions) {
				Utils::ObjectsCache(&CheatVariables::TestObjects::List, CheatVariables::TestObjects::Name);
			}
			// check the method in Utils.cpp to understand how find localplayer with that variable
			Utils::ObjectsCache(&CheatVariables::Ghosts, "GhostAI");

			Utils::ObjectsCache(&CheatVariables::CursedItems, "CursedItem");
		}
		catch (const std::exception& e) {
			printf(e.what());
		}

		IL2CPP::Thread::Detach(m_pThisThread);
		Sleep(2000);
	}
}