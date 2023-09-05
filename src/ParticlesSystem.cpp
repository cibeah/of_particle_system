#include <list>
#include "glm/glm.hpp"

#include "Particle.h"
#include "ParticlesSystem.h"
#include "ofMain.h"

using namespace glm;

ParticlesSystem::ParticlesSystem() {
	m_elements = std::list<Particle>(1, Particle());
};

ParticlesSystem::ParticlesSystem(int number){
	//m_elements = std::list<Particle>(number, Particle());
	m_elements = std::list<Particle>(number);
};

void ParticlesSystem::display() {
	//ofLog(OF_LOG_NOTICE, "Displaying " + ofToString(m_elements.size()) + " elements");
	for (list<Particle>::iterator it = m_elements.begin(); it != m_elements.end(); ++it)
	{
		Particle p = *it;
		p.display();
	};
};

void ParticlesSystem::add(glm::vec3 color) {
	m_elements.push_back(Particle((int) ofGetWidth() / 2, (int) ofGetHeight() / 2, 5, color));
};

void ParticlesSystem::print_addresses() {
	int i = 0;
	for (list<Particle>::iterator it = m_elements.begin(); it != m_elements.end(); it++) {
		ofLog(OF_LOG_NOTICE, " Element " + ofToString(i + 1) + " @ " + ofToString(&(*it)));
		i++;
	}
	
};

void ParticlesSystem::update(int radius = 15) {
	for (list<Particle>::iterator it = m_elements.begin(); it != m_elements.end();)
	{
		//Particle p = *it;
		if ((*it).isDead()) {
			ofLog(OF_LOG_NOTICE, "Removing 1 element ");
			it = m_elements.erase(it);
		}
		else {
			(*it).get_neighbors(m_elements, radius);
			//(*it).test(m_elements, radius);
			it++;
		}
	};
	for (list<Particle>::iterator it = m_elements.begin(); it != m_elements.end();)
	{
		(*it).step();
		//ofLog(OF_LOG_NOTICE, "Growing 1 element ");
		it++;
	};
};