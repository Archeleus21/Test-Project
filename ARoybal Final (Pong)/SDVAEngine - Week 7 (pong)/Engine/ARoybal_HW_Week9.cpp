
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
	int player1Score = 0;
	int player2Score = 0;

	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Welcome to SDVA 203!");
	sf::CircleShape shape(10.0f);
	shape.setFillColor(sf::Color::White);
	shape.setOutlineThickness(2.0);
	shape.setOutlineColor(sf::Color::Green);
	shape.setPosition((windowWidth / 2.0f) - 5.0f, (windowHeight / 2.0f) - 5.0f);
	sf::RectangleShape player1(sf::Vector2f(20.0f, 75.0f));
	player1.setFillColor(sf::Color::Red);
	player1.setOutlineColor(sf::Color::White);
	player1.setOutlineThickness(1.0f);
	player1.setPosition(3.0f, 1.0f);
	sf::RectangleShape player2(sf::Vector2f(20.0f, 75.0f));
	player2.setFillColor(sf::Color::Blue);
	player2.setOutlineColor(sf::Color::White);
	player2.setOutlineThickness(1.0f);
	player2.setPosition(sf::Vector2f(windowWidth - 23.0f, 1.0f));

	float playerMoveUp = -600.0f;
	float playerMoveDown = 600.0f;

	float xBall = 0.0f;
	float yBall = 0.0f;
	float ballSpeed = 200.0f;
	float acceleration = 0.0f;
	const float PI = 3.1415; //PI
	float radiansToDegrees = 2 * PI / 360;//to convert angle to radians
	float diagX = sin(45 * radiansToDegrees) * ballSpeed;
	float diagY = cos(45 * radiansToDegrees) * ballSpeed;
	float xBallVelocity;
	float yBallVelocity;

	bool startGame = false;

	sf::Clock clock;

	//
	//set audio file to be used in when ball bounces off paddles and walls
	//
	sf::SoundBuffer ballBounceBuffer;

	if (!ballBounceBuffer.loadFromFile("Audio/Blip_Select9.wav"))
	{
		return 1;
	}
	//
	//sound used when the player scores
	//
	sf::SoundBuffer playerScoreBuffer;

	if (!playerScoreBuffer.loadFromFile("Audio/PlayerScore.wav"))
	{
		return 1;
	}

	//
	//set audio file to a sound variable
	//
	sf::Sound ballBounceSound;
	ballBounceSound.setBuffer(ballBounceBuffer);

	sf::Sound playerScoreSound;
	playerScoreSound.setBuffer(playerScoreBuffer);

	//
	//put music in the game
	//
	sf::Music gamePlayMusic;
	if (!gamePlayMusic.openFromFile("Audio/10_Arpanauts.ogg"))
	{
		return 1;
	}

	sf::Font myFont;
	if (!myFont.loadFromFile("Fonts/unispacerg.ttf"))
	{
		return 1;
	}

	sf::Text scoreBoard;
	scoreBoard.setFont(myFont);
	scoreBoard.setString("Score: ");
	scoreBoard.setPosition(512, 0);
	scoreBoard.setCharacterSize(20);
	scoreBoard.setOrigin(scoreBoard.getGlobalBounds().width / 2.0f, 0);

	sf::Text winner;
	//winner.setFont(myFont);
	//winner.setString("wins!");
	//winner.setPosition(512, 256);
	//winner.setCharacterSize(50);
	//winner.setOrigin(winner.getGlobalBounds().width / 2.0f, winner.getGlobalBounds().height /2.0f);

	gamePlayMusic.play();

	while (window.isOpen())
	{

		sf::Time deltaTime = clock.restart();

		xBallVelocity = diagX * deltaTime.asSeconds();
		yBallVelocity = diagY * deltaTime.asSeconds();

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
			winner.setOrigin(winner.getGlobalBounds().width / 2.0f, winner.getGlobalBounds().height / 2.0f);

			player1Score = 0;
			player2Score = 0;
		}
		else if (player2Score >= 5)
		{
			winner.setFont(myFont);
			winner.setString("Player 2 wins!");
			winner.setPosition(512, 256);
			winner.setCharacterSize(50);
			winner.setOrigin(winner.getGlobalBounds().width / 2.0f, winner.getGlobalBounds().height / 2.0f);


			player1Score = 0;
			player2Score = 0;
		}

		/*

		//
		//lets make the paddles change to white when the mouse is within the paddles
		//
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);

		if (mousePos.x > player1.getPosition().x &&
		mousePos.x < player1.getPosition().x + player1.getSize().x && mousePos.y > player1.getPosition().y &&
		mousePos.y < player1.getPosition().y + player1.getSize().y)
		{
		player1.setFillColor(sf::Color::White);
		}
		else
		{
		player1.setFillColor(sf::Color::Red);
		}

		if (mousePos.x > player2.getPosition().x &&
		mousePos.x < player2.getPosition().x + player2.getSize().x && mousePos.y > player2.getPosition().y &&
		mousePos.y < player2.getPosition().y + player2.getSize().y)
		{
		player2.setFillColor(sf::Color::White);
		}
		else
		{
		player2.setFillColor(sf::Color::Blue);
		}

		*/

		//
		//Now lets instead reverse the direction of the ball when it "collides" with each paddle.  use the same code as above, but, instead of 
		//changing the color, reverse the ball direction
		//
		/*
		if (shape.getPosition().x > player1.getPosition().x && shape.getPosition().x < player1.getPosition().x + player1.getSize().x &&
		shape.getPosition().y > player1.getPosition().y && shape.getPosition().y < player1.getPosition().y + player1.getSize().y)
		{
		diagX *= -1.0f;
		diagY *= -1.0f;
		}

		if (shape.getPosition().x > player2.getPosition().x && shape.getPosition().x < player1.getPosition().x + player1.getSize().x &&
		shape.getPosition().y > player2.getPosition().y && shape.getPosition().y < player2.getPosition().y + player2.getSize().y)
		{
		diagX *= -1.0f;
		diagY *= -1.0f;
		}
		*/

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

				//
				//plays sound when z is pressed
				//
				if (event.key.code == sf::Keyboard::Z)
				{
					playerScoreSound.play();
				}

				if (event.key.code == sf::Keyboard::Space)
				{

				
					startGame = !startGame;
				}
			}
			if (event.type == sf::Event::Closed)
				window.close();
		}

		//
		//Lets move the ball and get it to bounce
		//and print its position
		//

		printf("Ball Position: [%f][%f]", shape.getPosition().x, shape.getPosition().y);

	

		if (startGame == true)
		{
			winner.setString("");
			shape.move(xBallVelocity, yBallVelocity);

		}


		if (shape.getPosition().y > windowHeight - 26.0f)
		{
			ballBounceSound.play();
			diagY = -diagY;
		}
		else if (shape.getPosition().y < 0.9f)
		{
			ballBounceSound.play();
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
			playerScoreSound.play();
			player1Score += 1;
			startGame = false;		}
		else if (shape.getPosition().x < 0)
		{
			shape.setPosition((windowWidth / 2.0f) - 5.0f, (windowHeight / 2.0f) - 5.0f);
			shape.move(0, 0);
			diagX = 200;
			playerScoreSound.play();
			player2Score += 1;
			startGame = false;
		}

		//
		//lets get the player position and move the players paddles
		//

		printf("Player1 position: [%f][%f] \n", player1.getPosition().x, player1.getPosition().y);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{

			if (player1.getPosition().y < windowHeight - 76.0f)
			{
				player1.move(0.0f, playerMoveDown * deltaTime.asSeconds());

			}
			else //used to move the player within the bounds of the window instead of being a slight negative value.
			{
				player1.setPosition(3.0f, windowHeight - 76.0f);
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


			if (player2.getPosition().y < windowHeight - 76.0f)
			{
				player2.move(0.0f, playerMoveDown * deltaTime.asSeconds());

			}
			else
			{
				player2.setPosition(windowWidth - 23.0f, windowHeight - 76.0f);
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
				player2.setPosition(windowWidth - 23.0f, 1.0f);
			}
		}
		//
		//lets make the paddles change the direction of the ball and increase its speed.
		//
		if (shape.getPosition().x > player1.getPosition().x && shape.getPosition().x < player1.getPosition().x + player1.getSize().x &&
			shape.getPosition().y > player1.getPosition().y && shape.getPosition().y < player1.getPosition().y + player1.getSize().y)
		{
			ballBounceSound.play();
			diagX *= -1.2f;
			player1.setFillColor(sf::Color::White);

		}
		else
		{
			player1.setFillColor(sf::Color::Red);
		}

		if (shape.getPosition().x + 27 > player2.getPosition().x && shape.getPosition().x + 27 < player2.getPosition().x + player2.getSize().x &&
			shape.getPosition().y > player2.getPosition().y && shape.getPosition().y < player2.getPosition().y + player2.getSize().y)
		{
			ballBounceSound.play();
			diagX *= -1.2f;
			player2.setFillColor(sf::Color::White);

		}
		else
		{
			player2.setFillColor(sf::Color::Blue);
		}




		//
		//lets print the window and all images in the window
		//

		window.clear();
		window.draw(shape);
		window.draw(player1);
		window.draw(player2);
		window.draw(scoreBoard);
		window.draw(winner);
		window.display();
	}

	return 0;
}
