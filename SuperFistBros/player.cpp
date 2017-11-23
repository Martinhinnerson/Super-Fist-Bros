//
//  player.cpp
//  SuperFistBros
//
//  Created by Martin Hinnerson on 2015-11-08.
//  Copyright © 2015 Martin Hinnerson. All rights reserved.
//

#include "player.hpp"
#include "ResourcePath.hpp"
#include <stdlib.h>

using namespace std;
using namespace sf;

// Konstruktor
player::player(sf::Vector2f pos, sf::Vector2f size)
{
    hitbox.setSize(size);
    hitbox.setPosition(pos);
    hitbox.setFillColor(Color::Blue);
    velocity = sf::Vector2f(0, 0);
}

// Funktion som uppdaterar movement
void player::updateMovement()
{
    // sätt nyPos till gamla positionen
    newPos.x = hitbox.getPosition().x;
    newPos.y = hitbox.getPosition().y;
    
    // oms spelaren är i luften ska den påverkas av gravitation
    if (inAir)
    {
        velocity.y += gravityConst;
        //spelaren kan inte falla snabbare än maxGravity
        if (velocity.y > maxGravity)
        {
            velocity.y = maxGravity;
        }
    }
    
    // Timer innan man kan använda block igen
    if (blockTimer != 0)
    {
        ++blockTimer;
    }
    if (blockTimer == blockTimerMax)
    {
        blockTimer = 0;
    }
    
    // Om inte joystick är anslutern används tangentbord
    if (!usingJoystick)
    {
        //Om spelaren trycker på block-knappen så blockar spelaren och timern startas
        if (Keyboard::isKeyPressed(block_key))
        {
            if (blockTimer == 0)
            {
                ++blockTimer;
                ++blockSpriteCounter;
                isBlocking = true;
            }
        }
        else
        {
            blockSpriteCounter = 0;
            blockSpriteFrequency = 5;
            isBlocking = false;
        }
        
        // Om hoppknappen trykts hantera hopp
        if (Keyboard::isKeyPressed(up_key) && jumpTimer == 0 && jumpCounter < jumpCounterMax && !isBlocking)
        {
            ++jumpTimer;
            ++jumpCounter;
            velocity.y = -jumpSpeed;
            newPos.y += velocity.y;
            inAir = true;
        }
        
        // Om man tyckt att gå vänster och inte blockar
        // ändra riktning och anropa hjälpfunktionen
        if (Keyboard::isKeyPressed(left_key) && !isBlocking)
        {
            if (velocity.x <= movementSpeed)
            {
                direction_x = -1;
                facingDirection = -1;
            }
            leftRightMovement();
        }
        // Samma för höger
        else if (sf::Keyboard::isKeyPressed(right_key) && !isBlocking)
        {
            if (velocity.x <= movementSpeed)
            {
                facingDirection = 1;
                direction_x = 1;
            }
            leftRightMovement();
        }
        //annars om spelaren rör sig åt nått håll utan, minska hastigheten med luftmotståndet
        else if(velocity.x > movementSpeed)
        {
            velocity.x -= airMovement;
            newPos.x += velocity.x * direction_x;
        }
        // annars stanna spelaren om den inte hade en hastighete högre än förflyttningshastigheten
        else
        {
            if (velocity.x <= movementSpeed)
            {
                velocity.x = 0;
            }
            newPos.x += velocity.x * direction_x;
        }
    }
    else //Om joystick används, motsvarande funktioner som för tangetbord
    {
        if (Joystick::isButtonPressed(joystickNumber, 7))
        {
            if (blockTimer == 0)
            {
                ++blockTimer;
                isBlocking = true;
            }
        }
        else
        {
            isBlocking = false;
        }

        // Om hoppknappen trykts hantera hopp
        if ((Joystick::getAxisPosition(joystickNumber, Joystick::Axis::Y) < -60 || Joystick::isButtonPressed(joystickNumber, 3)) && jumpTimer == 0 && jumpCounter < jumpCounterMax && !isBlocking)
        {
            ++jumpTimer;
            ++jumpCounter;
            velocity.y = -jumpSpeed;
            newPos.y += velocity.y;
            inAir = true;
        }
       
        // Om man gått tyckt att gå höger eller vänster
        // ändra riktning och anropa hjälpfunktionen
        if (Joystick::getAxisPosition(joystickNumber, Joystick::Axis::X) < -60 && !isBlocking)
        {
            if (velocity.x <= movementSpeed)
            {
                direction_x = -1;
                facingDirection = -1;
            }
            leftRightMovement();
        }
        else if (Joystick::getAxisPosition(joystickNumber, Joystick::Axis::X) > 60 && !isBlocking)
        {
            if (velocity.x <= movementSpeed)
            {
                facingDirection = 1;
                direction_x = 1;
            }
            leftRightMovement();
        }
        else if(velocity.x > movementSpeed)
        {
            velocity.x -= airMovement;
            newPos.x += velocity.x * direction_x;
        }
        // annars stanna spelaren om den inte hade en hastighete högre än förflyttningshastigheten
        else
        {
            if (velocity.x <= movementSpeed)
            {
                velocity.x = 0;
            }
            newPos.x += velocity.x * direction_x;
        }
    }
    
    // Addera nya y-positionen med velocity.y
    newPos.y += velocity.y;
    
    // om spelaren rör sig i x-led sätt isWalking
    if (velocity.x > 0)
    {
        isWalking = true;
    }
    else
    {
        isWalking = false;
    }
    
    // Om spelaren är i luften, fixa med hopptimers
    if (inAir)
    {
        if (jumpTimer != 0)
        {
            ++jumpTimer;
            if (jumpTimer == jumpTimerMax)
            {
                jumpTimer = 0;
            }
        }
        counterSprite = 1;
        counterWalking = 0;
    }
    // Om spelaren är på marken
    else
    {
        jumpTimer = 0;
        jumpCounter = 0;
        inAir = false;
    }
    
    // Sätt de nya sidorna enligt nya positionen
    newTop = newPos.y;
    newBottom = newPos.y + hitbox.getSize().y;
    newLeft = newPos.x;
    newRight = newPos.x + hitbox.getSize().x;
}

// Funtion för att göra ett slag om slagknapp tryckts

void player::updateFist()
{
    if (!isBlocking)
    {
        if (!usingJoystick)
        {
            // Om fastFist_key tryckts och timern är nollställd
            if (Keyboard::isKeyPressed(fastFist_key) && timer == 0)
            {
                fistKey = 1; //Indikerar att det är fastFist som tryckts
                isFastFist = true;
                fastFist();
                ++timer;
            }
            // Om powerFist_key tryckts och timern är nollställd
            else if (Keyboard::isKeyPressed(powerFist_key) && timer == 0)
            {
                isStrongFist = true;
                fistKey = 2;
                ++timer;
            }
            //Om vi nått timerMax och fastFist skapats
            else if (timer == timerMax && fistKey == 1)
            {
                timer = 0;
            }
            //Om vi nått timerMax och powerFist skapats
            else if (timer == timerMax && fistKey == 2)
            {
                powerFist();
                ++timer;
            }
            //Om vi nått timerMax * 2, dvs vi väntat timerMax efter en powerFist
            else if (timer == timerMax * 2)
            {
                timer = 0;
            }
            //Om vi tryckt men inte nått max, räkna upp timern
            else if (timer != 0)
            {
                ++timer;
            }
        }
        else
        {
            // Om fastFist_key tryckts och timern är nollställd
            if (Joystick::isButtonPressed(joystickNumber, 1) && timer == 0)
            {
                fistKey = 1; //Indikerar att det är fastFist som tryckts
                isFastFist = true;
                fastFist();
                ++timer;
            }
            // Om powerFist_key tryckts och timern är nollställd
            else if (Joystick::isButtonPressed(joystickNumber, 2) && timer == 0)
            {
                isStrongFist = true;
                fistKey = 2;
                ++timer;
            }
            //Om vi nått timerMax och fastFist skapats
            else if (timer == timerMax && fistKey == 1)
            {
                timer = 0;
            }
            //Om vi nått timerMax och powerFist skapats
            else if (timer == timerMax && fistKey == 2)
            {
                powerFist();
                ++timer;
            }
            //Om vi nått timerMax * 2, dvs vi väntat timerMax efter en powerFist
            else if (timer == timerMax * 2)
            {
                timer = 0;
            }
            //Om vi tryckt men inte nått max, räkna upp timern
            else if (timer != 0)
            {
                ++timer;
            }
        }
    }
}

// funktion för att sätta knappar
void player::setKeyBinding(sf::Keyboard::Key u, sf::Keyboard::Key d,
                           sf::Keyboard::Key l, sf::Keyboard::Key r,
                           sf::Keyboard::Key f, sf::Keyboard::Key v)
{
    up_key = u;
    down_key = d;
    left_key = l;
    right_key = r;
    fastFist_key = f;
    powerFist_key = v;
    block_key = d;
}

// Kollisionshantering med plattformar
void player::checkPlatformCollision(vector<platform>* platforms)
{
    // Loopa genom alla plattformar
    for(int i = 0; i != platforms->size(); ++i)
    {
        platform platform = platforms->at(i);
        // Om spelarens och plattformens hitbox kolliderar
        if(!(newTop > platform.bottom || newBottom < platform.top || newLeft > platform.right || newRight < platform.left))
        {
            //Om spelaren kommer ovanifrån och kolliderar
            if (newBottom >= platform.top && bottom < platform.top)
            {
                newPos.y = hitbox.getPosition().y;
                
                //Funktion som flyttar spelaren så långt det går vid kollision
                while(newPos.y + hitbox.getSize().y < platform.top - 1)
                {
                    newPos.y += 1;
                }
                
                inAir = false;
            }
            //Om spelaren kommer underifrån och kolliderar
            else if (newTop <= platform.bottom && top > platform.bottom)
            {
                newPos.y = hitbox.getPosition().y;
                
                //Funktion som flyttar spelaren så långt det går åt vänster vid kollision
                while(newPos.y > platform.bottom + 1)
                {
                    newPos.y -= 1;
                }
                
                velocity.y = 0;
            }
            //Om spelaren kommer från vänster och kolliderar
            if (newRight >= platform.left && right < platform.left)
            {
                newPos.x = hitbox.getPosition().x;
                
                //Funktion som flyttar spelaren så långt det går vid kollision
                while(newPos.x + hitbox.getSize().x < platform.left - 1)
                {
                    newPos.x += 1;
                }
                
                velocity.x = 0;
            }
            //Om spelar kommmer från höger och kolliderar
            if(newLeft <= platform.right && left > platform.right)
            {
                newPos.x = hitbox.getPosition().x;
                
                //Funktion som flyttar spelaren så långt det går åt höger vid kollision
                while(newPos.x > platform.right + 1)
                {
                    newPos.x -= 1;
                }
                
                velocity.x = 0;
            }
        }
    }
}

//Kollisionshantering för slag
void player::checkPlayerCollision(vector<player>* players)
{
    //Gå igenom varje spelare på spelpan
    for (int i = 0; i < players->size(); ++i)
    {
        player player = players->at(i);
        
        //Gå igenom alla nuvarande "fistar" från spelare i
        for (int j = 0; j != player.limbs.size(); ++j)
        {
            //Om kollision med nuvarande objekt och "fist" j från spelare i
            //och om limbnumber ej är samma som playernumber
            if (hitbox.getGlobalBounds().intersects(player.limbs[j].hitbox.getGlobalBounds()) &&
                playerNumber != player.limbs[j].limbNumber)
            {
                //Kollision med annan spelares fist!
                direction_x = player.facingDirection;
                
                //Critical hit
                int critical = rand() % (damage + 1);
                
                //Om blockar, bara ta skada
                if (isBlocking)
                {
                    damage += player.limbs[j].Power;
                }
                else
                {
                    //Om under 100% skada flyger fixt avstånd
                    if (damage < 100)
                    {
                        velocity.x = player.limbs[j].Power;
                        velocity.y = -1 * velocity.x/2;
                    }
                    //Om "critical hit" inträffar
                    else if(critical > maxdamage)
                    {
                        velocity.x = player.limbs[j].Power + player.limbs[j].Power*2*maxdamage/100;
                        velocity.y = -1 * velocity.x / 2;
                    }
                    else
                    {
                        // Värden på hur långt/kort spelaren "flyger" iväg vid slag
                        int velomax{static_cast<int>(damage + floor(pow(log(damage + 1),2)) + 1)*player.limbs[j].Power};
                        int velomin{static_cast<int>(damage - floor(pow(log(damage + 1),2)) + 1)*player.limbs[j].Power};
                        
                        // Tar slumpmässigt tal mellan velomin och velomax så bakåtstöten inte är linjär
                        velocity.x = (rand() % velomax + velomin)/100;
                        velocity.y = -1 * velocity.x / 2;
                    }
                    //Uppdatera skadeprocenten
                    damage += player.limbs[j].Power;
                    
                    inAir = true;
                    jumpCounter = 1;
                    newPos.x += velocity.x * direction_x;
                }
            }
        }
    }
}

//Funktion för att kolla när en spelare hamnat utanför banan
void player::checkPlayerOutside(Vector2u windowSize)
{
    //Skalningskonstanter
    int xScale = 1.2;
    int yScale = -0.2;
    
    Vector2f currentPosition; //Vektor med nuvarande position
    currentPosition.x = hitbox.getPosition().x;
    currentPosition.y = hitbox.getPosition().y;
    
    //Om nuvarande position är utanför en viss skala av fönstret har spelaren "dött"
    if(currentPosition.x > windowSize.x * xScale|| currentPosition.x < windowSize.x * yScale || currentPosition.y > windowSize.y)
    {
        //Spelaren "respawnar"
        lives -= 1;
        damage = 0;
        velocity = Vector2f(0,0);
        inAir = true;
        hitbox.setPosition(startCoordinates);
    }
}

//Funktion för att flytta spelar efter kollisionshantering
void player::move()
{
    // Flytta spelare
    hitbox.setPosition(newPos.x, newPos.y);
    
    // Sätt nya sidorna
    top = hitbox.getPosition().y;
    bottom = hitbox.getPosition().y + hitbox.getSize().y;
    left = hitbox.getPosition().x;
    right = hitbox.getPosition().x + hitbox.getSize().x;
    
    // Flytta spriten till hitbox position
    sprite.setPosition(hitbox.getPosition());
}

// Attacken fastFist
void player::fastFist()
{
    Vector2f limb_koord; // koordinater att spawna limb på
    
    // sätt x-koord beroende på riktning och position
    if (facingDirection == -1)
    {
        limb_koord.x = hitbox.getPosition().x - hitbox.getSize().x/8;
    }
    else
    {
        limb_koord.x = hitbox.getPosition().x + hitbox.getSize().x;
    }
    
    // sätt y-koord
    limb_koord.y = hitbox.getPosition().y + hitbox.getSize().y/3;
    
    //skapa limb med limb_koord
    limb fist(limb_koord, Vector2f(hitbox.getSize().x/8, hitbox.getSize().y/8), fastFistPower);
    
    // sätt limbNumber till spelarend nummer
    fist.limbNumber = playerNumber;
    
    // lägg till listan i spelarens lista med limbs
    limbs.push_back(fist);
}

// Attacken powerFist
void player::powerFist()
{
    //Koordinater att spawna limb på
    Vector2f limb_koord;
    //Sätt x-koord beroende på riktning och position
    if (facingDirection == -1)
    {
        limb_koord.x = hitbox.getPosition().x - hitbox.getSize().x/4;
    }
    else
    {
        limb_koord.x = hitbox.getPosition().x + hitbox.getSize().x;
    }
    
    // sätt y-koord
    limb_koord.y = hitbox.getPosition().y + hitbox.getSize().y/3;
    
    //skapa limb med limb_koord
    limb fist(limb_koord, Vector2f(hitbox.getSize().x/4, hitbox.getSize().y/4), strongFistPower);
    
    // sätt limbNumber till spelarend nummer
    fist.limbNumber = playerNumber;
    
    // lägg till listan i spelarens lista med limbs
    limbs.push_back(fist);
    
}

//Hjälpfunktion för rörelse
void player::leftRightMovement()
{
    //Om spelarens hastighet är större än movementSpeed
    if (velocity.x > movementSpeed)
    {
        //Subtrahera luftmotståndskonstanten
        velocity.x -= airMovement;
    }
    else
    {
        velocity.x = movementSpeed;
    }
    newPos.x += velocity.x * direction_x;
}

//Funktion för animation av slag
void player::updateSprite()
{
    if (isBlocking)
    {
        if (blockSpriteFrequency == 0)
        {
            blockSpriteFrequency = 5;
            ++blockSpriteCounter;
        }
        if (blockSpriteCounter == 1)
        {
            --blockSpriteFrequency;
            if (facingDirection == -1)
            {
                sprite.setTextureRect(sf::IntRect(34, 280, 34, 40));
            }
            else
            {
                sprite.setTextureRect(sf::IntRect(34, 240, 34, 40));
            }
        }
        else if (blockSpriteCounter == 2)
        {
            if (facingDirection == -1)
            {
                sprite.setTextureRect(sf::IntRect(2 * 34, 280, 34, 40));
            }
            else
            {
                sprite.setTextureRect(sf::IntRect(2 * 34, 240, 34, 40));
            }
        }
    }
    //Om snabba slaget används
    else if (isFastFist)
    {
        //uppdatera räknare föra att animera sprite på ett snyggt sätt
        ++counterFastFist;
        if(counterFastFist == fastFistFrequency)
        {
            counterFastFist = 0;
            ++fastSpriteCounter;
            if (fastSpriteCounter == 2)
            {
                isFastFist = false;
                fastSpriteCounter = 0;
            }
        }
        // Ändra spelarens sprite beroende på riktingen
        if (facingDirection == -1)
        {
            sprite.setTextureRect(sf::IntRect(fastSpriteCounter * 40, 80, 40, 40));
        }
        else
        {
            sprite.setTextureRect(sf::IntRect(fastSpriteCounter * 40, 120, 40, 40));
        }
    }
    //Om snabba slaget används
    else if (isStrongFist)
    {
        //uppdatera räknare föra att animera sprite på ett snyggt sätt
        ++counterStrongFist;
        if(counterStrongFist == strongFistFrequency)
        {
            counterStrongFist = 0;
            ++strongSpriteCounter;
            if (strongSpriteCounter == 4)
            {
                isStrongFist = false;
                strongSpriteCounter = 0;
            }
        }
        // Ändra spelarens sprite beroende på riktingen
        if (facingDirection == -1)
        {
            sprite.setTextureRect(sf::IntRect(strongSpriteCounter * 40, 160, 40, 40));
        }
        else
        {
            sprite.setTextureRect(sf::IntRect(strongSpriteCounter * 40, 200, 40, 40));
        }
    }
    else
    {
        // Ändra spelarens sprite beroende på riktingen
        if (facingDirection == -1)
        {
            sprite.setTextureRect(sf::IntRect(counterSprite * 34, 0, 34, 40));
        }
        else
        {
            sprite.setTextureRect(sf::IntRect(counterSprite * 34, 40, 34, 40));
        }
        
        // Om spelaren rör på sig updatera sprites enligt räknare
        if (isWalking)
        {
            if (counterWalking == stepFrequency)
            {
                counterSprite++;
                counterWalking = 0;
                if (counterSprite == 6)
                {
                    counterSprite = 0;
                }
            }
            ++counterWalking;
        }
        else
        {
            counterSprite = 0;
        }
    }
}