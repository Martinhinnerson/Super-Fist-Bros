//
//  limb.hpp
//  SuperFistBros
//
//  Created by Martin Hinnerson on 2015-11-09.
//  Copyright © 2015 Martin Hinnerson. All rights reserved.
//

#ifndef limb_hpp
#define limb_hpp

#include <iostream>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "ResourcePath.hpp"
#include "platform.hpp"
#include "objects.hpp"

class limb : public object
{
public:
    sf::Vector2f newPos;
    float newTop, newBottom, newLeft, newRight;
    
    int limbNumber; // samma nummer som spelaren som skapar limben
    int Power; // kraft på slaget
    
    limb(sf::Vector2f, sf::Vector2f, int);
};

#endif /* limb_hpp */
