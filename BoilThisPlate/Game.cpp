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

// singleton stuff
Game* Game::s_pInstance = 0;

//constants
const sf::Time Game::TimePerFrame = sf::seconds(1.f/60.f);
const int Game::DisplayWidth = 1920;
const int Game::DisplayHeight = 1080;
const bool Game::Fullscreen = false;
const bool Game::VSync = true;
const int Game::Scale = 4;

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
, mFont()
, mStatisticsText()
{

    // set vsync to user setting
    mWindow.setVerticalSyncEnabled(VSync);

    // make sure the mouse cursor is not visible
    mWindow.setMouseCursorVisible(false);

    // reseed random number generator
    //reseedRandomizer();

    // set camera offset
    //mCameraOffset=sf::Vector2f(0,0);

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

    mMarkerSprite.setTexture(mMarker);

    mFont.loadFromFile("assets/04B_25__.TTF");
    mStatisticsText.setFont(mFont);
    mStatisticsText.setColor(sf::Color::White);
    mStatisticsText.setPosition(24.f, DisplayHeight-90.f);
    mStatisticsText.setCharacterSize(20);


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
}

void Game::render()
{
    // clear background to white
    mRenderTexture.clear(sf::Color::Black);

    // draw the map first
    //TheMapManager::Instance()->render();

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
                                  "Total Frames = " + toString(mStatisticsTotalFrames)
                                  );

        mStatisticsUpdateTime -= sf::seconds(1.0f);
        mStatisticsNumFrames = 0;
    }

}

void Game::drawMarker(int x, int y)
{
    mMarkerSprite.setPosition((x-4)*Scale,(y-4)*Scale);
    mRenderTexture.draw(mMarkerSprite);
}
