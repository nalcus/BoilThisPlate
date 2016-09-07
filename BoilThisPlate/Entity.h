#ifndef __Entity__
#define __Entity__

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "frameData.h"

enum
{
    LEFT=0, RIGHT
};

class Entity
{
public:
    Entity();

    virtual void update(sf::Time deltaTime)=0;
    virtual void render()=0;
    virtual void receiveCollision(Entity * SourceEntity)=0;

    bool testCollision(Entity* SourceEntity);
    void readFramesDataFromFile(const char * filename);
    int findFrameNamed(std::string);

    sf::Vector2f * getPosition() {return &mPosition;}
    sf::Vector2f * getSize() {return &mSize;}
    std::string * getType() {return &mType;}
    Entity * getParentEntity() {return mParentEntity;}
    sf::Sprite* getSprite() {return &mSprite;}
    bool getActive() {return mActive;}

    void drawBoundingBox();

protected:
    sf::Sprite mSprite;
    sf::Vector2f mPosition;
    sf::Vector2f mVelocity;
    sf::Vector2f mAcceleration;
    sf::Vector2f mSize;
    std::string mType;
    Entity * mParentEntity;
    bool mActive;
    int mAge;

    std::vector<frameData> mFramesData;
};

#endif // __Entity__
