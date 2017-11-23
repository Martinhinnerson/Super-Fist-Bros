//
//  platform.cpp
//  SuperFistBros
//
//  Created by Martin Hinnerson on 2015-11-08.
//  Copyright © 2015 Martin Hinnerson. All rights reserved.
//

#include <iostream>
#include "platform.hpp"

using namespace std;
using namespace sf;

//Konstruktor, tar in position, storlek och färg
platform::platform(sf::Vector2f pos, sf::Vector2f size, sf::Color color)
{
    hitbox.setSize(size);
    hitbox.setPosition(pos);
    hitbox.setFillColor(color);
    
    top = hitbox.getPosition().y;
    bottom = hitbox.getPosition().y + hitbox.getSize().y;
    left = hitbox.getPosition().x;
    right = hitbox.getPosition().x + hitbox.getSize().x;
    
    //sätt en offset i x och y för spriten, då spriten ej är 2D
    sprite.setPosition(hitbox.getPosition().x - 30, hitbox.getPosition().y - hitbox.getSize().y - 30);
    sprite.setTextureRect(sf::IntRect(0, 0, 456, 240));
}

//Animerar platformen
void platform::updateAnimation()
{
    if (frequencyCounter == updateFrequency)
    {
        if (delayTimer == delay)
        {
            // Hämta nästa steg i animationen
            sprite.setTextureRect(sf::IntRect(frameCounter * 456, 0, 456, 240));
            ++frameCounter;
            if (frameCounter == numFrames) {
                frameCounter = 0;
                delayTimer = 0;
            }
        }
        frequencyCounter = 0;
    }
    if (delayTimer < delay)
    {
        ++delayTimer;
    }
    ++frequencyCounter;
}