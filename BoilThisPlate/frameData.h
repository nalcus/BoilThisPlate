#ifndef __frameData__
#define __frameData__

#include <string>

class frameData
{
public:
    std::string n;
    int x;
    int y;
    int w;
    int h;
    float pX;
    float pY;
    int oX;
    int oY;
    int oW;
    int oH;
    int mX;
    frameData(std::string aN, int aX, int aY, int aW, int aH, float apX, float apY, int aoX, int aoY, int aoW, int aoH, int amX)
    :n(aN),x(aX), y(aY), w(aW), h(aH), pX(apX), pY(apY), oX(aoX), oY(aoY), oW(aoW), oH(aoH), mX(amX)
    {

    }
};





#endif // __frameData__
