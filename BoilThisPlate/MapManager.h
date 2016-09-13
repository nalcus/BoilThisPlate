#ifndef __MapManager__
#define __MapManager__

#include <SFML/Graphics.hpp>
#include <vector>
#include "tinyxml2.h"

class MapManager
{
public:
    static MapManager* Instance()
    {
        if(s_pInstance==0)
        {
            s_pInstance=new MapManager;
            return s_pInstance;
        }
        return s_pInstance;
    }

    void setTileset(sf::Texture * pTexture);
    bool init();
    void update(sf::Time deltaTime);
    void render();
    int getClipAtScreenPosition(int x, int y);
     int getMapHeight ()  {return mMapHeight;}
    int getMapWidth ()  {return mMapWidth;}


private:
    MapManager();
    static MapManager* s_pInstance;
    sf::Sprite mSprite;

    std::vector<int> mBgData;
    std::vector<int> mClipData;

    int mMapWidth;
    int mMapHeight;
    int mClipFirstGID;
    int mBgFirstGID;
};

typedef MapManager TheMapManager;


#endif // __MapManager__
