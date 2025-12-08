// Level3.h
#pragma once
#include "LevelBase.h"
#include <functional>

class Level3 : public LevelBase
{
public:
    /**
     * Initializes an instance of level 3
     * 
     * @param parent the parent window, since Level 3 consists of a few specific frames out of the greater app
     */
    Level3(wxWindow* parent);

    /**
     * Loads the specific screen from the level's preset asset library via indexing
     * 
     * @param screen the indexed screen to be loaded
     */
    void LoadScreen(int screen) override;

private:
    /**
     * Simulates a wifi outage by removing the player's ability to interact with the page for a randomized period of time.
     * 
     * @param onFinished callback void function needed to complete the process
     */
    void SimulateWifiOutage(std::function<void()> onFinished);

    /**
     * Simulates a page-loading scenario by switching to a loading asset for a randomized period of time.
     * 
     * @param onFinished callback void function needed to complete the process
     */
    void SimulateLoadingOnly(std::function<void()> onFinished);

    bool m_isLoading = false;
};
