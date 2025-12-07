// Level3.h
#pragma once
#include "LevelBase.h"
#include <functional>

class Level3 : public LevelBase
{
public:
    Level3(wxWindow* parent);
    void LoadScreen(int screen) override;

private:
    void SimulateWifiOutage(std::function<void()> onFinished);
    void SimulateLoadingOnly(std::function<void()> onFinished);

    bool m_isLoading = false;
};
