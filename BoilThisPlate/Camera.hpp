//
//  Camera.hpp
//  BoilThisPlate
//
//  Created by Nicholas Alcus on 9/7/16.
//  Copyright © 2016 Nicholas Alcus. All rights reserved.
//

#ifndef Camera_hpp
#define Camera_hpp

#include <SFML/Graphics.hpp>

class Camera
{
public:
    static Camera* Instance()
    {
        if(s_pInstance==0)
        {
            s_pInstance=new Camera;
            return s_pInstance;
        }
        return s_pInstance;
    }
    sf::Vector2f getOffset() {return mOffset;}
    void update();
    void setFocalPoint(float,float);
private:
    Camera();
    static Camera* s_pInstance;
    
    sf::Vector2f mOffset;
    sf::Vector2f mFocalPoint;
};

typedef Camera TheCamera;

#endif /* Camera_hpp */
