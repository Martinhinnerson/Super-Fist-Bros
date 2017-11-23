//
// Disclamer:
// ----------
//
// This code will work only if you selected window, graphics and audio.
//
// Note that the "Run Script" build phase will copy the required frameworks
// or dylibs to your application bundle so you can execute it on any OS X
// computer.
//
// Your resource files (images, sounds, fonts, ...) are also copied to your
// application bundle. To get the path to these resource, use the helper
// method resourcePath() from ResourcePath.hpp
//

#include <SFML/Graphics.hpp>
#include <iostream>
#include "player.hpp"
#include "platform.hpp"
#include "game.hpp"
#include "Menu.hpp"

// Here is a small helper for you ! Have a look.
#include "ResourcePath.hpp"

using namespace std;
using namespace sf;

int main(int, char const**)
{
    // Skapa main window
    RenderWindow window(VideoMode::getFullscreenModes().at(0), "Fist window");
    window.setFramerateLimit(60);

    // Sätt rätt ikon
    Image icon;
    if (!icon.loadFromFile(resourcePath() + "fist.png")) {
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    
    // Ladda font
    sf::Font font;
    if (!font.loadFromFile(resourcePath() + "sansation.ttf"))
    {
        return EXIT_FAILURE;
    }
    
    // Ladda texturer för spelare, platformen och bakgrunden
    sf::Texture texturePlayer;
    if (!texturePlayer.loadFromFile(resourcePath() + "Fatmansheet.png"))
    {
        return EXIT_FAILURE;
    }
    
    sf::Texture texturePlayer2;
    if (!texturePlayer2.loadFromFile(resourcePath() + "Fatmansheet2.png"))
    {
        return EXIT_FAILURE;
    }
     
    sf::Texture texturePlatform;
    if (!texturePlatform.loadFromFile(resourcePath() + "final_destination.png"))
    {
        return EXIT_FAILURE;
    }
    
    sf::Texture textureBackround;
    if (!textureBackround.loadFromFile(resourcePath() + "backround.jpg"))
    {
        return EXIT_FAILURE;
    }
    
    sf::Texture textureBackround2;
    if (!textureBackround2.loadFromFile(resourcePath() + "backround_controls.png"))
    {
        return EXIT_FAILURE;
    }
    
    // Skapa en sprite för bakgrunden
    Sprite backround(textureBackround);
    backround.setScale(window.getSize().x/backround.getLocalBounds().width,
                       window.getSize().y/backround.getLocalBounds().height);

    Sprite backroundControls(textureBackround2);
    backroundControls.setScale(window.getSize().x/backround.getLocalBounds().width,
                       window.getSize().y/backround.getLocalBounds().height);
    
    map<const std::string, const sf::Texture&> textures{{"Player1", texturePlayer}, {"Player2", texturePlayer2}, {"Platform1", texturePlatform}};

    // Skapa en instans av game
    game game;
    menu menu{window.getSize(), font};
    
    // Starta spel-loopen
    while (window.isOpen())
    {
        // Hantera event
        Event event;
        while (window.pollEvent(event))
        {
            // Event-typ Close window: exit
            if (event.type == Event::Closed) {
                window.close();
           }
            // Event-typ Escape pressed: exit eller share på ps4 controller
            if ((event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) ||
                (Joystick::isConnected(0) && event.type == Event::JoystickButtonPressed && event.joystickButton.button == 8))
            {
                // Om i meny, avsluta spel
                if(!game.gameRunning)
                {
                    window.close();
                }
                // Visa meny, återställ spelet
                else
                {
                    game.gameRunning = false;
                    menu.menustate = 1;
                    game.players.clear();
                    game.platforms.clear();
                }
            }
            // Om i meny
            if (!game.gameRunning)
            {
                //Spelet kan startas med enter eller options på ps4 controller
                if ((event.type == Event::KeyPressed && event.key.code == Keyboard::Return) ||
                    (Joystick::isConnected(0) && event.type == Event::JoystickButtonPressed && event.joystickButton.button == 9))
                {
                    // Starta spelet
                    game.gameRunning = true;
                    // Menustate = 0 => Spelet körs
                    menu.menustate = 0;
                }
                //Hantera tryck med musknapp 0 (vänster musknapp)
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == 0)
                {
                    menu.ButtonPress(menu.Checkbutton(sf::Mouse::getPosition(window)).codename,
                                     game, window);
                }
                // Menustate = 0 => Initiera spelet
                if (menu.menustate == 0)
                {
                    game.init(window.getSize(), font, textures);
                    // Flytta spelarna till startpositioner
                    game.updateMovement();
                    game.move();
                }
            }
        }
        // Om spelet är igång
        if(game.gameRunning)
        {
            if (game.startTimer == 0)
            {
                //Ta bort alla fistar
                game.clearLimbs();
                
                // Om nån spelare har förlorat har den andra vunnit
                if (game.checkWin())
                {
                    //skriv ut i terminalen vem som vinner och återställ spelarnas liv och position
                    for(int i = 0; i < game.players.size(); ++i)
                    {
                        player player{game.players[i]};
                        if (player.lives > 0)
                        {
                            cout << "Player " << player.playerNumber << " wins!" << endl;
                        }
                        player.lives = 5;
                        player.hitbox.setPosition(player.startCoordinates);
                    }
                    //återställ menu och spelet och radera nuvarande platformar och spelare
                    game.gameRunning = false;
                    menu.menustate = 1;
                    game.players.clear();
                    game.platforms.clear();
                    
                    //hoppa över resten och starta om while-loopen
                    continue;
                }
                
                // Kolla om spelare är utanför spelplanen
                game.checkOutside(window.getSize());
                
                // Hantera knapptryckningar (förflyttningar och slag)
                game.updateMovement();
                game.updateFist();
                
                // Kontrollera och hantera kollision mellan objekt
                game.checkCollision();
                
                // Uppdatera spelplanen efter att kollision är kontorllerad
                game.move();
            }
            else
            {
                //uppdatera delay
                game.startDelay(window);
            }
                
            // Rensa allt på skärmen och rita allt igen
            window.clear();
            
            // Rita bakgrund
            window.draw(backround);
            
            // Stega vidare i animeringen för platform och spelare
            game.updatePlatforms();
            game.updatePlayerSprites();
            
            // Loop igenom alla platformar och rita dessa
            for (int i = 0; i != game.platforms.size(); ++i)
            {
                window.draw(game.platforms[i].sprite);
            }
            
            // Loopa igenom alla spelare och deras "limbs" och rita dessa
            // tillsammans med deras status
            for (int i =  0; i != game.players.size(); ++i)
            {
                if(game.players[i].lives != 0)
                {
                    // Rita spelarnas sprites
                    window.draw(game.players[i].sprite);
                }
                // Rita status
                game.players[i].status.setString("Player " + to_string(game.players[i].playerNumber) + "\nLives: " + to_string(game.players[i].lives) + "\nDamage: " + to_string(game.players[i].damage) + " \%");
                window.draw(game.players[i].status);
            }
            if (game.startTimer > 0)
            {
                window.draw(game.startText);
            }
        }
        // Om i meny, rita nuvarande meny
        else
        {
            if (game.startTimer == 0)
            {
                game.startTimer = game.startTimerMax;
            }
            window.clear();
            if(menu.menustate == 2)
            {
                window.draw(backroundControls);
            }
            else
            {
                window.draw(backround);
            }
            menu.Draw(window);
        }
        
        // Update the window
        window.display();
    }
    return EXIT_SUCCESS;
}
