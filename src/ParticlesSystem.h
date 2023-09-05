#pragma once
#include "ofMain.h"
#include "Particle.h"
#include <list>


class ParticlesSystem
{
public:
	ParticlesSystem();
	ParticlesSystem(int number);
	void add(glm::vec3 color);
	void update(int radius);
	void display();
	void print_addresses();

private:
	std::list<Particle> m_elements;
	std::list<Particle>::iterator it;
};

