#pragma once
#include "UIWindow.hpp"

class CerberusMenu : public UIWindow {
private:
    bool m_bIsInsertPressed = false;
    char* m_cMapBuf;
public:
    CerberusMenu();
    void Draw() override;
    void Tick() override;
};