//
//  game.hpp
//  SuperFistBros
//
//  Created by Martin Hinnerson on 2015-11-08.
//  Copyright © 2015 Martin Hinnerson. All rights reserved.
//

#ifndef game_hpp
#define game_hpp

#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include "ResourcePath.hpp"
#include "platform.hpp"
#include "player.hpp"

class game
{
public:
    // Vector med alla spelare
    std::vector<player> players;
    // Vector med alla platformar
    std::vector<platform> platforms;
    
    game()=default;
    
    void init(const sf::Vector2u,const sf::Font&, std::map<const std::string, const sf::Texture&>&);
    
    bool checkWin();
    void updateMovement();
    void updateFist();
    void clearLimbs();
    void checkCollision();
    void checkOutside(sf::Vector2u);
    void move();
    void updatePlatforms();
    void updatePlayerSprites();
    void startDelay(sf::RenderWindow&);
    
    bool gameRunning = false;
    float startTimer = 0;
    int startTimerMax = 100;
    
    sf::Text startText;
};


#endif /* game_hpp */
