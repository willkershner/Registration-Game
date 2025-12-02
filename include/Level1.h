// Level1.h
#pragma once
#include "LevelBase.h"

class Level1 : public LevelBase
{
public:
    Level1(wxWindow* parent);
    void LoadScreen(int screen) override;
};
