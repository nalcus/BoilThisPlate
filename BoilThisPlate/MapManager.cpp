#include "MapManager.h"
#include "Randomizer.h"

MapManager* MapManager::s_pInstance = 0;

#include <iostream>
#include "tinyxml2.h"
#include "Game.h"
#include "Camera.hpp"

using namespace std;

bool checkDocError (tinyxml2::XMLDocument &rDoc);

MapManager::MapManager()
{
    ;
}

bool MapManager::init()
{
    mBgData.clear(); // clear our background data vector
    mClipData.clear(); // clear our clip data vector

    tinyxml2::XMLDocument doc;
    doc.LoadFile( "assets/map1.tmx" ); // open the map

    if(!checkDocError(doc))
    {
        return false;
    }

    tinyxml2::XMLElement* mapElement = doc.FirstChildElement( "map" );
    if (mapElement==0)
    {
        return false;
    }

    // get the width and height of the map
    mapElement->QueryIntAttribute( "width", &mMapWidth );
    mapElement->QueryIntAttribute( "height", &mMapHeight );

    // we need the clip tile first gid

    // first look for clip tileset  elemen t
    tinyxml2::XMLElement* nextTilesetElement = doc.FirstChildElement()->FirstChildElement( "tileset" );
    if (nextTilesetElement==0)
    {
        return false;
    }

    // look for layer name "bg_layer_0". this is the art layer.
    string tilesetName;
    while (nextTilesetElement!=0)
    {
        // get layer's name attribute
        tilesetName = nextTilesetElement->Attribute("name");
        // make sure it matches our string if not..
        if (tilesetName!="platformertiles")
        {
            // check the next sibling layer
            nextTilesetElement = nextTilesetElement->NextSiblingElement("tileset");
        }
        else
        {
            // if we did find it exit the while loop with a break while we are pointing at the matching layer.
            break;
        }
    }

    if (nextTilesetElement==0)
    {
        return false;
    }
    // set clip first gid to what we got from xml...
    nextTilesetElement->QueryIntAttribute( "firstgid", &mClipFirstGID);


    // first look for bg layer
    tinyxml2::XMLElement* nextLayerElement = doc.FirstChildElement()->FirstChildElement( "layer" );
    if (nextLayerElement==0)
    {
        return false;
    }

    // look for layer name "bg_layer_0". this is the art layer.
    string layerName;
    while (nextLayerElement!=0)
    {
        // get layer's name attribute
        layerName = nextLayerElement->Attribute("name");
        // make sure it matches our string if not..
        if (layerName!="map")
        {
            // check the next sibling layer
            nextLayerElement = nextLayerElement->NextSiblingElement("layer");
        }
        else
        {
            // if we did find it exit the while loop with a break while we are pointing at the matching layer.
            break;
        }
    }
    // if we didn't find the layer return.
    if (nextLayerElement==0)
    {
        return false;
    }


    // jump into the data section
    tinyxml2::XMLElement* dataElement = nextLayerElement->FirstChildElement( "data" );

    // if we didn't find it return
    if (dataElement==0)
    {
        return false;
    }

    // cycle through the data and fill our vector
    tinyxml2::XMLElement* nextElement = dataElement->FirstChildElement();
    int nextElementData;
    while (nextElement!=0)
    {

        nextElement->QueryIntAttribute( "gid", &nextElementData);
        mBgData.push_back(nextElementData);
        nextElement = nextElement->NextSiblingElement();

    }
    
    std::cout << "map init success!" << std::endl;

    return true;
    // set scale for big pixels!
    //mSprite.setScale(2.f,2.f);
}

void MapManager::setTileset(sf::Texture *pTileset)
{
    mSprite.setTexture(*pTileset);

}

void    MapManager::update(sf::Time deltaTime)
{


}

int  MapManager::getClipAtScreenPosition(int x, int y)
{
    int rowoffset=0;
    int coloffset=0;

    int r=rowoffset+(y/16);
    int c=coloffset+(x/16);

    int index=c+(r*mMapWidth);

    int mapTile=-1;

    if (r>=0&&r<mMapWidth&&c>=0&&c<mMapWidth) // if r and c are within map bounds
    {

        index=c+(r*mMapWidth);
        if (index >=0 && index < mBgData.size()) // is index in range? out data
        {
            mapTile = mBgData.at(index)-(mBgFirstGID+1);
        }
        else
        {
            // std::cout << "index out of range! at" << c << ", " << r << endl;
        }
    }
    
    return mapTile;
}

void     MapManager::render()
{
    int tileMapWidth=8;
    int tileSize=32;

    sf::IntRect rect = sf::IntRect(0,0,tileSize,tileSize);

    float xoffset=TheCamera::Instance()->getOffset().x*2;
    float yoffset=TheCamera::Instance()->getOffset().y*2;
    float scale=TheGame::Instance()->getScale()/2;
    float scaledSize=scale*tileSize;

    int mapTile=0;
    int index=-1;

    int totalRows=int(TheGame::Instance()->getDisplayHeight())/scaledSize;
    int totalCols=int(TheGame::Instance()->getDisplayWidth())/scaledSize;
    
    float rowoffset=yoffset/scaledSize;
    float coloffset=xoffset/scaledSize;

    for (int r=0; r<mMapWidth; r++)
    {
        for (int c=0; c<mMapWidth; c++)
        {

            if (r>=0&&r<mMapWidth&&c>=0&&c<mMapWidth) // if r and c are within map bounds
            {

                index=c+(r*mMapWidth);
                if (index >=0 && index < mBgData.size()) // is index in range? out data
                {

                    mapTile = mBgData.at(index)-(mBgFirstGID+1);
                    if (mapTile!=-1) {
                        rect.top=tileSize*int(mapTile/tileMapWidth);
                        rect.left=tileSize*(mapTile%tileMapWidth);
                        mSprite.setTextureRect(rect);
                        mSprite.setPosition
                        ((c*tileSize-xoffset)*scale,(r*tileSize-yoffset)*scale);

                        mSprite.setScale(scale,scale);
                        TheGame::Instance()->getRenderTexture()->draw(mSprite);
                    }

                }
                else
                {
                    
                }

            }
            else
            {
                // do n0thing
            }
        }
    }


}
