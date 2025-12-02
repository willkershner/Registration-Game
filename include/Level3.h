// Level3.h
#pragma once
#include "LevelBase.h"

class Level3 : public LevelBase
{
public:
    Level3(wxWindow* parent);
    void LoadScreen(int screen) override;
};
