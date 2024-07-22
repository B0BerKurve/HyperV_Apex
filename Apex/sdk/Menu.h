#pragma once
#include "lmcons.h"
#include <WinUser.h>
#include <d3d9.h>
#include <math.h>
#include "memory.h"
#include <string>
#include <filesystem>

#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imgui_elements.h"
#include "../skCrypt.h"
#include "../xorstr.hpp"




__forceinline bool save_config() { // very simple config system thats not very stable sometimes
	FILE* file = NULL;
	errno_t err = fopen_s(&file, skCrypt("config.dat"), skCrypt("wb+"));
	if (err != 0) {
		return false;
	}

	fwrite(&settings, sizeof(settings), 1, file);
	fclose(file);
	return true;
}

__forceinline bool load_config() { // very simple config system thats not very stable sometimes
	FILE* file = NULL;
	errno_t err = fopen_s(&file, skCrypt("config.dat"), skCrypt("rb"));
	if (err != 0) {
		return false;
	}

	fread(&settings, sizeof(settings), 1, file);
	fclose(file);
	return true;
}


void RenderMenu()
{
		if (GetAsyncKeyState(VK_INSERT) & 1) { settings.showmenu = !settings.showmenu; }
		if (settings.showmenu)
		{ 
		
			static auto current_tab = 0;
			static auto button_height = 30.0f;
			static auto config_height = 20.0f;
			static auto flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse;
			static const char* hitboxes[]{ "Head", "Neck" ,"Pelvis" };
			static std::string combo_preview = ""; // 


			ImGui::SetNextWindowSize(ImVec2(450, 350));
			ImGui::Begin(skCrypt("apex legends"), nullptr, flags);
			if (ImGui::BeginChild(1, { ImGui::GetContentRegionAvail().x * 0.20f, ImGui::GetContentRegionAvail().y }, true)) {
				if (ImGui::Button(skCrypt("visuals"), { ImGui::GetContentRegionAvail().x, button_height })) { current_tab = 0; }
				if (ImGui::Button(skCrypt("aimbot"), { ImGui::GetContentRegionAvail().x, button_height })) { current_tab = 1; }
				if (ImGui::Button(skCrypt("other"), { ImGui::GetContentRegionAvail().x, button_height })) { current_tab = 2; }
				ImGui::SetCursorPosY(ImGui::GetContentRegionAvail().y * 1.35f);
				ImGui::Separator();
				if (ImGui::Button(skCrypt("save"), { ImGui::GetContentRegionAvail().x, config_height })) { save_config(); }
				if (ImGui::Button(skCrypt("load"), { ImGui::GetContentRegionAvail().x, config_height })) { load_config(); }
				ImGui::EndChild();
			}
			ImGui::SameLine();
			switch (current_tab) {
			case 0:
				if (ImGui::BeginChild(3, { ImGui::GetContentRegionAvail().x * 0.5f, ImGui::GetContentRegionAvail().y }, true)) {
					
					ImGui::AnimatedCheckbox(skCrypt("Player Box"), &settings.visuals_box);  //0.05f, 0.05f, 0.05f, 0.54f
					ImGui::SliderInt(skCrypt("Box Style"), &settings.box_mode, 1, 3);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip(skCrypt("1 = Cornered\n2 = Box\n3 = Filled Box"));
					//ImGui::AnimatedCheckbox(skCrypt("Tracer"), &settings.Snapline);
					  ImGui::AnimatedCheckbox(skCrypt("Seer-Q Health"), &settings.SeerHealth); 
					 ImGui::AnimatedCheckbox(skCrypt("Player Armor"), &settings.ArmorBox);
					//ImGui::AnimatedCheckbox(skCrypt("Health Bar"), &settings.healthbar);
					//ImGui::AnimatedCheckbox(skCrypt("Skelton"), &settings.Skelton);
					  /*ImGui::AnimatedCheckbox(skCrypt("Player Glow"), &settings.Player_glow);*/
					  ImGui::AnimatedCheckbox(skCrypt("Show Distance"), &settings.DistanceESP);
					 ImGui::SliderFloat(skCrypt("Distance"), &settings.DistanceESPFL, 40.f, 500.);
					ImGui::EndChild();
				}
				ImGui::SameLine();
				ImGui::BeginGroup(); {
					if (ImGui::BeginChild(4, { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * 0.45f }, true)) {
						ImGui::Text(skCrypt("items"));
					/*	ImGui::AnimatedCheckbox(skCrypt("Loot ESP"), &settings.LootESP);
						ImGui::AnimatedCheckbox(skCrypt("Loot Deadbox"), &settings.LootDeadbox);*/
						//ImGui::AnimatedCheckbox(skCrypt("Loot Glow"), &settings.LootGlow);
					   /* ImGui::SliderFloat(skCrypt("Distance Item"), &settings.LootDistance, 40.f, 500.);*/
						ImGui::EndChild(); 
					}
					/*if (ImGui::BeginChild(5, {ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y}, true)) {
						ImGui::Text(("other"));
						//ImGui::Separator();
						ImGui::AnimatedCheckbox(skCrypt("Player Glow"), &settings.glow);
						ImGui::AnimatedCheckbox(skCrypt("Item Glow"), &settings.itemglow);  
						ImGui::AnimatedCheckbox(skCrypt("Deathbox Glow"), &settings.deathbox);
						ImGui::AnimatedCheckbox(skCrypt("Radar"), &settings.Radar);;
						if (ImGui::Button("Map Radar", ImVec2(100, 30)))
						{

							uintptr_t pLocal = Memory.Read<uintptr_t>(g_base + local_player);

							int defTeam = Memory.Read<int>(pLocal + 0x044c);

							for (uintptr_t i = 0; i <= 80000; i++)
							{
								Memory.Write<int>(pLocal + 0x044c, 1);
							}

							for (uintptr_t i = 0; i <= 80000; i++)
							{
								Memory.Write<int>(pLocal + 0x044c, defTeam);
							}
						}
						ImGui::EndChild(); 
					}
					ImGui::EndGroup();*/
				}
				break;
			case 1:
				if (ImGui::BeginChild(6, { ImGui::GetContentRegionAvail().x * 0.5f, ImGui::GetContentRegionAvail().y }, true)) {
					ImGui::Text(skCrypt("aimbot"));
				    ImGui::Separator();
					ImGui::AnimatedCheckbox(skCrypt("Aimbot"), &settings.aimbot);
					ImGui::SliderFloat(skCrypt("Smoothing"), &settings.AimBotSmooth, 1.f, 40);

					//ImGui::AnimatedCheckbox(skCrypt("Show Fov"), &settings.misc_showfov);
					//ImGui::SliderInt(skCrypt("RCS"), &settings.rcs, 1.f, 100);
			/*		ImGui::SliderFloat(skCrypt("Fov"), &settings.rcs, 30.f, 200.f);*/

					//ImGui::Text(skCrypt("")); ImGui::SameLine(); HotkeyButton(aimkey, ChangeKey, keystatus); ImGui::SameLine(); ImGui::Text(("Key"));
					//ImGui::Combo(("hitbox"), &settings.aim_bone, hitboxes, sizeof(hitboxes) / sizeof(*hitboxes));
					ImGui::AnimatedCheckbox(skCrypt("Show Fov"), &settings.misc_showfov);
					ImGui::SliderFloat(skCrypt("Fov"), &settings.aimbotfov, 30.f, 200.f);
					ImGui::EndChild();
				}
				ImGui::SameLine();
				if (ImGui::BeginChild(7, ImGui::GetContentRegionAvail(), true)) {
					ImGui::Text(skCrypt("other"));
					//ImGui::AnimatedCheckbox(("Norecoil"), &settings.Norecoil);
					//ImGui::SliderInt(("Norecoil_intensity"), &settings.norecoil_intensity, 20.f, 100.f);
					ImGui::Separator();
					ImGui::EndChild();
				}
				break;
			case 2:
				if (ImGui::BeginChild(8, { ImGui::GetContentRegionAvail().x * 0.5f, ImGui::GetContentRegionAvail().y }, true)) {
					ImGui::Text(skCrypt("colours"));
					ImGui::Separator();


					ImGui::EndChild();
				}
				ImGui::SameLine();
				if (ImGui::BeginChild(9, { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y }, true)) {
					ImGui::Text(skCrypt("misc"));
					ImGui::Separator();
					//ImGui::AnimatedCheckbox(_("hide from capture"), &settings.hide_from_capture); // public
					ImGui::EndChild();
				}
				break;
			}

			ImGui::End();
			//ImGui::EndFrame();
		}
	}
//}



