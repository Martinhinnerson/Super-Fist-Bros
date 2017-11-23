//
//  objects.hpp
//  SuperFistBros
//
//  Created by Martin Hinnerson on 2015-11-15.
//  Copyright © 2015 Martin Hinnerson. All rights reserved.
//

#ifndef objects_hpp
#define objects_hpp

#include <SFML/Graphics.hpp>

class object
{
public:
    virtual ~object() = default;
    sf::RectangleShape hitbox;
    float top, bottom, left, right;
    sf::Sprite sprite;
    sf::Texture texture;
protected:
    object() = default;
    object(const object&) = default;
};

#endif /* objects_hpp */
