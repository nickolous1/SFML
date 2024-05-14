#include <SFML/Graphics.hpp>
#include <iostream>
#include <Windows.h>
using namespace std;

/*

1. create a scoreboard in the middle top of game
2.striker that can bounce the ball

for puck:
need to get x,y
make collisions with velocity (speed and direction)
freely move with friction
bouncing off walls in proper direction

*/

const float PI = 3.14159265f;


class Puck {
public:
	sf::CircleShape puckValue;
	float puckSpeed;
	float puckDirection;

	Puck(float speed, float direction, float radius, float x, float y) {
		puckValue.setRadius(radius);
		puckValue.setPosition(x, y);
		puckSpeed = speed;
		puckDirection = direction;
		puckValue.setFillColor(sf::Color::Yellow);
	}

	void move() {
		float deltaX = puckSpeed * cos(puckDirection);
		float deltaY = puckSpeed * sin(puckDirection);
		puckValue.move(deltaX, deltaY);
	}

	void collision(float boardWidth, float boardHeight) {
		float puckX = puckValue.getPosition().x;
		float puckY = puckValue.getPosition().y;
		
		if (puckX  > boardWidth - 60 || (puckX - 20) < 0) {
			puckDirection = PI - puckDirection;  
			puckSpeed += 0.5;
		}

		if (puckY  > boardHeight - 60 || puckY - 10 < 0 ) {
			puckDirection = -(puckDirection);
			puckSpeed += 0.5;
		}
	}

	void friction() {
		if (puckSpeed > 0) {
			puckSpeed -= 0.1;
		}
		
	}

	void strikerCollision(sf::CircleShape striker) {
		float strikerx = striker.getPosition().x;
		float strikery = striker.getPosition().y;
		float puckx = puckValue.getPosition().x;
		float pucky = puckValue.getPosition().y;
		float puckRadius = 30.0;
		float strikerRadius = 40.0;

		float deltax = puckx - strikerx;
		float deltay = pucky - strikery;

		float distance = sqrt(deltax * deltax + deltay * deltay);

		if (distance < 70) { //70 is total radius
			float angle = atan2(deltay, deltax);
			float collisionAngle = angle - puckDirection;
			collisionAngle = collisionAngle + PI / 2.0;
			puckDirection = angle - collisionAngle;
			puckSpeed = 30.0;
			float moveDistance = 70.0 - distance;
			puckValue.move(moveDistance * cos(angle), moveDistance * sin(angle));
		}

	}

	bool goalCheck(sf::RectangleShape goal) {
		float goalx = goal.getPosition().x;
		float goaly = goal.getPosition().y;
		float puckx = puckValue.getPosition().x;
		float pucky = puckValue.getPosition().y;
		float puckDiameter = 60.0;
		float goalWidth = 50.0;

		float deltax = puckx - goalx;
		float deltay = pucky - goaly;

		float distance = sqrt(deltax * deltax + deltay * deltay);

		if (distance < 110) {
			puckValue.setPosition(805, 300);
			puckSpeed = 0;
			return true;
		}
		else {
			return false;
		}


	}

};
	
	
int main() {

	int pointsR = 0;
	int pointsB = 0;

	sf::Font font;
	font.loadFromFile("Minecraft.ttf");
	string text = "Scores(Red - blue) : " + to_string(pointsR) + " " + to_string(pointsB);
	string winRed = "Red wins with 5 points!";
	string winBlue = "Blue wins with 5 points!";
	sf::Text Scores;
	Scores.setFont(font);
	Scores.setString(text);
	Scores.setCharacterSize(28);
	Scores.setFillColor(sf::Color::Black);
	Scores.setPosition(0.0f, 10.0f);
	sf::Text winR;
	winR.setFont(font);
	winR.setString(winRed);
	winR.setCharacterSize(50);
	winR.setFillColor(sf::Color::Red);
	winR.setPosition(570.0f, 300.0f);
	sf::Text winB;
	winB.setFont(font);
	winB.setString(winBlue);
	winB.setCharacterSize(50);
	winB.setFillColor(sf::Color::Blue);
	winB.setPosition(570.0f, 300.0f);
	sf::RenderWindow window(sf::VideoMode(1700, 700), "Game", sf::Style::Close | sf::Style::Titlebar);
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);

	sf::RectangleShape windowTexture(sf::Vector2f(1700.0f, 700.0f));
	sf::Texture texture3;
	texture3.loadFromFile("HockeyBoard.png");
	windowTexture.setTexture(&texture3);

	sf::RectangleShape leftGoal(sf::Vector2f(50.0f, 230.0f));
	leftGoal.setOrigin(25.0f, 115.0f);
	sf::RectangleShape rightGoal(sf::Vector2f(50.0f, 230.0f));
	rightGoal.setOrigin(25.0f, 115.0f);
	leftGoal.setPosition(0.0f, 220.0f);
	rightGoal.setPosition(1650.0f, 220.0f);
	//leftGoal.setFillColor(sf::Color::Red);
	//rightGoal.setFillColor(sf::Color::Red);

	sf::CircleShape strikerR(40.0f);
	strikerR.setPosition(250.0f, 350.0f);
	strikerR.setOrigin(20.0f, 20.0f);
	//block.setFillColor(sf::Color::Red);
	sf::Texture texture;
	texture.loadFromFile("red striker.jpg");
	strikerR.setTexture(&texture);

	sf::CircleShape strikerB(40.0f);
	strikerB.setPosition(1250.0f, 350.0f);
	strikerB.setOrigin(20.0f, 20.0f);
	//block.setFillColor(sf::Color::Red);
	sf::Texture texture2;
	texture2.loadFromFile("blue striker.jfif");
	strikerB.setTexture(&texture2);

	sf::RectangleShape startScreen(sf::Vector2f(1700.0f, 700.0f));
	//strikerB.setPosition(1250.0f, 350.0f);
	//strikerB.setOrigin(20.0f, 20.0f);
	//block.setFillColor(sf::Color::Red);
	sf::Texture texture4;
	texture4.loadFromFile("Untitled.png");
	startScreen.setTexture(&texture4);

	sf::RectangleShape endScreen(sf::Vector2f(1700.0f, 700.0f));
	sf::Texture texture5;
	texture5.loadFromFile("end.png");
	endScreen.setTexture(&texture5);

	Puck puck(0.0, PI / 4, 30.0, 805, 300);

	int screenFlag = 0;



	while (window.isOpen()) {
		sf::Event run;

		while (window.pollEvent(run)) {
			switch (run.type) {

			case sf::Event::Closed:
				window.close();
				break;
			}

		}

		string text = "  Scores  (Red - blue):  " + to_string(pointsR) + "  -  " + to_string(pointsB);
		Scores.setString(text);
		sf::Vector2f Rpos = strikerR.getPosition();
		sf::Vector2f Bpos = strikerB.getPosition();


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && Rpos.y > 20) {
			strikerR.move(0.0f, -10.0f);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && Rpos.y < 640) {
			strikerR.move(0.0f, 10.0f);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) && Rpos.x > 20) {
			strikerR.move(-10.0f, 0.0f);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) && Rpos.x < 770) {
			strikerR.move(10.0f, 0.0f);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && Bpos.y > 20) {
			strikerB.move(0.0f, -10.0f);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && Bpos.y < 640) {
			strikerB.move(0.0f, 10.0f);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && Bpos.x > 865) {
			strikerB.move(-10.0f, 0.0f);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && Bpos.x < 1640) {
			strikerB.move(10.0f, 0.0f);
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			//sf::Vector2i mousePos = sf::Mouse::getPosition(window);
			//strikerR.setPosition((float)mousePos.x, (float)mousePos.y);
			std::cout << Bpos.x << " " << Bpos.y << "\n" << Rpos.x << " " << Rpos.y << std::endl;
		}

		puck.collision(1700, 700);
		puck.friction();
		puck.move();
		puck.strikerCollision(strikerR);
		puck.strikerCollision(strikerB);
		if (puck.goalCheck(leftGoal)) {
			pointsB += 1;
			cout << pointsR << endl;
		}
		if(puck.goalCheck(rightGoal)){
			pointsR += 1;
			cout << pointsB << endl;
		}
		window.clear();
		if (screenFlag == 0) {
			window.draw(startScreen);
		}
		else if (screenFlag == 1) {
			window.draw(windowTexture);
			//window.draw(leftGoal);
			//window.draw(rightGoal);
			window.draw(Scores);
			window.draw(puck.puckValue);
			window.draw(strikerR);
			window.draw(strikerB);
			if (pointsR == 5) {
				window.draw(winR);
				window.display();
				sf::sleep(sf::milliseconds(10000));
				screenFlag = 2;
				//window.close();
			}
			if (pointsB == 5) {
				window.draw(winB);
				window.display();
				sf::sleep(sf::milliseconds(10000));
				screenFlag = 2;
				//window.close();
			}
		}
		else {
			window.draw(endScreen);
		}

		window.display(); //needs to be last line
	

	}


	return 0;
}