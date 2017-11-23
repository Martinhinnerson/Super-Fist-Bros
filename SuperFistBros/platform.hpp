//
//  platform.hpp
//  SuperFistBros
//
//  Created by Martin Hinnerson on 2015-11-08.
//  Copyright © 2015 Martin Hinnerson. All rights reserved.
//

#ifndef platform_hpp
#define platform_hpp

#include <iostream>
#include <SFML/Graphics.hpp>
#include "objects.hpp"

class platform : public object
{
public:
    
    int updateFrequency = 5;
    int frequencyCounter = updateFrequency;
    int numFrames = 6;
    int frameCounter = 0;
    int delay = 100;
    int delayTimer = 0;
    
    platform(sf::Vector2f, sf::Vector2f, sf::Color);
    void updateAnimation();
};

#endif /* platform_hpp */
