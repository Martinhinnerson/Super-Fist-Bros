//
//  Menu.cpp
//  SuperFistBros
//
//  Created by Tim fornell on 2015-11-13.
//  Copyright © 2015 Martin Hinnerson. All rights reserved.
//

#include "Menu.hpp"

using namespace std;
using namespace sf;

// Konstruktor, tar in storlek på fönstret och en font
// Här konstrueras alla knappar som ska finnas och lagras i menu:s map, buttons.
menu :: menu(Vector2u windowSize, Font& font)
{
    int button_x = windowSize.x * 0.25;
    int button_y = windowSize.y * 0.25;
    int buttonSpacing = 200;
    int TextSize = 80;
    
    // Skapa alla knappar
    // Meny 1 - startmenu
    button button1{"Start the game","start", sf::Vector2f(button_x, button_y),TextSize};
    button button2{"Settings","set", sf::Vector2f(button_x,button_y + buttonSpacing), TextSize};
    button button3{"Exit game", "exit", sf::Vector2f(button_x,button_y + 2*buttonSpacing), TextSize};
    // Meny 2 - inställningar (bonugtillägg)
  //  button button4{"Change skin", "skin", sf::Vector2f(button_x,button_y),TextSize};
  //  button button5{"Change keys", "keys", sf::Vector2f(button_x,button_y + buttonSpacing),TextSize};
    button button6{"Return to main menu", "return", sf::Vector2f(button_x,button_y + 2*buttonSpacing),TextSize};
    
    // Skapa en tillfällig map med alla knappar, kopiera sedan till buttons.
    std::map<std::string,button> temp {{button1.codename,button1},{button2.codename,button2},
        {button3.codename,button3},//{button4.codename,button4},{button5.codename,button5},
        {button6.codename,button6}};
    buttons = temp;
    
    // Sätt fonten på alla knappar
    buttons["start"].buttonText.setFont(font);
    buttons["exit"].buttonText.setFont(font);
    buttons["return"].buttonText.setFont(font);
    buttons["set"].buttonText.setFont(font);
    buttons["skin"].buttonText.setFont(font);
    buttons["keys"].buttonText.setFont(font);
}

// Checkbutton - Kontrollerar om användare tryckt på
// någon knapp med musen.
button& menu :: Checkbutton(sf::Vector2i mousepos)
{
    // Ta reda på vilken meny som visas så rätt knappar kontrolleras.
    std::vector<std::string>* buttonCode_ptr;
    
    // Startmeny
    if(menustate == 1)
    {
        buttonCode_ptr = &buttonCode1;
    }
    // Inställningar
    else if(menustate == 2)
    {
        buttonCode_ptr = &buttonCode2;
    }
    
    for(int i{0}; i < buttonCode_ptr->size(); ++i)
    {
        // Iterator
        std::map<std::string,button>::iterator it{buttons.find((*buttonCode_ptr)[i])};
        
        // Mousepos innanför knappens gränser
        if(it->second.buttonText.getGlobalBounds().contains(mousepos.x,mousepos.y))
        {
            return it->second;
        }
    }
}

// Draw - Ritar ut rätt meny (beror på menustate)
void menu :: Draw(sf::RenderWindow& window)
{
    // Ta reda på vilken meny som ska visas
    std::vector<std::string>* buttonCode_ptr;

    // Start-meny
    if(menustate == 1)
    {
        buttonCode_ptr = &buttonCode1;
    }
    // Inställningar
    else if(menustate == 2)
    {
        buttonCode_ptr = &buttonCode2;
    }
    
    // Rita varje knapp
    for(int i{0}; i < buttonCode_ptr->size(); ++i)
    {
        window.draw(buttons[(*buttonCode_ptr)[i]].buttonText);
    }
        
}

// Buttonpress - Hanterar knapptryckningar
// Tar in koden till vilken knapp som tryckts, referens till game och spelfönstret
void menu :: ButtonPress(std::string buttonPressed, game& gamewindow, sf::RenderWindow& window)
{
    if(buttonPressed != "0")
    {
        //button 1 eller om man trycker enter eller om joystick 0 trycker start = start game
        if(buttonPressed == "start")
        {
            // Starta spelet
            gamewindow.gameRunning = true;
            // Menustate = 0 => Spelet körs
            menustate = 0;
        }
        //button 2 = settings
        else if(buttonPressed == "set")
        {
            // Ändra meny
            menustate = 2;
        }
        //button 3 = exit game
        else if(buttonPressed == "exit")
        {
            // Avsluta spelet
            window.close();
        }
        else if(buttonPressed == "return" )
        {
            // Ändra meny
            menustate = 1;
        }
        else if(buttonPressed == "skin")
        {
            std::cout << "Ej implementerad!" << std::endl;
        }
        else if(buttonPressed == "keys")
        {
            std::cout << "Ej implementerad!" << std::endl;
        }
    }
}