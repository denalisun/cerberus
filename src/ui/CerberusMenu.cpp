#include "CerberusMenu.hpp"
#include "../Globals.hpp"
#include "../Utils.hpp"

CerberusMenu::CerberusMenu() {
    this->m_bIsInsertPressed = false;
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
    ImGui::SetNextWindowSize(ImVec2(400, 300));
    if (ImGui::Begin("Cerberus Mod Tools", reinterpret_cast<bool*>(true), ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize)) {
        ImGui::Text("Placeholder!");

        ImGui::InputText("Map Name", m_cMapBuf, IM_ARRAYSIZE(this->m_cMapBuf));
        if (ImGui::Button("Open Map")) {

            Globals::GPlayerController->SwitchLevel(chartowchar(m_cMapBuf));
        }

        ImGui::End();
    }
}