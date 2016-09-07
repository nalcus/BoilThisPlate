#ifndef __Projectile__
#define __Projectile__

#include <SFML/Graphics.hpp>
#include "Entity.h"

class Projectile : public Entity
{
public:

Projectile(sf::Texture* pTexture
                       , float newSizeX, float newSizeY
                       , float newPositionX, float newPositionY
                       , float newVelocityX, float newVelocityY
                       , Entity * newParentEntity);

    virtual void update(sf::Time deltaTime);
    virtual void render();
    virtual void receiveCollision(Entity * SourceEntity) {}

};








#endif // __Projectile__
