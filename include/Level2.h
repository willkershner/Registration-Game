// Level2.h
#pragma once
#include "LevelBase.h"

class Level2 : public LevelBase
{
public:
    /**
     * Initializes an instance of level 2
     * 
     * @param parent the parent window, since Level 2 consists of a few specific frames out of the greater app
     */
    Level2(wxWindow* parent);

    /**
    * Loads the specific screen from the level's preset asset library via indexing
    * 
    * @param screen the indexed screen to be loaded
    */
    void LoadScreen(int screen) override;
};
