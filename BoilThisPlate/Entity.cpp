#include "Entity.h"
#include "Game.h"
#include "tinyxml2.h"
#include <iostream>

Entity::Entity()
{
    mType="INVALID";
    mParentEntity=0;
    mActive=false;
    mAge=0;
}

bool Entity::testCollision(Entity * SourceEntity)
{
    const int l1=mPosition.x-(mSize.x/2);
    const int r1=mPosition.x+(mSize.x/2);
    const int t1=mPosition.y-mSize.y;
    const int b1=mPosition.y;

    const int l2=SourceEntity->getPosition()->x-(SourceEntity->getSize()->x/2);
    const int r2=SourceEntity->getPosition()->x+(SourceEntity->getSize()->x/2);
    const int t2=SourceEntity->getPosition()->y-SourceEntity->getSize()->y;
    const int b2=SourceEntity->getPosition()->y;

    if (l1<r2&&r1>l2&&t1<b2&&b1>t2)
    {
        return true;
    }

    return false;


}

void Entity::readFramesDataFromFile(const char * filename)
{


    // read different frame values from file.
    mFramesData.clear();
    tinyxml2::XMLDocument doc;
    doc.LoadFile( filename ); // open the map
    if(!checkDocError(doc))
    {
        return ;
    }
    tinyxml2::XMLElement* mapElement = doc.FirstChildElement( "TextureAtlas" );
    if (mapElement==0)
    {
        std::cout << "textureatlas not found!" << std::endl;
        return;
    }
    tinyxml2::XMLElement* nextElement = mapElement->FirstChildElement("sprite");
    if (mapElement==0)
    {
        std::cout << "sprite not found!" << std::endl;
        return;
    }
    std::string n;
    int x, y, w, h, oX, oY, oW, oH, mX;
    float pX, pY;
    // we need to find the largest width image to set our mX- mirroring horiz offset
    int largestWidth=0;
    while (nextElement!=0)
    {
        n = nextElement->Attribute("n");
        nextElement->QueryIntAttribute( "x", &x);
        nextElement->QueryIntAttribute( "y", &y);
        nextElement->QueryIntAttribute( "w", &w);
        nextElement->QueryIntAttribute( "h", &h);
        nextElement->QueryFloatAttribute("pX", &pX);
        nextElement->QueryFloatAttribute("pY", &pY);
        nextElement->QueryIntAttribute( "oX", &oX);
        nextElement->QueryIntAttribute( "oY", &oY);
        nextElement->QueryIntAttribute( "oW", &oW);
        nextElement->QueryIntAttribute( "oH", &oH);
        //nextElement->QueryIntAttribute( "mX", &mX);
        mX=0;
        mFramesData.push_back(frameData(n,x,y,w,h,pX,pY,oX,oY,oW,oH,mX));
        nextElement = nextElement->NextSiblingElement();


        if (w>largestWidth)
        {
            largestWidth=w;
        }


    }
    if (!mFramesData.empty())
    {


        for (int i=0; i<mFramesData.size(); i++)
        {
            mFramesData.at(i).mX=largestWidth-mFramesData.at(i).w;
        }
    }


}

void Entity::update(sf::Time deltaTime)
{
    mAge++;
}

void Entity::drawBoundingBox()
{

    sf::RectangleShape boundy = sf::RectangleShape();
    boundy.setPosition(mPosition.x-mSize.x/2,mPosition.y-mSize.y);
    boundy.setOutlineColor(sf::Color(255,0,0,255.f));
    boundy.setOutlineThickness(2);
    boundy.setFillColor(sf::Color(0,0,0,0));
    //boundy.move(sf::Vector2f(-16.f,-16.f));
    boundy.setSize(mSize);
    TheGame::Instance()->getRenderTexture()->draw(boundy);
}


int Entity::findFrameNamed(std::string name)
{
    if (!mFramesData.empty())
    {
        for (int i=0; i<mFramesData.size(); i++)
        {
            if (name==mFramesData.at(i).n)
            {
                return i;
            }
        }
        std::cout << "unable to find frame named " << name << std::endl;
    }
    return 0;
}


