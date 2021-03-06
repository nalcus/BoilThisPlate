//
//  Game.h
//  BoilThisPlate
//
//  Created by Nicholas Alcus on 9/6/16.
//  Copyright © 2016 Nicholas Alcus. All rights reserved.
//

#ifndef Game_h
#define Game_h

#include "tinyxml2.h"

bool checkDocError (tinyxml2::XMLDocument &rDoc);

class Game
{
public:

    // return instance for singleton
    static Game * Instance()
    {
        if (s_pInstance==0)
        {
            s_pInstance=new Game();
            return s_pInstance;
        }
        return s_pInstance;
    }

    void run();
    void drawMarker(int x, int y);
    void drawGrid();

    sf::RenderWindow * getWindow()
    {
        return &mWindow;
    }
    sf::RenderTexture * getRenderTexture()
    {
        return &mRenderTexture;
    }
    int getDisplayWidth() {return DisplayWidth;}
    int getDisplayHeight() {return DisplayHeight;}
    float getScale() {return Scale;}
    void zoomIn();
    void zoomOut();

private:
    Game();
    void processEvents();
    
    void init();
    void update(sf::Time deltaTime);
    void render();
    void updateStatistics(sf::Time elapsedTime);
    static Game* s_pInstance;

    sf::RenderWindow mWindow;
    sf::RenderTexture mRenderTexture;
    sf::Sprite mRenderSprite;
    sf::Sprite mMarkerSprite;


    static const sf::Time	TimePerFrame;
    static const int DisplayWidth;
    static const int DisplayHeight;
    static const bool Fullscreen;
    static const bool VSync;
    
    float Scale;

    sf::Font				mSmallFont;
    sf::Font				mStatisticsFont;
    
    sf::Text				mSmallText;
    
    sf::Text				mStatisticsText;
    sf::Time				mStatisticsUpdateTime;
    std::size_t				mStatisticsNumFrames;
    std::size_t				mStatisticsTotalFrames;

    sf::Texture mPlayerTexture;
    sf::Texture mMarker;
    sf::Texture mMapTileset;


};

typedef Game TheGame;

#endif /* Game_h */
