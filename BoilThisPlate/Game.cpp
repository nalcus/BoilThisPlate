//
//  Game.cpp
//  BoilThisPlate
//
//  Created by Nicholas Alcus on 9/6/16.
//  Copyright © 2016 Nicholas Alcus. All rights reserved.
//

#include <SFML/Graphics.hpp>
#include <iostream>

#include "StringHelp.h"
#include "Game.h"
#include "PlayerEntity.h"
#include "EntityManager.h"
#include "Randomizer.h"
#include "Camera.hpp"
#include "MapManager.h"

// singleton stuff
Game* Game::s_pInstance = 0;

//constants
const sf::Time Game::TimePerFrame = sf::seconds(1.f/60.f);
const int Game::DisplayWidth = 1920;
const int Game::DisplayHeight = 1080;
const bool Game::Fullscreen = false;
const bool Game::VSync = true;

bool checkDocError (tinyxml2::XMLDocument &rDoc)
{
    if (rDoc.ErrorID()!=0)
    {
        std::cout << "doc.ErrorID(): " << rDoc.ErrorID() << std::endl;
        return false;
    }
    return true;
}


Game::Game()
: mWindow(sf::VideoMode(DisplayWidth, DisplayHeight), "BoilThisPlate", (Fullscreen)?sf::Style::Fullscreen:sf::Style::Close)
, mStatisticsUpdateTime()
, mStatisticsNumFrames(0)
, mStatisticsTotalFrames(0)
, mPlayerTexture()

, mMarker ()
, mMarkerSprite()
, mStatisticsFont()
, mStatisticsText()
{

    // set vsync to user setting
    mWindow.setVerticalSyncEnabled(VSync);

    // make sure the mouse cursor is not visible
    mWindow.setMouseCursorVisible(false);

    // reseed random number generator
    reseedRandomizer();

    mRenderTexture.create(DisplayWidth, DisplayHeight);
    mRenderSprite.setTexture(mRenderTexture.getTexture());

    if (!mPlayerTexture.loadFromFile("assets/hoodie_spritesheet.png"))
    {
        std::cout << "didn't load file hoodie" << std::endl;
    }

    if (!mMarker.loadFromFile("assets/marker.png"))
    {
        std::cout << "didn't load file marker" << std::endl;
    }
    
    if (!mMapTileset.loadFromFile("assets/platformertiles.png"))
    {
        std::cout << "didn't load file platformertiles" << std::endl;
    }
    
    if (!TheMapManager::Instance()->init())
    {
        std::cout << "map init failed!" << std::endl;
    }
    
    TheMapManager::Instance()->setTileset(&mMapTileset);

    mMarkerSprite.setTexture(mMarker);
    
    mSmallFont.loadFromFile("assets/00TT.TTF");
    mSmallText.setFont(mSmallFont);
    mSmallText.setColor(sf::Color::White);
    mSmallText.setPosition(24.f, DisplayHeight-90.f);
    mSmallText.setCharacterSize(16);

    mStatisticsFont.loadFromFile("assets/04B_25__.TTF");
    mStatisticsText.setFont(mStatisticsFont);
    mStatisticsText.setColor(sf::Color::White);
    mStatisticsText.setPosition(24.f, DisplayHeight-180.f);
    mStatisticsText.setCharacterSize(20);
    
    Scale = 4.f;

    // add Player

    PlayerEntity *tempent=new PlayerEntity(&mPlayerTexture);
    TheEntityManager::Instance()->pushEntity(tempent);

}

void Game::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while (mWindow.isOpen())
    {
        // clock.restart() rests the clock and returns the time since last restart
        sf::Time elapsedTime = clock.restart();
        timeSinceLastUpdate += elapsedTime;
        while (timeSinceLastUpdate > TimePerFrame)
        {
            timeSinceLastUpdate -= TimePerFrame;
            processEvents();
            update(TimePerFrame);
        }
        updateStatistics(elapsedTime);
        render();

    }

}


// process os events
void Game::processEvents()
{
    sf::Event event;
    while (mWindow.pollEvent(event))
    {
        if (event.type== sf::Event::Closed)
        {

            mWindow.close();
        }


        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
            {
                mWindow.close();
            }


        }
    }
}

void Game::update(sf::Time deltaTime)
{
    // update each game entity by time slice
    TheEntityManager::Instance()->update(deltaTime);
    
    TheCamera::Instance()->update();
}

void Game::zoomIn()
{
    Scale+=0.03f;
    if (Scale>10) Scale=10;
}

void Game::zoomOut()
{
    Scale-=0.03f;
    if (Scale<2) Scale=2;
}

void Game::render()
{
    // clear background to white
    mRenderTexture.clear(sf::Color::Black);

    // draw the map first
    TheMapManager::Instance()->render();
    
    // grid over map
    //drawGrid();

    // draw the entities next
    TheEntityManager::Instance()->render();

    // draw the statistical text
    mRenderTexture.draw(mStatisticsText);

    // display the rendertexture
    mRenderTexture.display();

    // set the rendersprite scale
    mRenderSprite.setScale(1,1);

    // draw the rendersprite to the main window
    mWindow.draw(mRenderSprite);

    // display the main window
    mWindow.display();

}

void Game::updateStatistics(sf::Time elapsedTime)
{
    mStatisticsUpdateTime += elapsedTime;
    mStatisticsNumFrames ++;
    mStatisticsTotalFrames ++;


    if (mStatisticsUpdateTime >= sf::seconds(1.0f))
    {
        mStatisticsText.setString(
                                  "BoilThisPlate\nFrames / Second = " + toString(mStatisticsNumFrames) + "\n" +
                                  "Time / Update = " + toString(mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames) + "us\n" +
                                  "Total Frames = " + toString(mStatisticsTotalFrames) +
                                  "\nScale = " + toString(Scale)
                                  +"\nCameraOffsetX = " + toString(TheCamera::Instance()->getOffset().x)
                                  +"\nCameraOffsetY = " + toString(TheCamera::Instance()->getOffset().y)
                                  );

        mStatisticsUpdateTime -= sf::seconds(1.0f);
        mStatisticsNumFrames = 0;
    }

}

void Game::drawMarker(int x, int y)
{
    float sx=((x-TheCamera::Instance()->getOffset().x)*Scale);
    float sy=((y-TheCamera::Instance()->getOffset().y)*Scale);
    mMarkerSprite.setPosition(sx-4,
                              sy-4);
    mRenderTexture.draw(mMarkerSprite);
    mSmallText.setPosition(sx-4, sy-32);
    mSmallText.setString(
                              "World: ("+toString(x)+", "+toString(y)+")\n"+
                              "Screen: ("+toString(sx)+", "+toString(sy)+")\n"
                              );
    mRenderTexture.draw(mSmallText);
}

void Game::drawGrid()
{
    int gridSize=32;
    sf::RectangleShape boxy = sf::RectangleShape();
    int lBound=0;
    int rBound=TheMapManager::Instance()->getMapWidth()*gridSize;
    int tbound=16;
    int bbound=TheMapManager::Instance()->getMapHeight()*gridSize;
    
    
    for (int i=lBound;i<rBound;i+=gridSize)
    {
        for (int j=tbound; j<bbound;j+=gridSize)
    {
        int x=(i-TheCamera::Instance()->getOffset().x)*Scale;
        int y=(j-TheCamera::Instance()->getOffset().y)*Scale;
        if (x<DisplayWidth&&y<DisplayHeight)
              {
    boxy.setPosition(sf::Vector2f(x,y));
    boxy.setOutlineColor(sf::Color(255,255,255,128.f));
    boxy.setOutlineThickness(1);
    boxy.setFillColor(sf::Color(0,0,0,0));
    boxy.setSize(sf::Vector2f(gridSize*Scale,gridSize*Scale));
    getRenderTexture()->draw(boxy);
       
        mSmallText.setPosition(x,y);
        mSmallText.setString(
                             "W:("+toString(i)+", "+toString(j)+")\n"+
                             "S:("+toString(x)+", "+toString(y)+")\n"
                             );
        mRenderTexture.draw(mSmallText);
        }
    }
    }
}
