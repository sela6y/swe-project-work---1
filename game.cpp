#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;

void updateBranches(int seed);

const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

enum class side { LEFT, RIGHT, NONE };
side branchPositions[NUM_BRANCHES];


int main()
{
	RenderWindow window(VideoMode (1920, 1080), "Timber!!!", Style::Fullscreen);

	Texture textureBackground1;
	textureBackground1.loadFromFile("graphics/background.png");
	Sprite spriteBackground1;
	spriteBackground1.setTexture(textureBackground1);
	spriteBackground1.setPosition(0, 0);

	Texture textureBackground2;
	textureBackground2.loadFromFile("graphics/background1.png");

	Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810, 0);

	Texture textureTree2;
	textureTree2.loadFromFile("graphics/tree2.png");
	Sprite spriteTree2;
	spriteTree2.setTexture(textureTree2);
	Sprite spriteTree3;
	spriteTree3.setTexture(textureTree2);
	Sprite spriteTree4;
	spriteTree4.setTexture(textureTree2);
	Sprite spriteTree5;
	spriteTree5.setTexture(textureTree2);
	Sprite spriteTree6;
	spriteTree6.setTexture(textureTree2);


	spriteTree2.setPosition(200, 0);
	spriteTree3.setPosition(450, -400);
	spriteTree4.setPosition(1200, -400);
	spriteTree5.setPosition(1400, -500);
	spriteTree6.setPosition(1600, 0);

	Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png");
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 800);

	bool beeActive = false;

	float beeSpeed = 0.0f;

	Texture textureCloud;

	textureCloud.loadFromFile("graphics/cloud.png");

	const int NUM_CLOUDS = 6;
	Sprite clouds[NUM_CLOUDS];
	int cloudSpeeds[NUM_CLOUDS];
	bool cloudsActive[NUM_CLOUDS];

	for (int i = 0; i < NUM_CLOUDS; i++)
	{
		clouds[i].setTexture(textureCloud);
		clouds[i].setPosition(-300, i * 150);
		cloudsActive[i] = false;
		cloudSpeeds[i] = 0;

	}

	Clock clock;

	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	bool paused = true;
	int score = 100, highscore = 0;
	int level = 1;

	int levelshift;

	Text messageText;
	Text scoreText;
	Text highscoreText;

	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	messageText.setFont(font);
	scoreText.setFont(font);
	highscoreText.setFont(font);


	highscoreText.setFillColor(Color::White);
	highscoreText.setCharacterSize(100);
	highscoreText.setPosition(1200, 20);

	messageText.setString(" Use left arrow to chop the tree from left side \nUse right arrow to chop the tree from right side\n            Avoid being hit by branch\n              Cut all trees in time\n              Press Enter to start!\n            Press ESC anytime to QUIT");
	scoreText.setString("Score = 0");

	messageText.setCharacterSize(50);
	scoreText.setCharacterSize(100);

	messageText.setFillColor(Color::Magenta);
	scoreText.setFillColor(Color::White);

	FloatRect textRect = messageText.getLocalBounds();

	messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

	scoreText.setPosition(20, 20);

	RectangleShape rect1;
	rect1.setFillColor(sf::Color(0, 0, 0, 150));
	rect1.setSize(Vector2f(600,105));
	rect1.setPosition(0,30);

	RectangleShape rect2;
	rect2.setFillColor(sf::Color(0, 0, 0, 150));
	rect2.setSize(Vector2f(1000, 105));
	rect2.setPosition(1150, 30);

	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");

	for (int i = 0; i < NUM_BRANCHES; i++) {
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);

		branches[i].setOrigin(220, 20);
	}

	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(580, 720);

	side playerSide = side::LEFT;

	Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(600, 860);

	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(700, 830);

	const float AXE_POSITION_LEFT = 700;
	const float AXE_POSITION_RIGHT = 1075;

	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810, 720);

	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	bool acceptInput = false;

	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopBuffer);

	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sound/death.wav");
	Sound death;
	death.setBuffer(deathBuffer);

	SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("sound/out_of_time.wav");
	Sound outOfTime;
	outOfTime.setBuffer(ootBuffer);

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{


			if (event.type == Event::KeyReleased && !paused)
			{
				acceptInput = true;

				spriteAxe.setPosition(2500, spriteAxe.getPosition().y);
			}

		}

		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Return))
		{
			paused = false;

			if (highscore < score) highscore = score;

			level = 1;
			score = 0;
			timeRemaining = 6;

			for (int i = 1; i < NUM_BRANCHES; i++)
			{
				branchPositions[i] = side::NONE;
			}

			spriteRIP.setPosition(675, 2000);

			spritePlayer.setPosition(580, 720);

			acceptInput = true;

		}

		if (acceptInput)
		{
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				playerSide = side::RIGHT;

				spriteAxe.setRotation(0);

				if(playerSide != branchPositions[4]) score++;

                switch(score){
                    case 20:
                        level++;
                        for(int i = 0; i < 6; ++i) branchPositions[i] = side::NONE;
                        break;
                    case 40:
                        level++;
                        for(int i = 0; i < 6; ++i) branchPositions[i] = side::NONE;
                        break;
                    case 60:
                        level++;
                        for(int i = 0; i < 6; ++i) branchPositions[i] = side::NONE;
                        break;
                    case 80:
                        level++;
                        for(int i = 0; i < 6; ++i) branchPositions[i] = side::NONE;
                        break;
                    case 100:
                        level++;
                        for(int i = 0; i < 6; ++i) branchPositions[i] = side::NONE;
                        break;
                    case 120:
                        level++;
                        spriteBackground1.setTexture(textureBackground2);
                        for(int i = 0; i < 6; ++i) branchPositions[i] = side::NONE;
                        break;
                    case 140:
                        level++;
                        for(int i = 0; i < 6; ++i) branchPositions[i] = side::NONE;
                        break;
                    case 160:
                        level++;
                        for(int i = 0; i < 6; ++i) branchPositions[i] = side::NONE;
                        break;
                    case 180:
                        level++;
                        for(int i = 0; i < 6; ++i) branchPositions[i] = side::NONE;
                        break;
                    case 200:
                        level++;
                        for(int i = 0; i < 6; ++i) branchPositions[i] = side::NONE;
                        break;
                    case 220:
                        level++;
                        for(int i = 0; i < 6; ++i) branchPositions[i] = side::NONE;
                        break;
                    case 240:
                        level++;
                        for(int i = 0; i < 6; ++i) branchPositions[i] = side::NONE;
                        break;
                }

				timeRemaining += (2 / score) + .15;

				switch(level){
                case 1:{
                    spriteAxe.setPosition(1075, 830);

                    spritePlayer.setPosition(1200, 720);

                    spriteLog.setPosition(810, 930);
                    break;
                    }
                case 2:{
                    spriteAxe.setPosition(255, 990);

                    spritePlayer.setPosition(380, 880);

                    spriteLog.setPosition(200, 990);
                    break;
                    }
                case 3:{
                    spriteAxe.setPosition(1455, 620);

                    spritePlayer.setPosition(1580, 508);

                    spriteLog.setPosition(1400, 620);
                    break;
                    }
                case 4:{
                    spriteAxe.setPosition(1650, 990);

                    spritePlayer.setPosition(1780, 880);

                    spriteLog.setPosition(1780, 990);
                    break;
                    }
                case 5:{
                    spriteAxe.setPosition(510, 715);

                    spritePlayer.setPosition(630, 608);

                    spriteLog.setPosition(450, 715);
                    break;
                    }
                case 6:{
                    spriteAxe.setPosition(1265, 720);

                    spritePlayer.setPosition(1390, 608);

                    spriteLog.setPosition(1200, 720);
                    break;
                    }
                case 7:{
                    spriteAxe.setPosition(1075, 830);

                    spritePlayer.setPosition(1200, 720);

                    spriteLog.setPosition(810, 930);
                    break;
                    }
                case 8:{
                    spriteAxe.setPosition(255, 990);

                    spritePlayer.setPosition(380, 880);

                    spriteLog.setPosition(200, 990);
                    break;
                    }
                case 9:{
                    spriteAxe.setPosition(1455, 620);

                    spritePlayer.setPosition(1580, 508);

                    spriteLog.setPosition(1400, 620);
                    break;
                    }
                case 10:{
                    spriteAxe.setPosition(1650, 990);

                    spritePlayer.setPosition(1780, 880);

                    spriteLog.setPosition(1780, 990);
                    break;
                    }
                case 11:{
                    spriteAxe.setPosition(510, 715);

                    spritePlayer.setPosition(630, 608);

                    spriteLog.setPosition(450, 715);
                    break;
                    }
                case 12:{
                    spriteAxe.setPosition(1265, 720);

                    spritePlayer.setPosition(1390, 608);

                    spriteLog.setPosition(1200, 720);
                    break;
                    }
				}

				updateBranches(score);

				logSpeedX = -5000;
				logActive = true;


				acceptInput = false;

				chop.play();

			}

			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				playerSide = side::LEFT;

				spriteAxe.setRotation(-180);

                if(playerSide != branchPositions[4]) score++;

                switch(score){
                    case 20:
                        level++;
                        for(int i = 0; i < 6; ++i) branchPositions[i] = side::NONE;
                        break;
                    case 40:
                        level++;
                        for(int i = 0; i < 6; ++i) branchPositions[i] = side::NONE;
                        break;
                    case 60:
                        level++;
                        for(int i = 0; i < 6; ++i) branchPositions[i] = side::NONE;
                        break;
                    case 80:
                        level++;
                        for(int i = 0; i < 6; ++i) branchPositions[i] = side::NONE;
                        break;
                    case 100:
                        level++;
                        for(int i = 0; i < 6; ++i) branchPositions[i] = side::NONE;
                        break;
                    case 120:
                        level++;
                        spriteBackground1.setTexture(textureBackground2);
                        for(int i = 0; i < 6; ++i) branchPositions[i] = side::NONE;
                        break;
                    case 140:
                        level++;
                        for(int i = 0; i < 6; ++i) branchPositions[i] = side::NONE;
                        break;
                    case 160:
                        level++;
                        for(int i = 0; i < 6; ++i) branchPositions[i] = side::NONE;
                        break;
                    case 180:
                        level++;
                        for(int i = 0; i < 6; ++i) branchPositions[i] = side::NONE;
                        break;
                    case 200:
                        level++;
                        for(int i = 0; i < 6; ++i) branchPositions[i] = side::NONE;
                        break;
                    case 220:
                        level++;
                        for(int i = 0; i < 6; ++i) branchPositions[i] = side::NONE;
                        break;
                    case 240:
                        level++;
                        for(int i = 0; i < 6; ++i) branchPositions[i] = side::NONE;
                        break;

                }

				timeRemaining += (2 / score) + .15;

				switch(level){
                case 1:{
                    spriteAxe.setPosition(852, 830);

                    spritePlayer.setPosition(580, 720);

                    spriteLog.setPosition(810, 830);
                    break;
                    }
                case 2:{
                    spriteAxe.setPosition(277, 990);

                    spritePlayer.setPosition(-5, 880);

                    spriteLog.setPosition(200, 990);
                    break;
                    }
                case 3:{
                    spriteAxe.setPosition(1502, 620);

                    spritePlayer.setPosition(1220, 508);

                    spriteLog.setPosition(1400, 620);
                    break;
                    }
                case 4:{
                    spriteAxe.setPosition(1712, 990);

                    spritePlayer.setPosition(1430, 880);

                    spriteLog.setPosition(1400, 990);
                    break;
                    }
                case 5:{
                    spriteAxe.setPosition(552, 715);

                    spritePlayer.setPosition(280, 608);

                    spriteLog.setPosition(450, 715);
                    break;
                    }
                case 6:{
                    spriteAxe.setPosition(1307, 720);

                    spritePlayer.setPosition(1030, 608);

                    spriteLog.setPosition(1200, 720);
                    break;
                    }
                case 7:{
                    spriteAxe.setPosition(852, 830);

                    spritePlayer.setPosition(580, 720);

                    spriteLog.setPosition(810, 830);
                    break;
                    }
                case 8:{
                    spriteAxe.setPosition(277, 990);

                    spritePlayer.setPosition(-5, 880);

                    spriteLog.setPosition(200, 990);
                    break;
                    }
                case 9:{
                    spriteAxe.setPosition(1502, 620);

                    spritePlayer.setPosition(1220, 508);

                    spriteLog.setPosition(1400, 620);
                    break;
                    }
                case 10:{
                    spriteAxe.setPosition(1712, 990);

                    spritePlayer.setPosition(1430, 880);

                    spriteLog.setPosition(1400, 990);
                    break;
                    }
                case 11:{
                    spriteAxe.setPosition(552, 715);

                    spritePlayer.setPosition(280, 608);

                    spriteLog.setPosition(450, 715);
                    break;
                    }
                case 12:{
                    spriteAxe.setPosition(1307, 720);

                    spritePlayer.setPosition(1030, 608);

                    spriteLog.setPosition(1200, 720);
                    break;
                    }
				}

				updateBranches(score);

				logSpeedX = 5000;
				logActive = true;


				acceptInput = false;

				chop.play();

			}


		}

		if (!paused)
		{
			Time dt = clock.restart();

			timeRemaining -= dt.asSeconds();

			timeBar.setSize(Vector2f(timeBarWidthPerSecond *
				timeRemaining, timeBarHeight));


			if (timeRemaining <= 0.0f) {

				paused = true;

				messageText.setString("Out of time!!");

				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

				outOfTime.play();


			}


			if (!beeActive)
			{

				srand((int)time(0) * 10);
				beeSpeed = (rand() % 200) + 200;

				srand((int)time(0) * 10);
				float height = (rand() % 500) + 500;
				spriteBee.setPosition(2000, height);
				beeActive = true;

			}
			else
			{

				spriteBee.setPosition(spriteBee.getPosition().x -(beeSpeed * dt.asSeconds()),spriteBee.getPosition().y);

				if (spriteBee.getPosition().x < -100)
				{
					beeActive = false;
				}
			}

			for (int i = 0; i < NUM_CLOUDS; i++)
			{
				if (!cloudsActive[i])
				{
					srand((int)time(0) * i);
					cloudSpeeds[i] = (rand() % 200);

					srand((int)time(0) * i);
					float height = (rand() % 150);
					clouds[i].setPosition(-200, height);
					cloudsActive[i] = true;

				}
				else
				{
					clouds[i].setPosition(clouds[i].getPosition().x +(cloudSpeeds[i] * dt.asSeconds()),clouds[i].getPosition().y);

					if (clouds[i].getPosition().x > 1920)
					{
						cloudsActive[i] = false;
					}

				}

			}

            std::stringstream ss;
            ss << "Score = " << score;
            scoreText.setString(ss.str());

            std::stringstream ss2;
            ss2 << "High Score " << highscore;
            highscoreText.setString(ss2.str());

			for (int i = 0; i < NUM_BRANCHES; i++)
			{

				float height = i * 150;

                if (branchPositions[i] == side::LEFT)
				{
					switch(level){
					    case 1:{
					        branches[i].setPosition(610, height);
					        break;
                            }
                        case 2:{
					        branches[i].setPosition(0, height + 180);
					        break;
                            }
                        case 3:{
					        branches[i].setPosition(1200, height - 200);
					        break;
                            }
                        case 4:{
					        branches[i].setPosition(1400, height + 180);
					        break;
                            }
                        case 5:{
					        branches[i].setPosition(250, height - 100);
					        break;
                            }
                        case 6:{
					        branches[i].setPosition(1000, height - 100);
					        break;
                            }
                    }

					branches[i].setOrigin(220, 40);
					branches[i].setRotation(180);
				}

				else if (branchPositions[i] == side::RIGHT)
				{
                    switch(level){
					    case 1:{
					        branches[i].setPosition(1330, height);
					        break;
                            }
                        case 2:{
					        branches[i].setPosition(560, height + 180);
					        break;
                            }
                        case 3:{
					        branches[i].setPosition(1760, height - 200);
					        break;
                            }
                        case 4:{
					        branches[i].setPosition(1960, height + 180);
					        break;
                            }
                        case 5:{
					        branches[i].setPosition(760, height - 100);
					        break;
                            }
                        case 6:{
					        branches[i].setPosition(1560, height - 100);
					        break;
                            }
					    case 7:{
					        branches[i].setPosition(1330, height);
					        break;
                            }
                        case 8:{
					        branches[i].setPosition(560, height + 180);
					        break;
                            }
                        case 9:{
					        branches[i].setPosition(1760, height - 200);
					        break;
                            }
                        case 10:{
					        branches[i].setPosition(1960, height + 180);
					        break;
                            }
                        case 11:{
					        branches[i].setPosition(760, height - 100);
					        break;
                            }
                        case 12:{
					        branches[i].setPosition(1560, height - 100);
					        break;
                            }
					}
                branches[i].setOrigin(220, 40);
                branches[i].setRotation(0);

				}
				else
				{
					branches[i].setPosition(3000, height);
				}
			}

			if (logActive)
			{

				spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()),spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));

				if (spriteLog.getPosition().x < -100 ||
					spriteLog.getPosition().x > 2000)
				{
					logActive = false;
					switch(level){
					    case 1: spriteLog.setPosition(810, 750);
                                break;
					    case 2: spriteLog.setScale(.5, .5);
                                spriteLog.setPosition(200, 990);
                                break;
					    case 3: spriteLog.setScale(.5, .5);
                                spriteLog.setPosition(1400, 620);
                                break;
					    case 4: spriteLog.setScale(.5, .5);
                                spriteLog.setPosition(1600, 870);
                                break;
					    case 5: spriteLog.setScale(.5, .5);
                                spriteLog.setPosition(450, 715);
                                break;
					    case 6: spriteLog.setScale(.5, .5);
                                spriteLog.setPosition(1200, 720);
                                break;
					    case 7: spriteLog.setPosition(810, 750);
                                break;
					    case 8: spriteLog.setScale(.5, .5);
                                spriteLog.setPosition(200, 990);
                                break;
					    case 9: spriteLog.setScale(.5, .5);
                                spriteLog.setPosition(1400, 620);
                                break;
					    case 10: spriteLog.setScale(.5, .5);
                                spriteLog.setPosition(1600, 870);
                                break;
					    case 11: spriteLog.setScale(.5, .5);
                                spriteLog.setPosition(450, 715);
                                break;
					    case 12: spriteLog.setScale(.5, .5);
                                spriteLog.setPosition(1200, 720);
                                break;
					}
				}
			}

			if (branchPositions[5] == playerSide)
			{
				paused = true;
				acceptInput = false;

				spriteRIP.setPosition(525, 760);

				spritePlayer.setPosition(2000, 660);

				messageText.setString("SQUISHED!!");

				FloatRect textRect = messageText.getLocalBounds();

				messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

				death.play();
			}


		}


		if(level == 13){

            paused = true;
            acceptInput = false;

            messageText.setString(" RICH MAN\nPress ESC");

		}

        window.clear();

		for (int i = 0; i < NUM_CLOUDS; i++)
		{
			window.draw(clouds[i]);
		}

        window.draw(spriteBackground1);
        switch(level){
                case 1: window.draw(spriteTree5);
                        window.draw(spriteTree3);
                        window.draw(spriteTree4);

                        for (int i = 0; i < NUM_BRANCHES; i++) {
                            window.draw(branches[i]);
                        }

                        window.draw(spritePlayer);

                        window.draw(spriteLog);

                        window.draw(spriteTree);
                        window.draw(spriteTree6);
                        window.draw(spriteTree2);
                        break;
                case 2: window.draw(spriteTree5);
                        window.draw(spriteTree3);
                        window.draw(spriteTree4);
                        window.draw(spriteTree6);

                        for (int i = 0; i < NUM_BRANCHES; i++) {
                            window.draw(branches[i]);
                        }

                        window.draw(spritePlayer);

                        window.draw(spriteLog);

                        window.draw(spriteTree2);
                        break;
                case 3: for (int i = 0; i < NUM_BRANCHES; i++) {
                            window.draw(branches[i]);
                        }

                        window.draw(spritePlayer);

                        window.draw(spriteLog);

                        window.draw(spriteTree5);
                        window.draw(spriteTree3);
                        window.draw(spriteTree4);
                        window.draw(spriteTree6);
                        break;
                case 4: window.draw(spriteTree3);
                        window.draw(spriteTree4);
                        for (int i = 0; i < NUM_BRANCHES; i++) {
                            window.draw(branches[i]);
                        }

                        window.draw(spritePlayer);

                        window.draw(spriteLog);

                        window.draw(spriteTree6);
                        break;
                case 5: for (int i = 0; i < NUM_BRANCHES; i++) {
                            window.draw(branches[i]);
                        }

                        window.draw(spritePlayer);

                        window.draw(spriteLog);

                        window.draw(spriteTree3);
                        window.draw(spriteTree4);
                        break;
                case 6: for (int i = 0; i < NUM_BRANCHES; i++) {
                            window.draw(branches[i]);
                        }

                        window.draw(spritePlayer);

                        window.draw(spriteLog);

                        window.draw(spriteTree4);
                        break;
                case 7: window.draw(spriteTree5);
                        window.draw(spriteTree3);
                        window.draw(spriteTree4);

                        for (int i = 0; i < NUM_BRANCHES; i++) {
                            window.draw(branches[i]);
                        }

                        window.draw(spritePlayer);

                        window.draw(spriteLog);

                        window.draw(spriteTree);
                        window.draw(spriteTree6);
                        window.draw(spriteTree2);
                        break;
                case 8: window.draw(spriteTree5);
                        window.draw(spriteTree3);
                        window.draw(spriteTree4);
                        window.draw(spriteTree6);

                        for (int i = 0; i < NUM_BRANCHES; i++) {
                            window.draw(branches[i]);
                        }

                        window.draw(spritePlayer);

                        window.draw(spriteLog);

                        window.draw(spriteTree2);
                        break;
                case 9: for (int i = 0; i < NUM_BRANCHES; i++) {
                            window.draw(branches[i]);
                        }

                        window.draw(spritePlayer);

                        window.draw(spriteLog);

                        window.draw(spriteTree5);
                        window.draw(spriteTree3);
                        window.draw(spriteTree4);
                        window.draw(spriteTree6);
                        break;
                case 10: window.draw(spriteTree3);
                        window.draw(spriteTree4);
                        for (int i = 0; i < NUM_BRANCHES; i++) {
                            window.draw(branches[i]);
                        }

                        window.draw(spritePlayer);

                        window.draw(spriteLog);

                        window.draw(spriteTree6);
                        break;
                case 11: for (int i = 0; i < NUM_BRANCHES; i++) {
                            window.draw(branches[i]);
                        }

                        window.draw(spritePlayer);

                        window.draw(spriteLog);

                        window.draw(spriteTree3);
                        window.draw(spriteTree4);
                        break;
                case 12: for (int i = 0; i < NUM_BRANCHES; i++) {
                            window.draw(branches[i]);
                        }

                        window.draw(spritePlayer);

                        window.draw(spriteLog);

                        window.draw(spriteTree4);
                        break;
            }


		if (paused)
		{
			window.draw(messageText);
		}

		else{

        window.draw(spriteAxe);

		window.draw(spriteRIP);

		window.draw(rect1);
		window.draw(rect2);

		window.draw(spriteBee);

		window.draw(scoreText);

		window.draw(highscoreText);

		window.draw(timeBar);

		}

		window.display();

	}
}

void updateBranches(int seed)
{
	for (int j = NUM_BRANCHES - 1; j > 0; j--) {
		branchPositions[j] = branchPositions[j - 1];
	}

	if((seed % 20) > 14) branchPositions[0] = side::NONE;
	else{
        srand((int)time(0) + seed);
        int r = (rand() % 5);

        switch (r) {
        case 0:
            branchPositions[0] = side::LEFT;
            break;

        case 1:
            branchPositions[0] = side::RIGHT;
            break;

        default:
            branchPositions[0] = side::NONE;
            break;
        }
	}
}

































