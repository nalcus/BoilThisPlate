#include "EntityManager.h"
#include "Game.h"
#include <iostream>
#include "Entity.h"
#include <string>
//#include "MapManager.h"

EntityManager* EntityManager::s_pInstance = 0;

void EntityManager::handleCollisionBetween(Entity* FirstEntity, Entity * SecondEntity)
{
    FirstEntity->receiveCollision(SecondEntity);
    SecondEntity->receiveCollision(FirstEntity);
}

void    EntityManager::update(sf::Time deltaTime)
{
    if (!mEntities.empty())
    {
        for (unsigned int i=mEntities.size()-1;i>0;i--)
        {
            if(!mEntities.at(i)->getActive())
            {
                mEntities.erase(mEntities.begin()+i);
            }
        }
    }


   // update movements
    if (!mEntities.empty())
    {
        for (unsigned int i=0; i<mEntities.size(); i++)
        {
            if(mEntities.at(i)->getActive())
            {


                // update movements and animation
                mEntities.at(i)->update(deltaTime);
            }
        }
    }

    // do collision check

    if (!mEntities.empty())
    {
        for (unsigned int i=0; i<mEntities.size(); i++)
        {
            if(mEntities.at(i)->getActive())
            {

                // test collisions
                // j=i+1 makes sure we never test an entity against itself,
                // and each entity against each other entity only once
                for (unsigned int j=i+1; j<mEntities.size(); j++)
                {
                    // make sure j is active too
                    if (mEntities.at(j)->getActive())
                    {
                        if (mEntities.at(i)->testCollision(mEntities.at(j)))
                        {
                            // make sure they are not the same type
                            if ((*mEntities.at(i)->getType())!=*mEntities.at(j)->getType())
                            {
                                // handle collision
                                handleCollisionBetween(mEntities.at(i),mEntities.at(j));

                            }

                        }
                    }
                }
            }
        }
    }

    // update camera

    if (!mEntities.empty()) // check if there are any entities
    {
        for (unsigned int i=mEntities.size()-1;i>0;i--) // cycle through each entity
        {
            if(mEntities.at(i)->getActive()) // if the entity is active
            {
                std::string Pl="PLAYER";
                if(*mEntities.at(i)->getType()==Pl) // if entity is the player
                {




                }

            }
        }
    }
}

void     EntityManager::render()
{

    if (!mEntities.empty())
    {


            for (unsigned int i=0; i<mEntities.size(); i++)
            {

                    mEntities.at(i)->render();

            }
        

    }

}

Entity* EntityManager::getPlayer()
{
    std::string En="PLAYER";
    
    if (!mEntities.empty())
    {
        for (unsigned int i=0; i<mEntities.size(); i++)
        {
            
            if(*mEntities.at(i)->getType()==En)
            {
                return mEntities.at(i);
            }
            
        }
        
        
    }


}

void EntityManager::pushEntity(Entity * pEntity)
{
    mEntities.push_back(pEntity);
}


