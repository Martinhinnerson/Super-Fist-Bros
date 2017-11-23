//
//  Buttons.hpp
//  SuperFistBros
//
//  Created by Tim fornell on 2015-11-13.
//  Copyright © 2015 Martin Hinnerson. All rights reserved.
//

#ifndef Buttons_hpp
#define Buttons_hpp
#include <SFML/Graphics.hpp>
#include <iostream>

// Klass för knappar
class button
{
public:
    // Knappens kod
    std::string codename;
    // Vad som står på knappen
    sf::Text buttonText;
    button(const std::string&, const std::string&, const sf::Vector2f, const int);
    button() = default;
};

#endif /* Buttons_hpp */
