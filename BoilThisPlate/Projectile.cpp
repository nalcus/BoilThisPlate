#include "Projectile.h"
#include <SFML/Graphics.hpp>
#include "Game.h"
#include <iostream>

Projectile::Projectile(sf::Texture* pTexture
                       , float newSizeX, float newSizeY
                       , float newPositionX, float newPositionY
                       , float newVelocityX, float newVelocityY
                       , Entity * newParentEntity)
{
    mType="PROJECTILE";
    mSize=sf::Vector2f(newSizeX,newSizeY);
    mPosition=sf::Vector2f(newPositionX,newPositionY);
    mVelocity=sf::Vector2f(newVelocityX,newVelocityY);
    mParentEntity=newParentEntity;
    // ignore everything

    // we're alive!!
    mActive=true;
}

void Projectile::update(sf::Time deltaTime)

{
    Entity::update(deltaTime);
    if (mAge++>2)
    {
        mActive=false;
    }
}

void Projectile::render()
{

}
