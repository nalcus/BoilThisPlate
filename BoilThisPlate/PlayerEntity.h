#ifndef __PlayerEntity__
#define __PlayerEntity__

#include "Entity.h"
#include "frameData.h"
#include <vector>

class PlayerEntity : public Entity
{
public:

    PlayerEntity(sf::Texture* pTexture);

    virtual void update(sf::Time deltaTime);
    virtual void render();

    virtual void receiveCollision(Entity * SourceEntity);

private:
    sf::Sprite mSprite;
    int mFacing;
    int mFrame;
    int mFramesUntilNextAnimationFrame;
    int mFramesUntilAction;

    int mFramesUntilHealthy;

    // State bools
    bool mIsFalling;
    bool mIsJumping;
    bool mIsMoving;
    bool mIsAttacking;
    bool mIsDucking;
    bool mIsClimbing;
    bool mIsHurting;
    bool mJustGotHit;
    bool mIsRolling;
};


#endif // __PlayerEntity__
