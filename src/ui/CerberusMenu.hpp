#pragma once
#include "UIWindow.hpp"

class CerberusMenu : public UIWindow {
private:
    bool m_bIsInsertPressed = false;

    bool m_bShouldAddGameMode = false;
    std::string m_sMapBuf;
public:
    CerberusMenu();
    void Draw() override;
    void Tick() override;
};