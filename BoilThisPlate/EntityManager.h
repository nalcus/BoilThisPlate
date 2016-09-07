#ifndef __EntityManager__
#define __EntityManager__

#include <SFML/Graphics.hpp>
#include <vector>

#include "Entity.h"

class EntityManager
{
public:
    static EntityManager* Instance()
    {
        if(s_pInstance==0)
        {
            s_pInstance=new EntityManager;
            return s_pInstance;
        }
        return s_pInstance;
    }
    void update(sf::Time deltaTime);
    void render();
    void pushEntity(Entity * pEntity);
    std::vector<Entity*> * getEntites() {return &mEntities;}
private:
    void handleCollisionBetween(Entity* FirstEntity, Entity * SecondEntity);
    EntityManager() {}
    ~EntityManager() {}
    std::vector<Entity*> mEntities;
    static EntityManager* s_pInstance;
    int mFramesUntilNextAction;
};

typedef EntityManager TheEntityManager;



#endif // __EntityManager__
