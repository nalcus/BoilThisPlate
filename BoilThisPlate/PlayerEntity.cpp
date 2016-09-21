
#include "PlayerEntity.h"
#include "Randomizer.h"
#include "Game.h"
#include <iostream>
#include "frameData.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include "Projectile.h"
#include "EntityManager.h"
#include "MapManager.h"
#include "Camera.hpp"



using namespace std;

PlayerEntity::PlayerEntity(sf::Texture* pTexture)
    : mFacing(getRandomInt()%2)
    , mFrame(0)
    , mFramesUntilNextAnimationFrame(0)
    , mFramesUntilAction(getRandomInt()%600)
    ,mIsFalling(false)
    ,mIsJumping(false)
    ,mIsMoving(false)
    ,mIsAttacking(false)
    ,mIsDucking(false)
    ,mIsClimbing(false)
    ,mIsHurting(false)
    ,mJustGotHit(false)
    ,mIsRolling(false)
{
    // set entity type
    mType = "PLAYER";

    // player has no parent entity
    mParentEntity=0;

    // we're alive!!
    mActive=true;
    mAge=0;

    mFramesUntilHealthy=0;

    // initial position and momentum
    mPosition.x=96*16;
    mPosition.y=16*16;
    mVelocity=sf::Vector2f(0.f,0.f);
    mAcceleration=sf::Vector2f(0.f,0.f);
    mSize=sf::Vector2f(10.f,mDefaultHeight);

    // set texture to argument
    mSprite.setTexture(*pTexture);


    readFramesDataFromFile("assets/hoodie_spritesheet.xml");

    mFrame=findFrameNamed("idle_1.png");
}


void PlayerEntity::update(sf::Time deltaTime)
{


    Entity::update(deltaTime);

    sf::Vector2f totalForces=sf::Vector2f(0.f,0.f);

    int height=TheGame::Instance()->getDisplayHeight();
    //int horizon=25*16;

    // get the tile under foot.
    
    
    
    

    // get keyboard input

    bool bA=sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    bool bD=sf::Keyboard::isKeyPressed(sf::Keyboard::D);
    bool bDuckKeyPressed=sf::Keyboard::isKeyPressed(sf::Keyboard::S);
    bool bJumpKeyPressed=sf::Keyboard::isKeyPressed(sf::Keyboard::W);
    bool bAttackKeyPressed=sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
    bool bZoomOut=sf::Keyboard::isKeyPressed(sf::Keyboard::LBracket);
    bool bZoomIn=sf::Keyboard::isKeyPressed(sf::Keyboard::RBracket);

    if (bZoomOut) {TheGame::Instance()->zoomOut();}
    if (bZoomIn) {TheGame::Instance()->zoomIn();}


    const float playerRunForce = 0.05f;
    const float playerJumpForce = -2.3f;
    const int climbAgainDelay = 5;
    static int framesSinceClimbing=climbAgainDelay;

    framesSinceClimbing++;
    mFramesUntilHealthy--;

    if (lowerleft==-1&&lowerright==-1) {
        mIsFalling=true;
    } else {
        mIsFalling=false;
        mVelocity.y=0;
        mAcceleration.y=0;
        mPosition.y=16*((int)mPosition.y/16);
    }

    // we only want to roll until we first hit the ground
    if (!mIsFalling&&mIsRolling)
    {
        mIsRolling=false;
    }

    // we just got injured so run injury script
    if (mJustGotHit)
    {
        mFramesUntilHealthy=120;
        mVelocity=sf::Vector2f(0.f,0.f);
        mAcceleration=sf::Vector2f(0.f,0.f);
        mIsClimbing=false;
        framesSinceClimbing=0;
        totalForces.y+=0.33*playerJumpForce;
        float lateralForce=(mFacing==RIGHT)?playerJumpForce:-playerJumpForce;
        totalForces.x+=0.33*lateralForce;
        mJustGotHit=false;
        mIsRolling=true;
    }


    mIsHurting=(mFramesUntilHealthy>1);


    // ducking is true if we are on the ground and pressing duck. false in either is not true.
    mIsDucking=!mIsFalling&&bDuckKeyPressed&&!mIsClimbing;


    // if we are no longer pressing the attack key and the attack animation is over, set attacking to false
    if(mFrame!=findFrameNamed("attack_1a.png")
       &&mFrame!=findFrameNamed("attack_1b.png")
       &&mFrame!=findFrameNamed("duckattack_1a.png")
       &&mFrame!=findFrameNamed("duckattack_1b.png"))
    {
        mIsAttacking=false;
    }

    static bool bAttackKeyWasPressed=false;

    // check for attack being pressed then attack!!
    if(bAttackKeyPressed&&!bAttackKeyWasPressed&&!mIsAttacking&&!mIsClimbing)
    {
        mIsAttacking=true;
        // in case we were rolling before we won't be now
        mIsRolling=false;
        mFramesUntilNextAnimationFrame=10;
        mFrame=(mIsDucking)
            ?findFrameNamed("duckattack_1a.png")
            :findFrameNamed("attack_1a.png"); // attack frame or duck attack frame

        float projectileOffset=16.f+(mSize.x*0.5f);
        // DO ATTACK
        Projectile * tempsword=new Projectile(0
                                              , 32.f
                                              ,16.f
                                              , mPosition.x+((mFacing==RIGHT)?projectileOffset:-projectileOffset)
                                              ,16.f+mPosition.y-(mSize.y)+((mIsDucking)?16:0)
                                              ,0.f
                                              ,0.f
                                              ,this);
        TheEntityManager::Instance()->pushEntity(tempsword);


    }
    bAttackKeyWasPressed=bAttackKeyPressed;

    // if we are not falling and jump key is not pressed
    if (!bJumpKeyPressed&&!mIsFalling)
    {
        mIsJumping=false;
    }

    static bool bJumpKeyWasPressed=false;
    // delay until the wall can be gripped again



    if (bJumpKeyPressed&&(!mIsFalling||mIsClimbing)&&!bJumpKeyWasPressed&&!mIsDucking&&framesSinceClimbing>climbAgainDelay)
    {

        mIsJumping=true;
        // in case we were rolling before we won't be now
        mIsRolling=false;
        if (mIsClimbing)
        {
            mIsClimbing=false;
            framesSinceClimbing=0;
            totalForces.y+=0.80*playerJumpForce;
            float lateralForce=(mFacing==RIGHT)?playerJumpForce:-playerJumpForce;
            totalForces.x+=0.33*lateralForce;
            mFacing=(mFacing==RIGHT)?LEFT:RIGHT;

        }
        else
        {
            totalForces.y+=playerJumpForce;
        }
    }
    bJumpKeyWasPressed=bJumpKeyPressed;



    // move left
    if (bA&&!bD&&!mIsDucking&&!mIsClimbing&&framesSinceClimbing>climbAgainDelay)
    {
        if (!mIsFalling)
            {
                mFacing=LEFT;
        }
        totalForces.x-=playerRunForce;
    }

    // move right
    if (bD&&!bA&&!mIsDucking&&!mIsClimbing&&framesSinceClimbing>climbAgainDelay)
    {
               if (!mIsFalling)
            {
        mFacing=RIGHT;
            }
        totalForces.x+=playerRunForce;
    }
    
    // clip left wall
    if ((midleft>-1||upperleft>-1)&&(totalForces.x<0||mVelocity.x<0||mAcceleration.x<0))
    {
        totalForces.x=0;
        mVelocity.x=0;
        mAcceleration.x=0;
        mPosition.x=(16*((int)(mPosition.x)/16))+mSize.x*0.5;
    }
    
    if ((midright>-1||upperright>-1)&&(totalForces.x>0||mVelocity.x>0||mAcceleration.x>0))
    {
        totalForces.x=0;
        mVelocity.x=0;
        mAcceleration.x=0;
        mPosition.x=(16*((int)(mPosition.x)/16+1))-mSize.x*0.5;
    }



    float playerHeight = (mIsDucking)?32:48;


    const float gravityForce = 0.125;


    // *********************************** ANIMATION *********************************************************

    enum
    {
        IDLE=0,
        IDLE_ATTACK,
        MOVING,
        MOVING_ATTACK,
        FALLING,
        FALLING_ATTACK,
        DUCK,
        DUCK_ATTACK,
        CLIMB
    };

    int state=(mIsClimbing)?CLIMB:(mIsDucking)?(mIsAttacking?DUCK_ATTACK:DUCK):((mIsFalling)?(mIsAttacking?FALLING_ATTACK:FALLING):
              (mVelocity.x<-0.1f||mVelocity.x>0.1f)?(mIsAttacking?MOVING_ATTACK:MOVING) :(mIsAttacking?IDLE_ATTACK:IDLE));


    const int FIRST_ATTACK_FRAMES = 7;
    const int SECOND_ATTACK_FRAMES = 7;


    switch (state)
    {
    case FALLING:
        totalForces.y+=gravityForce;
        mFrame=(mIsRolling)?findFrameNamed("roll_1.png"):findFrameNamed("jump_1.png");
        mFramesUntilNextAnimationFrame=0;
        break;
    case FALLING_ATTACK:
        totalForces.y+=gravityForce;
        if ((mFrame==findFrameNamed("duckattack_1a.png")
             ||mFrame==findFrameNamed("attack_1a.png"))
            &&mFramesUntilNextAnimationFrame<1)
        {
            mFrame=findFrameNamed("attack_1b.png");
            mFramesUntilNextAnimationFrame=FIRST_ATTACK_FRAMES;
        }

        else if ((mFrame==findFrameNamed("duckattack_1b.png")||mFrame==findFrameNamed("attack_1b.png"))&&mFramesUntilNextAnimationFrame<1)
        {
            mFrame=findFrameNamed("jump_1.png"); // falling
            mFramesUntilNextAnimationFrame=SECOND_ATTACK_FRAMES;
        }
        break;
    case IDLE:
        mFrame=findFrameNamed("idle_1.png"); // idle
        mFramesUntilNextAnimationFrame=0;
        break;
    case IDLE_ATTACK:
        if ((mFrame==findFrameNamed("duckattack_1a.png")||mFrame==findFrameNamed("attack_1a.png"))&&mFramesUntilNextAnimationFrame<1)
        {
            mFrame=findFrameNamed("attack_1b.png");
            mFramesUntilNextAnimationFrame=FIRST_ATTACK_FRAMES;
        }

        else if ((mFrame==findFrameNamed("duckattack_1b.png")||mFrame==findFrameNamed("attack_1b.png"))&&mFramesUntilNextAnimationFrame<1)
        {
            mFrame=findFrameNamed("idle_1.png"); //  idle
            mFramesUntilNextAnimationFrame=SECOND_ATTACK_FRAMES;
        }
        break;
    case MOVING:

        // switch running frames every n frames
        if (mFramesUntilNextAnimationFrame<1)
        {
            // running frames
            mFramesUntilNextAnimationFrame=8;
            mFrame++;
            if (mFrame>findFrameNamed("run_1c.png")||mFrame<findFrameNamed("run_1a.png"))
            {
                mFrame=findFrameNamed("run_1a.png"); // running
            }
        }
        break;
    case MOVING_ATTACK:
        if ((mFrame==findFrameNamed("duckattack_1a.png")||mFrame==findFrameNamed("attack_1a.png"))&&mFramesUntilNextAnimationFrame<1)
        {
            mFrame=findFrameNamed("attack_1b.png");
            mFramesUntilNextAnimationFrame=FIRST_ATTACK_FRAMES;
        }

        else if ((mFrame==findFrameNamed("duckattack_1b.png")||mFrame==findFrameNamed("attack_1b.png"))&&mFramesUntilNextAnimationFrame<1)
        {
            mFrame=findFrameNamed("run_1a.png"); // running
            mFramesUntilNextAnimationFrame=SECOND_ATTACK_FRAMES ;
        }
        break;
    case DUCK:
        mFrame=findFrameNamed("duck_1.png"); // duck
        mFramesUntilNextAnimationFrame=0;
        break;
    case DUCK_ATTACK:
        if ((mFrame==findFrameNamed("duckattack_1a.png")||mFrame==findFrameNamed("attack_1a.png"))&&mFramesUntilNextAnimationFrame<1)
        {
            mFrame=findFrameNamed("duckattack_1b.png");
            mFramesUntilNextAnimationFrame=FIRST_ATTACK_FRAMES;
        }

        else if ((mFrame==findFrameNamed("duckattack_1b.png")||mFrame==findFrameNamed("duckattack_1b.png"))&&mFramesUntilNextAnimationFrame<1)
        {
            mFrame=findFrameNamed("duck_1.png"); //  duck
            mFramesUntilNextAnimationFrame=SECOND_ATTACK_FRAMES;
        }
        break;
    case CLIMB:
    {
        // pick a randon frame 2 or 3
        if (mFrame!=findFrameNamed("climb_1a.png")&&mFrame!=findFrameNamed("climb_1b.png"))
        {
             // in case we were rolling before we won't be now

            mFrame=(getRandomInt()%2>0)?findFrameNamed("climb_1a.png"):findFrameNamed("climb_1b.png");
        }
        break;
    }

    }


    // not falling anymore go to idle frame
    // or if not running


    //loco-mote!!
    // friction
    mAcceleration*=0.9f;
    mAcceleration+=totalForces;
    mVelocity*=0.9f;
    mVelocity+=mAcceleration;
    mPosition+=mVelocity;


    mFramesUntilNextAnimationFrame--;

    // Move camera to focus player position
    TheCamera::Instance()->setFocalPoint(mPosition.x,mPosition.y);
}

void PlayerEntity::render()
{
    int x=0, y=0, w=0, h=0, oX=0, oY=0; // these we'll eventually read from xml
    int spriteOffsetX=0, spriteOffsetY=mIsDucking?-24:-28;

    // set height based on standing or ducking
    mSize.y=mIsDucking?mDuckingHeight:mDefaultHeight;

    // set pixels to game scale!
    mSprite.setScale(TheGame::Instance()->getScale(),TheGame::Instance()->getScale());

    if (mFacing==RIGHT)
    {
        spriteOffsetX = -mSize.x*0.5;

        x=mFramesData.at(mFrame).x;
        y=mFramesData.at(mFrame).y;
        w=mFramesData.at(mFrame).w;
        h=mFramesData.at(mFrame).h;
        oX=mFramesData.at(mFrame).oX;
        oY=mFramesData.at(mFrame).oY;
    }

    else if (mFacing==LEFT)
    {
        spriteOffsetX=(mSize.x*0.5)-mFramesData.at(mFrame).w;

        x=mFramesData.at(mFrame).x+mFramesData.at(mFrame).w;
        y=mFramesData.at(mFrame).y;
        w=0-mFramesData.at(mFrame).w;
        h=mFramesData.at(mFrame).h;
        oX=-mFramesData.at(mFrame).oX+mFramesData.at(mFrame).mX;
        oY=mFramesData.at(mFrame).oY;
    }

    sf::IntRect rect = sf::IntRect(x,y,w,h);
    mSprite.setTextureRect(rect);

    drawBoundingBox();
    if (!(mIsHurting&&(mAge%10<5))) {

        mSprite.setPosition((mPosition.x+spriteOffsetX-TheCamera::Instance()->getOffset().x)*TheGame::Instance()->getScale(),(mPosition.y+spriteOffsetY-TheCamera::Instance()->getOffset().y)*TheGame::Instance()->getScale());
    TheGame::Instance()->getRenderTexture()->draw(mSprite);
    }
    TheGame::Instance()->drawMarker(mPosition.x, mPosition.y);
}

void PlayerEntity::receiveCollision(Entity * SourceEntity)
{
    std::string En="ENEMY";

    if (*SourceEntity->getType()==En&&!mIsHurting)
    {
        mJustGotHit=true;
    }
}
