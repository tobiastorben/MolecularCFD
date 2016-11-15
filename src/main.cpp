#include <SFML/Graphics.hpp>
#include "../header/methods.h"
#include <chrono>
#include <thread>
#include "../header/LinMat.h"


int main()
{

	//-----INPUT SECTION-----//
	double winSize = 800;//pixels
	double worldSize = 10;//meters
	double fps = 1500;
	int nVertices = 10;

	double radius = 0.12;
	int nParticles = 500;
	double particleMass = 1;
	double attraction = 1;// TODO : IMPLEMENT
	
	double boxSizeX = 9;
	double boxSizeY = 9;

	double xPosMin = -4;
	double xPosMax = 4;
	double yPosMin = -4;
	double yPosMax = 4;

	double biasVelX = 0;
	double biasVelY = 0;
	double absMaxRandVel = 1;
	//-----END OF INPUT SECTION-----//

	//Pixels per meter
	double scale = winSize / worldSize;

	//Matrix of particle positions and velocities
	LinMat pos(nParticles, 2);
	LinMat vel(nParticles, 2);

	//Generete initial states for particles
	createInitPos(pos, xPosMin, xPosMax, yPosMin, yPosMax);
	createInitVel(vel, biasVelX, biasVelY, absMaxRandVel);

	int* sortedX = new int[nParticles];
	int* sortedY = new int[nParticles];
	bool* collided = new bool[nParticles];
	for (int i = 0; i < nParticles; i++) {
		sortedX[i] = i;
		sortedY[i] = i;
	}

	double dt = 1 / fps;
	double diff;
	int margin;
	std::chrono::system_clock::time_point start, stop;
	std::chrono::duration<double> time_span;


	sf::RenderWindow window(sf::VideoMode(winSize, winSize), "MolecularCFD");
	sf::CircleShape particle(radius*scale, nVertices);
	particle.setFillColor(sf::Color::Blue);
	sf::Vertex line1[] =
	{
		sf::Vertex(sf::Vector2f(scale*(-boxSizeX) / 2 + winSize / 2, -scale*boxSizeY/2 + winSize / 2)),
		sf::Vertex(sf::Vector2f(scale*(-boxSizeX) / 2 + winSize / 2, -scale*(-boxSizeY) / 2 + winSize / 2))
	};
	sf::Vertex line2[] =
	{
		sf::Vertex(sf::Vector2f(scale*(-boxSizeX) / 2 + winSize / 2, -scale*boxSizeY / 2 + winSize / 2)),
		sf::Vertex(sf::Vector2f(scale*(boxSizeX) / 2 + winSize / 2, -scale*(boxSizeY) / 2 + winSize / 2))
	};
	sf::Vertex line3[] =
	{
		sf::Vertex(sf::Vector2f(scale*(boxSizeX) / 2 + winSize / 2, -scale*(boxSizeY) / 2 + winSize / 2)),
		sf::Vertex(sf::Vector2f(scale*(boxSizeX) / 2 + winSize / 2, -scale*(-boxSizeY) / 2 + winSize / 2))
	};
	sf::Vertex line4[] =
	{
		sf::Vertex(sf::Vector2f(scale*(boxSizeX) / 2 + winSize / 2, -scale*(-boxSizeY) / 2 + winSize / 2)),
		sf::Vertex(sf::Vector2f(scale*(-boxSizeX) / 2 + winSize / 2, -scale*(-boxSizeY) / 2 + winSize / 2))
	};
	sf::Vertex xAxis[] =
	{
		sf::Vertex(sf::Vector2f(-50+winSize/2, winSize/2)),
		sf::Vertex(sf::Vector2f(50+winSize/2,winSize/2))
	};
	sf::Vertex yAxis[] =
	{
		sf::Vertex(sf::Vector2f( winSize / 2,-50 +winSize / 2)),
		sf::Vertex(sf::Vector2f(winSize / 2, 50 +winSize / 2))
	};

	//TEMP FOR DEBUG PURPOSES
	std::vector<double> test;
	for (int i = 0; i < nParticles; i++) {
		test.push_back(0);
	}
	
	//MAIN LOOP
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		start = std::chrono::high_resolution_clock::now();
		window.clear();

		//Sort after position
		insertion_sort(pos, sortedX, 0);
		insertion_sort(pos, sortedY, 1);

		//Reset collided array
		for (int i = 0; i < nParticles; i++) {
			collided[i] = false;
		}

		//resolve collisions
		interMolecularCollisions(pos, vel, collided, sortedX, radius);
		boundaryCollisions(pos, vel, boxSizeX, boxSizeY, radius, sortedX, sortedY, collided);

		//State integration
		integrateStates(pos, vel, dt);

		//Rendering
		/*window.draw(line1, 2, sf::Lines);
		window.draw(line2, 2, sf::Lines);
		window.draw(line3, 2, sf::Lines);
		window.draw(line4, 2, sf::Lines);*/
		window.draw(xAxis, 2, sf::Lines);
		window.draw(yAxis, 2, sf::Lines);
		for (int i = 0; i < nParticles; i++) {
			particle.setPosition(scale*(pos.get((pos.index(i,0)))-radius)+winSize/2, -scale*(pos.get((pos.index(i,1)))+radius)+winSize/2);
			window.draw(particle);
		}
		window.display();

		stop = std::chrono::high_resolution_clock::now();
		time_span = std::chrono::duration_cast<std::chrono::duration<double>>(stop - start);
		diff = time_span.count();
		if (diff < dt) {
			margin = 1e9*(dt - diff);
			std::this_thread::sleep_for(std::chrono::nanoseconds(margin));
		}
	}

	return 0;
}