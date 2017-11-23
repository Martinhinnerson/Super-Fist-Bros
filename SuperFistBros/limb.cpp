//
//  limb.cpp
//  SuperFistBros
//
//  Created by Martin Hinnerson on 2015-11-09.
//  Copyright © 2015 Martin Hinnerson. All rights reserved.
//

#include "limb.hpp"
using namespace std;
using namespace sf;

// Konstruktor, tar in position storlek och styrkekonstant
limb::limb(Vector2f pos, Vector2f size, int p)
{
    hitbox.setSize(size);
    hitbox.setPosition(pos);
    hitbox.setFillColor(Color::White);
    Power = p;
}

