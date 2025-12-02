// Level2.h
#pragma once
#include "LevelBase.h"

class Level2 : public LevelBase
{
public:
    Level2(wxWindow* parent);
    void LoadScreen(int screen) override;
};
