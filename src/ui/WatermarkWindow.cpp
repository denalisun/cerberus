#include "WatermarkWindow.hpp"

WatermarkWindow::WatermarkWindow() {
    m_bIsOpen = true;
}

void WatermarkWindow::Draw() {
    ImGuiIO& io = ImGui::GetIO();

    ImGui::SetNextWindowBgAlpha(0.45f);

    ImVec2 windowPos = ImVec2((CORNER & 1) ? io.DisplaySize.x - DISTANCE_TO_CORNER : DISTANCE_TO_CORNER, (CORNER & 2) ? io.DisplaySize.y - DISTANCE_TO_CORNER : DISTANCE_TO_CORNER);
    ImVec2 windowPosPivot = ImVec2((CORNER & 1) ? 1.0f : 0.0f, (CORNER & 2) ? 1.0f : 0.0f);
    ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always, windowPosPivot);

    if (ImGui::Begin("Watermark", reinterpret_cast<bool*>(true), WATERMARK_WINDOW_FLAGS)) {
        ImGui::Text("Cerberus v0.1a");
        ImGui::Text("Alpha Release - Be aware of bugs and crashes!");
        ImGui::Text("Press INSERT to open the Cerberus Menu!");

        ImGui::Separator();
        
        ImGui::Text("Twitter: @denalisun_");

        ImGui::End();
    }
}