#include <SFML/Graphics.hpp>
#include "../header/methods.h"
#include <chrono>
#include <thread>
#include "../header/LinMat.h"


int main()
{
	double winSize = 800;//pixels
	double worldSize = 100;//meters
	double scale = winSize / worldSize;

	int nParticles = 5000;
	double particleMass = 1;
	double attraction = 1;// TODO : IMPLEMENT
	double boxSizeX = 100;
	double boxSizeY = 100;

	double xPosMin = -42;
	double xPosMax = 42;
	double yPosMin = -42;
	double yPosMax = 42;

	double biasVelX = 0;
	double biasVelY = 0;
	double absMaxRandVel =0;

	LinMat pos(nParticles, 2);
	LinMat vel(nParticles, 2);
	createInitPos(pos, xPosMin, xPosMax, yPosMin, yPosMax);
	createInitVel(vel, biasVelX, biasVelY, absMaxRandVel);

	int* sortedX = new int[nParticles];
	int* sortedY = new int[nParticles];
	bool* collided = new bool[nParticles];
	for (int i = 0; i < nParticles; i++) {
		sortedX[i] = i;
		sortedY[i] = i;
	}


	double radius = 0.6;

	double fps = 50;
	double dt = 1 / fps;
	double t = 0;

	std::chrono::system_clock::time_point start, stop;
	std::chrono::duration<double> time_span;
	double diff;
	int margin;

	sf::RenderWindow window(sf::VideoMode(winSize, winSize), "MolecularCFD");
	sf::CircleShape particle(radius*scale, 10);
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

	int k;
	std::vector<double> test;
	for (int i = 0; i < nParticles; i++) {
		test.push_back(0);
	}
	
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

		insertion_sort(pos, sortedX, 0);
		insertion_sort(pos, sortedY, 1);
		for (int i = 0; i < nParticles; i++) {
			collided[i] = false;
			test[i] = pos.get(pos.index(sortedX[i], 0));
		}

		//resolve collisions
		resolveCollisions(pos, vel, collided, sortedX, radius);
		k = 0;
		while (k < nParticles && pos.get(pos.index(sortedX[k], 0)) < (-(boxSizeX) / 2 + radius)){
		if (!collided[sortedX[k]]) {
			vel.set(pos.index(sortedX[k], 0), -0.95*vel.get(pos.index(sortedX[k], 0)));
			pos.set(pos.index(sortedX[k], 0), -(boxSizeX) / 2 + radius);
			collided[sortedX[k]] = true;

		}
		k++;
	}

		k = nParticles - 1;
		while (k != 0 && pos.get(pos.index(sortedX[k], 0)) > ((boxSizeX) / 2 - radius)){
			if (!collided[sortedX[k]]) {
				vel.set(pos.index(sortedX[k], 0), -0.95*vel.get(pos.index(sortedX[k], 0)));
				pos.set(pos.index(sortedX[k], 0), (boxSizeX)/2 - radius);
				collided[sortedX[k]] = true;

			}
			k--;
		}
		

		k = 0;
		while (k < nParticles && pos.get(pos.index(sortedY[k], 1)) < (-(boxSizeY) / 2 + radius)){
			if (!collided[sortedY[k]]) {
				vel.set(pos.index(sortedY[k], 1), -0.95*vel.get(pos.index(sortedY[k], 1)));
				pos.set(pos.index(sortedY[k], 1), -(boxSizeY) / 2 + radius);
				collided[sortedY[k]] = true;

			}
			k++;
		}
		k = nParticles-1;
		while (k != 0 && pos.get(pos.index(sortedY[k], 1)) > ((boxSizeY) / 2 - radius)){
			if (!collided[sortedY[k]]) {
				vel.set(pos.index(sortedY[k], 1), -0.95*vel.get(pos.index(sortedY[k], 1)));
				pos.set(pos.index(sortedY[k], 1), (boxSizeY) / 2 - radius);
				collided[sortedY[k]] = true;

			}
			k--;
		}

		int ind;

		for (int i = 0; i < nParticles; i++) {
			ind = pos.index(i, 0);
			vel.set(ind + 1, vel.get(ind + 1) - 98.1*dt);
			vel.set(ind, vel.get(ind));
			pos.set(ind, pos.get(ind) + dt*vel.get(ind));
			pos.set(ind+1, pos.get(ind+1) + dt*vel.get(ind+1));
		}

		//draw shapes
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
		t+=dt;
	}

	return 0;
}