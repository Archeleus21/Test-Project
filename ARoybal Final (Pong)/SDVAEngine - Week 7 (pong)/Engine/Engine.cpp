
#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <ctime>
#include <math.h>
#include <iostream>	
#include <sstream>
#include <SFML/Audio.hpp>	
#include <SFML/Audio/Music.hpp>

int main()
{
	int windowWidth = 1024.0f;
	int windowHeight = 512.0f;
	int paddleWidth = 40.0f;
	int paddleHeight = 90.0f;
	int powerUpSize = 40.0f;
	int ballSize = 40.0f;
	int player1Score = 0;
	int player2Score = 0;
	int ballTex = 16;
	int playerTex = 16;
	int playerTurn = 0;
	//
	//Mario projectile
	//

	sf::RectangleShape fireBall(sf::Vector2f(20, 20));
	fireBall.setPosition(sf::Vector2f(512, 100));

	//
	//Mario Proctire Texture
	//
	sf::Texture fireBallTexture;
	
	if (!fireBallTexture.loadFromFile("Texture/Enemies1.png", sf::IntRect(8, 589, 15, 15)))
	{

		return 1;
	}

	fireBall.setTexture(&fireBallTexture);

	//
	//powerups
	//
	srand(static_cast<unsigned int>(time(0)));
	int xSuperMushroom = rand() % 1024;
	int ySuperMushroom = rand() % 512;
	bool powerUp = false;

	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Welcome to SDVA 203!");

	//
	//Background
	//
	sf::RectangleShape backGround(sf::Vector2f(windowWidth, windowHeight));
	backGround.setFillColor(sf::Color(255,255,255,128));

	//
	//background texture
	//

	sf::Texture backGroundTex;
	if (!backGroundTex.loadFromFile("Texture/MarioWorld.png", sf::IntRect(3200, 30, 325, 180)))
	{
		return 1;
	}

	backGround.setTexture(&backGroundTex);

	//
	//mario Logo
	//
	sf::RectangleShape marioLogo(sf::Vector2f(windowWidth / 3.0f, windowHeight / 3.0f));
	marioLogo.setFillColor(sf::Color(255, 255, 255, 128));
	marioLogo.setPosition(sf::Vector2f(512, 100));
	marioLogo.setOrigin(marioLogo.getGlobalBounds().width / 2.0f, marioLogo.getGlobalBounds().height / 2.0f);

	//
	//Mario logo texture
	//

	sf::Texture marioLogoTex;
	if(!marioLogoTex.loadFromFile("Texture/MarioWorld.png", sf::IntRect(276, 325, 176, 100)))
	{
		return 1;
	}

	marioLogo.setTexture(&marioLogoTex);
	
	//
	//SuperMushroom
	//

	sf::RectangleShape superMushroom(sf::Vector2f(75, 75));
	//superMushroom.setPosition(512, 256);

	//
	//SuperMushroom texture
	//
	sf::Texture superMushroomTex;
	if (!superMushroomTex.loadFromFile("Texture/Items & Objects.png", sf::IntRect(0, 16, 16, 16)))
	{
		return 1;
	}

	superMushroom.setTexture(&superMushroomTex);

	//
	//ball
	//
	sf::RectangleShape shape(sf::Vector2f(ballSize, ballSize));
	shape.setFillColor(sf::Color::White);
	shape.setPosition((windowWidth / 2.0f) - 5.0f, (windowHeight / 2.0f) - 5.0f);

	//
	//texture for ball
	//

	sf::Texture ballTexture;
	if (!ballTexture.loadFromFile("Texture/Enemies.png", sf::IntRect(ballTex * 11, ballTex, 16, 16)))
	{
		return 1;
	}

	shape.setTexture(&ballTexture);

	//
	//Player 1
	//
	sf::RectangleShape player1(sf::Vector2f(paddleWidth, paddleHeight));
	player1.setPosition(3.0f, 1.0f);

	//
	//set texture for player 1
	//

	sf::Texture player1Texture;
	if (!player1Texture.loadFromFile("Texture/Mario & Luigi.png", sf::IntRect(playerTex * 5.0f, 40, 16, 16)))
	{
		return 1;
	}

	player1.setTexture(&player1Texture);

	//
	//player 2
	//

	sf::RectangleShape player2(sf::Vector2f(paddleWidth, paddleHeight));
	player2.setPosition(sf::Vector2f(windowWidth - (paddleWidth + 3.0f) , 1.0f));

	//
	//set texture for player 2
	//

	sf::Texture player2Texture;
	if (!player2Texture.loadFromFile("Texture/Mario & Luigi.png", sf::IntRect(80, 355, 16, 16)))
	{
		return 1;
	}

	player2.setTexture(&player2Texture);
	
	float playerMoveUp = -600.0f;
	float playerMoveDown = 600.0f;
	float elapsedTime = 0.0f;
	float powerSpawnTime = 0.0f;
	float powerDuration = 0.0f;
	bool player1Jumping = false;
	bool player2Jumping = false;
	bool player1PoweredUp = false;
	bool player2PoweredUp = false;

	float xBall = 0.0f;
	float yBall = 0.0f;
	float ballSpeed = 200.0f;
	float acceleration = 0.0f;
	const float PI = 3.1415; //PI
	float radiansToDegrees = 2 * PI / 360;//to convert angle to radians
	float diagX = sin(35 * radiansToDegrees) * ballSpeed;
	float diagY = cos(35 * radiansToDegrees) * ballSpeed;
	float xBallVelocity;
	float yBallVelocity;

	bool startGame = false;

	sf::Clock clock;

	//
	//set audio file to be used in when ball bounces off paddles and walls
	//
	sf::SoundBuffer ballBounceBuffer;

	if (!ballBounceBuffer.loadFromFile("Audio/smb_kick.wav"))
	{
		return 1;
	}

	//
	//sound used when ball bounces off roof and floor
	//
	sf::SoundBuffer ballFloorRoofBuffer;

	if (!ballFloorRoofBuffer.loadFromFile("Audio/smb_bump.wav"))
	{
		return 1;
	}
	//
	//sound used when the player scores
	//
	sf::SoundBuffer marioScoreBuffer;

	if (!marioScoreBuffer.loadFromFile("Audio/mario-boing.wav"))
	{
		return 1;
	}

	sf::SoundBuffer luigiScoreBuffer;

	if (!luigiScoreBuffer.loadFromFile("Audio/Luigi85 sorry.wav"))
	{
		return 1;
	}

	//
	//sound used when the player wins
	//

	sf::SoundBuffer marioWinBuffer;

	if (!marioWinBuffer.loadFromFile("Audio/Mariothankyou.wav"))
	{
		return 1;
	}

	sf::SoundBuffer luigiWinBuffer;

	if (!luigiWinBuffer.loadFromFile("Audio/Luigithankyou.wav"))
	{
		return 1;
	}

	//
	//sound for powerup spawn
	//
	sf::SoundBuffer powerUpSpawn;
	if (!powerUpSpawn.loadFromFile("Audio/smb_powerup_appears.wav"))
	{
		return 1;
	}

	//
	//sound for gaining powerup
	//

	sf::SoundBuffer gainPowerUp;
	if (!gainPowerUp.loadFromFile("Audio/smb_powerup.wav"))
	{
		return 1;
	}

	//
	//sound for losing powerup
	//

	sf::SoundBuffer losePowerUp;
	if (!losePowerUp.loadFromFile("Audio/smb2_shrink.wav"))
	{
		return 1;
	}

	//
	//set audio file to a sound variable
	//
	sf::Sound ballBounceSound;  //off player paddles
	ballBounceSound.setBuffer(ballBounceBuffer);

	sf::Sound marioScoreSound;  //points scored
	marioScoreSound.setBuffer(marioScoreBuffer);

	sf::Sound luigiScoreSound;  //points scored
	luigiScoreSound.setBuffer(luigiScoreBuffer);

	sf::Sound marioWinSound;  //win
	marioWinSound.setBuffer(marioWinBuffer);

	sf::Sound luigiWinSound;  //win
	luigiWinSound.setBuffer(luigiWinBuffer);

	sf::Sound floorRoofSound;  //bounce off top and bottom of window
	floorRoofSound.setBuffer(ballFloorRoofBuffer);

	sf::Sound powerUpSpawnSound;
	powerUpSpawnSound.setBuffer(powerUpSpawn);

	sf::Sound gainPowerUpSound;
	gainPowerUpSound.setBuffer(gainPowerUp);

	sf::Sound losePowerUpSound;
	losePowerUpSound.setBuffer(losePowerUp);

	//
	//put music in the game
	//
	sf::Music gamePlayMusic;
	if (!gamePlayMusic.openFromFile("Audio/super_mario_medley.ogg"))
	{
		return 1;
	}

	sf::Font myFont;
	if (!myFont.loadFromFile("Fonts/unispacerg.ttf"))
	{
		return 1;
	}


	//
	//make a score board
	//
	sf::Text scoreBoard;
	scoreBoard.setFont(myFont);
	scoreBoard.setString("Score: ");
	scoreBoard.setPosition(512, 0);
	scoreBoard.setCharacterSize(20);
	scoreBoard.setOrigin(scoreBoard.getGlobalBounds().width / 2.0f, 0);

	//
	//display the winner
	//
	sf::Text winner;

	gamePlayMusic.play();
	gamePlayMusic.setLoop(true);

	while (window.isOpen())
	{

		sf::Time deltaTime = clock.restart();
		float dt = deltaTime.asSeconds();
		elapsedTime += dt;
		powerSpawnTime += dt;
		powerDuration += dt;

		//
		//spawn power up
		//


		if (powerSpawnTime >= 7.0f)
		{
		powerUp = true;

		}

		xBallVelocity = diagX * dt;
		yBallVelocity = diagY * dt;

		//
		//display the players score
		//

		std::ostringstream myStringStream;			
		std::ostringstream myStringStream1;

		myStringStream << "Score: \n" << player1Score << "   " << player2Score << std::endl;

		scoreBoard.setString(myStringStream.str());

		//
		//Deteremine a winner after 5 points and reset the scores.
		//
		if (player1Score >= 5)
		{
			winner.setFont(myFont);
			winner.setString("Player 1 wins!");
			winner.setPosition(512, 256);
			winner.setCharacterSize(50);
			winner.setColor(sf::Color::Black);
			winner.setOrigin(winner.getGlobalBounds().width / 2.0f, winner.getGlobalBounds().height / 2.0f);

			marioWinSound.play();

			player1Score = 0;
			player2Score = 0;
		}
		else if (player2Score >= 5)
		{
			winner.setFont(myFont);
			winner.setString("Player 2 wins!");
			winner.setPosition(512, 256);
			winner.setCharacterSize(50);
			winner.setColor(sf::Color::Black);
			winner.setOrigin(winner.getGlobalBounds().width / 2.0f, winner.getGlobalBounds().height / 2.0f);

			luigiWinSound.play();

			player1Score = 0;
			player2Score = 0;
		}

		sf::Event event;
		while (window.pollEvent(event))
		{

			if (event.type == sf::Event::KeyPressed)
			{
				//
				//plays sound when lcontrol is pressed
				//
				if (event.key.code == sf::Keyboard::LControl)
				{
					ballBounceSound.play();
				}

				if (event.key.code == sf::Keyboard::Space)
				{

					
					//
					//spawn power up
					//

					powerSpawnTime = 0.0f;

					startGame = !startGame;
				}
			}
			if (event.type == sf::Event::Closed)
				window.close();
		}

		//
		//Lets move the ball and get it to bounce
		//

	

		if (startGame == true)
		{
			winner.setString("");
			
			if (playerTurn == 0)
			{
				shape.move(xBallVelocity, yBallVelocity);
				
			}
			else
			{
				shape.move(-xBallVelocity, yBallVelocity);
			}

			if (ballSpeed >= 1000.0f)
			{
				ballSpeed = 1000.0f;
			}


		}


		if (shape.getPosition().y > windowHeight - ballSize)
		{

			floorRoofSound.play();			
			shape.setPosition(shape.getPosition().x, windowHeight - ballSize);

			diagY = -diagY;
			
		}
		else if (shape.getPosition().y < 0.0f)
		{
			floorRoofSound.play();
			shape.setPosition(shape.getPosition().x, 0.0f);

			diagY = -diagY;
		}

		//
		//reset the ball after player misses and play sound for score
		//
		if (shape.getPosition().x > 1024)
		{
			shape.setPosition((windowWidth / 2.0f) - 5.0f, (windowHeight / 2.0f) - 5.0f);
			shape.move(0, 0);
			diagX = 200;
			player1Score += 1;
			startGame = false;		

			if (player1Score < 5)
			{
				marioScoreSound.play();
			}
		}

		else if (shape.getPosition().x < 0)
		{
			shape.setPosition((windowWidth / 2.0f) - 5.0f, (windowHeight / 2.0f) - 5.0f);
			shape.move(0, 0);
			diagX = 200;
			player2Score += 1;
			startGame = false;

			if (player2Score < 5)
			{
				luigiScoreSound.play();
			}
		}

		//
		//lets get the player position and move the players paddles
		//

		//printf("Player1 position: [%f][%f] \n", player1.getPosition().x, player1.getPosition().y);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{

			if (player1.getPosition().y < windowHeight - player1.getSize().y)
			{
				player1.move(0.0f, playerMoveDown * deltaTime.asSeconds());

			}
			else //used to move the player within the bounds of the window instead of being a slight negative value.
			{
				player1.setPosition(3.0f, windowHeight - player1.getSize().y);
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			if (player1.getPosition().y > 1.0f)
			{
				player1.move(0.0f, playerMoveUp * deltaTime.asSeconds());

			}
			else
			{
				player1.setPosition(3.0f, 1.0f);
			}
		}


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{


			if (player2.getPosition().y < windowHeight - player2.getSize().y)
			{
				player2.move(0.0f, playerMoveDown * deltaTime.asSeconds());

			}
			else
			{
				player2.setPosition(windowWidth - (paddleWidth + 3.0f), windowHeight - player2.getSize().y);
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{


			if (player2.getPosition().y > 1.0f)
			{
				player2.move(0.0f, playerMoveUp * deltaTime.asSeconds());

			}
			else
			{
				player2.setPosition(windowWidth - (paddleWidth + 3.0f), 1.0f);
			}
		}
		//
		//lets make the paddles change the direction of the ball and increase its speed.
		//

		if (shape.getPosition().x > player1.getPosition().x && shape.getPosition().x < player1.getPosition().x + player1.getSize().x &&
			shape.getPosition().y > player1.getPosition().y && shape.getPosition().y < player1.getPosition().y + player1.getSize().y)
		{	
			elapsedTime = 0.0f;
			player1Jumping = true;
			ballBounceSound.play();
			diagX *= -1.1f;
			shape.setPosition(player1.getPosition().x + paddleWidth, shape.getPosition().y);		

			std::cout << "change texture 1" << std::endl;

			playerTurn = 0;

			//
			//change texture when ball hits player
			//
			player1Texture.loadFromFile("Texture/Mario & Luigi.png", sf::IntRect(155, 40, 16, 16));

			player1.setTexture(&player1Texture);

		}

		if (player1Jumping && elapsedTime > 0.2f)
		{
			player1Jumping = false;
			std::cout << "Paddle setting texture" << std::endl;
			// player1.setTextureRect(sf::IntRect(80, 40, 16, 16));
			player1Texture.loadFromFile("Texture/Mario & Luigi.png", sf::IntRect(80.0f, 40, 16, 16));
			 //player1.setTexture(&player1Texture);

		}

		if (shape.getPosition().x + ballSize > player2.getPosition().x && shape.getPosition().x + ballSize < player2.getPosition().x + player2.getSize().x &&
			shape.getPosition().y > player2.getPosition().y && shape.getPosition().y < player2.getPosition().y + player2.getSize().y)
		{	
			elapsedTime = 0.0f;	
			player2Jumping = true;
			ballBounceSound.play();

			std::cout << "change texture 2" << std::endl;

			diagX *= -1.1f;
	
			shape.setPosition(player2.getPosition().x - ballSize, shape.getPosition().y);
			playerTurn = 1;
			
			//
			//change texture when ball hits player
			//

			!player2Texture.loadFromFile("Texture/Mario & Luigi.png", sf::IntRect(155, 355, 16, 16));
			//player2.setTexture(&player2Texture);
		}

		if (player2Jumping && elapsedTime > 0.2f)
		{
			player2Jumping = false;
			std::cout << "Paddle setting texture 2" << std::endl;

			!player2Texture.loadFromFile("Texture/Mario & Luigi.png", sf::IntRect(80.0f, 355, 16, 16));
			//player2.setTexture(&player2Texture);

		}

		//
		//ball collides with powerup for player 1
		//

		if (shape.getPosition().x > superMushroom.getPosition().x && shape.getPosition().x < superMushroom.getPosition().x + superMushroom.getSize().x &&
			shape.getPosition().y > superMushroom.getPosition().y && shape.getPosition().y < superMushroom.getPosition().y + superMushroom.getSize().y)
		{

			player1PoweredUp = true;
			powerDuration = 0.0f;
			powerSpawnTime = 0.0f;
			powerUp = false;

			gainPowerUpSound.play();

			if (playerTurn == 0)
			{
				player1.setSize(sf::Vector2f(paddleWidth, paddleHeight + powerUpSize));
				//!player1Texture.loadFromFile("Texture/Mario & Luigi.png", sf::IntRect(80, 33, 16, 33));
				//player1.setTexture(&player1Texture);
			}
		}

		if (player1PoweredUp && powerDuration >= 5.0f)
		{	
			player1PoweredUp = false;
			std::cout << "Power up!" << std::endl;
			player1.setSize(sf::Vector2f(paddleWidth, paddleHeight));
			//!player1Texture.loadFromFile("Texture/Mario & Luigi.png", sf::IntRect(80.0f, 40, 16, 16));
			
			losePowerUpSound.play();

		}
		//
		//ball collides with powerup for player 2
		//

		if (shape.getPosition().x > superMushroom.getPosition().x && shape.getPosition().x < superMushroom.getPosition().x + superMushroom.getSize().x &&
			shape.getPosition().y > superMushroom.getPosition().y && shape.getPosition().y < superMushroom.getPosition().y + superMushroom.getSize().y)
		{
			player2PoweredUp = true;
			powerDuration = 0.0f;
			powerSpawnTime = 0.0f;
			powerUp = false;
			
			gainPowerUpSound.play();

			if (playerTurn == 1)
			{

				player2.setSize(sf::Vector2f(paddleWidth, paddleHeight + powerUpSize));
				//!player2Texture.loadFromFile("Texture/Mario & Luigi.png", sf::IntRect(80, 316, 16, 33));

			}

		}
		
		if (player2PoweredUp && powerDuration >= 5)
		{	
			player2PoweredUp = false;
			player2.setSize(sf::Vector2f(paddleWidth, paddleHeight));
			//!player2Texture.loadFromFile("Texture/Mario & Luigi.png", sf::IntRect(80, 355, 16, 16));
			
			losePowerUpSound.play();
		}


		//
		//lets print the window and all images in the window
		//

		window.clear();
		window.draw(backGround);
		window.draw(marioLogo);
		window.draw(shape);
		window.draw(fireBall);
		window.draw(player1);
		window.draw(player2);
		window.draw(scoreBoard);
		window.draw(winner);	
		
		if (powerUp == true)
		{
			std::cout<<"Power up spawned" << std::endl;
			window.draw(superMushroom);
			superMushroom.setPosition(sf::Vector2f(512, ySuperMushroom));

			if (superMushroom.getPosition().y >= 437)
			{
				superMushroom.setPosition(sf::Vector2f(512, 437));
			}
		}
		window.display();
	}

	return 0;
}
