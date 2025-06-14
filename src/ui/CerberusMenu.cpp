#include "CerberusMenu.hpp"
#include "../Globals.hpp"
#include "../Utils.hpp"
#include "imgui_stdlib.h"

CerberusMenu::CerberusMenu() {
    this->m_bIsInsertPressed = false;
    this->m_sMapBuf = "Athena_Terrain";
}

void CerberusMenu::Tick() {
    if ((GetAsyncKeyState(VK_INSERT) & 0x8000) && !this->m_bIsInsertPressed) {
        this->m_bIsOpen = !this->m_bIsOpen;
        this->m_bIsInsertPressed = true;
    }
    
    if (!(GetAsyncKeyState(VK_INSERT) & 0x8000) && this->m_bIsInsertPressed)
        this->m_bIsInsertPressed = false;
}

void CerberusMenu::Draw() {
    ImGui::SetNextWindowSize(ImVec2(800, 600));
    if (ImGui::Begin("Cerberus Mod Tools", reinterpret_cast<bool*>(true), ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize)) {
        ImGui::InputText("Map Name", &m_sMapBuf);
        ImGui::SameLine();
        ImGui::Checkbox("Force Athena_GameMode", &m_bShouldAddGameMode);
        if (ImGui::Button("Open Map")) {
            if (m_bShouldAddGameMode) {
                std::string mapWithMode = m_sMapBuf + "?game=/Game/Athena/Athena_GameMode.Athena_GameMode_C";
                Globals::GPlayerController->SwitchLevel(chartowchar(mapWithMode.c_str()));
            } else
                Globals::GPlayerController->SwitchLevel(chartowchar(m_sMapBuf.c_str()));
        }
        
        ImGui::End();
    }
}