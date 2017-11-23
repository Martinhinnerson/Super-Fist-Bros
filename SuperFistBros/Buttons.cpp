//
//  Buttons.cpp
//  SuperFistBros
//
//  Created by Tim fornell on 2015-11-13.
//  Copyright © 2015 Martin Hinnerson. All rights reserved.
//

#include "Buttons.hpp"
#include "ResourcePath.hpp"

using namespace std;
using namespace sf;

// Konstruktor för knappar, tar in beskrivande text, kod för knappen
// samt position och fontstorlek
button :: button(const std::string& text, const std::string& code
                 , const sf::Vector2f pos, const int size)
{
    buttonText.setString(text);
    buttonText.setCharacterSize(size/2);
    buttonText.setPosition(pos);
    buttonText.setColor(sf::Color::White);
    buttonText.setStyle(sf::Text::Bold);
    
    codename = code;
}
