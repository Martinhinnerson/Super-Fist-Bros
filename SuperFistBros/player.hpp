//
//  player.hpp
//  SuperFistBros
//
//  Created by Martin Hinnerson on 2015-11-08.
//  Copyright © 2015 Martin Hinnerson. All rights reserved.
//

#ifndef player_hpp
#define player_hpp

#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>
#include <vector>
#include "platform.hpp"
#include "limb.hpp"
#include "objects.hpp"


class player : public object
{
public:
    
    sf::Vector2f newPos; // 2D vector med de nya x och y koordinaterna
    sf::Vector2f startCoordinates; //Start-koordinater
    
    // Koordinater till de nya sidorna
    float newTop, newBottom, newLeft, newRight;
     
    // Vector med alla limbs just nu, än sålänge kan man bara ha en i taget
    std::vector<limb> limbs;
    
    // Knapparna som styr spelaren
    sf::Keyboard::Key up_key = sf::Keyboard::Up;
    sf::Keyboard::Key down_key = sf::Keyboard::Down;
    sf::Keyboard::Key left_key = sf::Keyboard::Left;
    sf::Keyboard::Key right_key = sf::Keyboard::Right;
    sf::Keyboard::Key fastFist_key = sf::Keyboard::Space;
    sf::Keyboard::Key powerFist_key = sf::Keyboard::M;
    sf::Keyboard::Key block_key = sf::Keyboard::Down;
    
    // Nummer för att veta vilken spelare vi kollision
    int playerNumber = 0;
    
    const int maxdamage = 200;
    int damage = 0;
    int lives = 3;
    
    // Gravitationskonstant
    const float gravityConst = 2;
    // Maxgravitation för att spelaren inte ska falla snabbare och snabbare
    const float maxGravity = 8;
    
    // Förflyttning
    const int jumpSpeed = 30; // Hur mycket hastighet i y-led ett hopp ger
    const float movementSpeed = 5; // hur snabbt man rör sig i sidled
    const float airMovement = 1;
    
    sf::Vector2f velocity; // 2D hastighetsvektor x och y
    
    int direction_x = 1; // nuvarande riktning, höger = 1, vänster = -1
    int facingDirection = 1; // riktgning gubben är vänd och slår åt
    
    bool isFastFist = false; // true om spelare slår snabba attacken
    bool isStrongFist = false; // true om spelare slår starka attacken
    bool isWalking = false; // true om spelaren rör på sig i x-led
    bool isBlocking = false; // true om spelarnas limbs slår ihop
    bool inAir = true; // True om man inte står på en plattform
    bool usingJoystick = false;
    
    int joystickNumber = 0;
    
    sf::Text status; // Text med din info som skriv på skärmen
    
    // Timer innan man får hoppa igen
    int jumpTimer = 0;
    const int jumpTimerMax = 15;
    
    // Variabel hur många gånger man får hoppa
    int jumpCounter = 0;
    const int jumpCounterMax = 2;
    
    int blockTimer = 0;
    const int blockTimerMax = 150;
    
    // Timer innan man får slå igen
    int timer = 0;
    const int timerMax = 20;
    int fistKey = 0;
    
    const int fastFistPower = 10;
    const int strongFistPower = 20;
    
    // fastFist Sprites
    
    int fastSpriteCounter = 0;
    const int fastFistFrequency = 3;
    int counterFastFist = 0;;
    
    int strongSpriteCounter = 0;
    const int strongFistFrequency = 5;
    int counterStrongFist = 0;
    
    int blockSpriteCounter = 0;
    const int blockSpriteCounterMax = 2;
    int blockSpriteFrequency = 5;
    
    // Räknare för att loopa genom spelarens spritesheet
    int counterSprite = 0;
    // Variabel och räknare för hur ofta spriten ska ändras
    const int stepFrequency = 5;
    int counterWalking = 0;
    
    // Konstruktor
    player(sf::Vector2f, sf::Vector2f);
    
    // Funktion för att änfdra spelarens tangenter
    void setKeyBinding(sf::Keyboard::Key, sf::Keyboard::Key,
                       sf::Keyboard::Key, sf::Keyboard::Key,
                       sf::Keyboard::Key, sf::Keyboard::Key);
    
    // Uppdatera knapptryck hur spelaren ska röra sig
    void updateMovement();
    void leftRightMovement(); //hjälpfunktion till rörelse
    // Uppdatera om spelare tryckt för att slå
    void updateFist();
    
    // Kollision mellan spelare och plattform
    void checkPlatformCollision(std::vector<platform>*);
    // Kollision mellan spelare och andra spelares fist
    void checkPlayerCollision(std::vector<player>*);
    // Kontroll om spelare hamnar utanför spelplanen
    void checkPlayerOutside(sf::Vector2u);
    
    // Flyttar spelaren
    void move();
    
    // Spelarens snabba slag
    void fastFist();
    
    // Spelarens kraftfulla slag
    void powerFist();
    
    void updateSprite();
};


#endif /* player_hpp */