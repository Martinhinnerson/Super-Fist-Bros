//
//  game.cpp
//  SuperFistBros
//
//  Created by Martin Hinnerson on 2015-11-08.
//  Copyright © 2015 Martin Hinnerson. All rights reserved.
//

#include "game.hpp"
#include "player.hpp"
#include "platform.hpp"

using namespace std;
using namespace sf;

// Init - Initieringsfunktion som körs när spelet startas och skapar spelarna samt plattformarna på rätt ställen.
// Tar in storlek på fönstret, font, map med alla texturer (spelar och plattformar).
void game::init(const sf::Vector2u windowSize, const sf::Font& font, std::map<const std::string, const sf::Texture&>& textures)
{
    startTimer = startTimerMax;
    
    startText.setFont(font);
    startText.setCharacterSize(144);
    startText.setStyle(sf::Text::Bold);
    startText.setPosition(windowSize.x * 0.45, windowSize.y * 0.4);
    
    // Bestämmer storlek på hitboxen
    int playerSpriteH = 40;
    int playerSpriteW = 34;
    int SpriteScaleH = 3;
    int SpriteScaleW = 2;
    
    // Skapa spelare 1
    player Player1(player(Vector2f(windowSize.x * 0.3, windowSize.y * 0.6),
                          Vector2f(playerSpriteW*SpriteScaleW,
                                   playerSpriteH*SpriteScaleH)));
    // Ange nummer på spelaren
    Player1.playerNumber = players.size() + 1;
    
    // Sätt knappar som styr spelare 1
    if (Joystick::isConnected(0))
    {
        Player1.usingJoystick = true;
        Player1.joystickNumber = 0;
        cout << "Player 1 uses Joystick!" << endl;
    }
    else
    {
        cout << "Player 1 uses W, A, S, D, F, R!" << endl;
        Player1.setKeyBinding(Keyboard::W, Keyboard::S, Keyboard::A, Keyboard::D, Keyboard::F, Keyboard::R);
    }
    
    // StartKoordinater
    Player1.startCoordinates.x = Player1.hitbox.getPosition().x;
    Player1.startCoordinates.y = windowSize.y/3;
    
    // Initera statustexten för spelare 1
    Player1.status.setPosition(50, windowSize.y - 100);
    Player1.status.setFont(font);
    Player1.status.setCharacterSize(24);
    
    // Hämta spelare 1:s sprite och initiera den
    Player1.sprite.setTexture(textures["Player1"]);
    Player1.sprite.setTextureRect(sf::IntRect(0, 0, Player1.hitbox.getSize().x,
                                              Player1.hitbox.getSize().y));
    Player1.sprite.scale(3, 3);
    players.push_back(Player1);
    
    // Skapa spelare 2
    player Player2(player(Vector2f(windowSize.x * 0.7, windowSize.y * 0.6),
                          Vector2f(playerSpriteW*SpriteScaleW,
                                   playerSpriteH*SpriteScaleH)));
    
    // Player 2 får standardkontroller, dvs piltangenter + mellanslag + m
    
    // Ange nummer på spelaren
    Player2.playerNumber = players.size() + 1;
    
    if (Joystick::isConnected(1))
    {
        Player2.usingJoystick = true;
        Player2.joystickNumber = 1;
        cout << "Player 2 uses Joystick!" << endl;
    }
    else
    {
        cout << "Player 2 uses UP, DOWN, LEFT, RIGHT, SPACE, M!" << endl;
    }
    
    // Ange vilket håll spelare kollar åt vid start
    Player2.facingDirection = -1;
    
    // Startkoordinater
    Player2.startCoordinates.x = Player2.hitbox.getPosition().x;
    Player2.startCoordinates.y = windowSize.y/3;
    
    // Initiera statustexten för spelare 2
    Player2.status.setPosition(windowSize.x - 200, windowSize.y - 100);
    Player2.status.setFont(font);
    Player2.status.setCharacterSize(24);
    
    // Hämta spelare 2:s sprite och initiera den
    Player2.sprite.setTexture(textures["Player2"]);
    Player2.sprite.setTextureRect(sf::IntRect(0, 0, Player2.hitbox.getSize().x,
                                              Player2.hitbox.getSize().y));
    Player2.sprite.scale(3, 3);
    players.push_back(Player2);
    
    //Skapa plattformar
    platform Platform1(platform(Vector2f(windowSize.x * 0.2, windowSize.y * 0.7), Vector2f(windowSize.x * 0.6, 30), Color::Green));
    Platform1.sprite.setTexture(textures["Platform1"]);
    Platform1.sprite.scale(3.6, 2);
    platforms.push_back(Platform1);
}

// Funktion för att flytta spelarna enligt knapptryckningar
void game::updateMovement()
{
    for(int i = 0; i < players.size(); ++i)
    {
        players[i].updateMovement();
    }
}

// Funktion för att läsa knapptryck om nån slår
void game::updateFist()
{
    for(int i = 0; i < players.size(); ++i)
    {
        players[i].updateFist();
    }
}

// Funktion som raderar alla limbs
void game::clearLimbs()
{
    for (int i = 0; i < players.size(); ++i)
    {
        players[i].limbs.clear();
    }
}

// Funktion som anropar spelarnas olika kollisionsfunktioner
void game::checkCollision()
{
    for(int i = 0; i < players.size(); ++i)
    {
        players[i].checkPlatformCollision(&platforms);
        players[i].checkPlayerCollision(&players);
    }
}

// Funktion som kontrollerar om någon spelare hamnar utanför spelplanen

void game::checkOutside(Vector2u windowSize)
{
    for(int i = 0; i < players.size(); ++i)
    {
        players[i].checkPlayerOutside(windowSize);
    }
}

// Funktion som anropar spelarnas förflyttningsfunktioner
void game::move()
{
    for(int i = 0; i < players.size(); ++i)
    {
        players[i].move();
    }
}

// Funktion som anropar plattformarnas animationsfunktioner
void game::updatePlatforms()
{
    for(int i = 0; i < platforms.size(); ++i)
    {
        platforms[i].updateAnimation();
    }
}

// checkWin - Kollar om någon spelar har fått slut på liv
bool game::checkWin()
{
    for (int i{0}; i < players.size(); ++i)
    {
        if (players[i].lives == 0)
        {
            return true;
        }
    }
    return false;
}

// updatePlayerSprites - uppdatera animationen för respektive spelare
void game::updatePlayerSprites()
{
    for(int i = 0; i < players.size(); ++i)
    {
        players[i].updateSprite();
    }
}

void game::startDelay(RenderWindow& window)
{
    --startTimer;
    if (startTimer > startTimerMax * 0.75)
    {
        startText.setString("3");
    }
    if (startTimer > startTimerMax * 0.5 && startTimer <= startTimerMax * 0.75)
    {
        startText.setString("2");
    }
    if (startTimer > startTimerMax * 0.25 && startTimer <= startTimerMax * 0.5)
    {
        startText.setString("1");
    }
    if (startTimer <= startTimerMax * 0.25)
    {
        startText.setPosition(window.getSize().x * 0.38, startText.getPosition().y);
        startText.setString("FIST!");
    }
}