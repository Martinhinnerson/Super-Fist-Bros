//
//  Menu.hpp
//  SuperFistBros
//
//  Created by Tim fornell on 2015-11-13.
//  Copyright © 2015 Martin Hinnerson. All rights reserved.
//

#ifndef Menu_hpp
#define Menu_hpp

#include "Buttons.hpp"
#include "game.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>
#include "ResourcePath.hpp"

// Klass för meny
class menu
{
public:
    // Lagra buttoncodes för respektive menustate
    
    // Startmeny
    std::vector<std::string> buttonCode1{"start","exit","set"};
    // Inställningar
    std::vector<std::string> buttonCode2{"return","keys","skin"};
    
    // Initieras med menustate = 1 => startmenyn (0 => spelet körs)
    int menustate = 1;
    
    // Map med alla tillgängliga knappar, lagrade med nyckel
    // som motsvarar dess kod
    std::map<std::string, button> buttons;
   
    // Konstruktor
    menu(sf::Vector2u, sf::Font&);
    
    button& Checkbutton(sf::Vector2i);
    void Draw(sf::RenderWindow&);
    void ButtonPress(std::string, game&, sf::RenderWindow&);
    void StartGame();
    void ExitGame();
};

#endif /* Menu_hpp */
