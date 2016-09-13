//
//  Camera.cpp
//  BoilThisPlate
//
//  Created by Nicholas Alcus on 9/7/16.
//  Copyright © 2016 Nicholas Alcus. All rights reserved.
//

#include "Camera.hpp"
#include "Game.h"

TheCamera * TheCamera::s_pInstance = 0;

Camera::Camera()
{
    mOffset.x=-(Game::Instance()->getDisplayWidth()/Game::Instance()->getScale()*0.5);
    mOffset.y=-Game::Instance()->getDisplayHeight()/Game::Instance()->getScale()*0.5;
    mFocalPoint=sf::Vector2f(0,0);
}

void Camera::update()
{
    float mx=mFocalPoint.x-(Game::Instance()->getDisplayWidth()/Game::Instance()->getScale()*0.5);
    float my=mFocalPoint.y-(Game::Instance()->getDisplayHeight()/Game::Instance()->getScale()*0.5);
    if (mOffset.x!=mx) mOffset.x+=0.1*(mx-mOffset.x);
    if (mOffset.y!=my) mOffset.y+=0.1*(my-mOffset.y);
    

}

void Camera::setFocalPoint(float x,float y)
{
    mFocalPoint=sf::Vector2f(x,y);
}



