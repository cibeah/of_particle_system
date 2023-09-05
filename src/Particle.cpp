#include "ofMain.h"
#include "Particle.h"
#include "glm/glm.hpp"

using namespace glm;

Particle::Particle(float init_x, float init_y, float size = 20.0, vec3 color = vec3(255.0, 255.0, 255.0)) : m_location(init_x, init_y), m_acceleration(0, 0), m_size(size), m_color(color) {
	m_velocity = vec2(0.0, 0.0);
	m_mass = pow(m_size, 3.0) / 10000;
	m_lifespan = 1024;
	m_environment_width = ofGetWidth();
	m_environment_height = ofGetHeight();
	m_num_neighbors = 0;
	m_max_neighbors = 1000;
	m_neighbors.resize(m_max_neighbors);
	m_beta = 0.1;
	update_norm();
};

Particle::Particle() {
	vec3 color = vec3(ofRandom(255), ofRandom(255), ofRandom(255));
	m_location = vec2(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
	m_acceleration = vec2(0, 0);
	m_size = 10;
	m_color = color;
	//m_velocity = vec2(0.0, 0.0);
	m_velocity = vec2(ofRandom(20) - 10, ofRandom(20) - 10);
	m_mass = pow(m_size, 3.0) / 10000;
	m_lifespan = 1024;
	m_environment_width = ofGetWidth();
	m_environment_height = ofGetHeight();
	m_num_neighbors = 0;
	m_max_neighbors = 1000;
	m_neighbors.resize(m_max_neighbors);
	m_beta = 0.1;
	update_norm();
};

glm::vec2 Particle::get_location() {
	return m_location;
}

glm::vec2 Particle::get_velocity() {
	return m_velocity;
}

glm::vec3 Particle::get_color() {
	return m_color;
}

float Particle::get_mass() {
	return m_mass;
}

float Particle::get_color_norm() {
	return m_color_norm;
}

void Particle::update_norm() {
	m_color_norm = sqrt(pow(m_color[0], 2.0) + pow(m_color[1], 2.0) + pow(m_color[2], 2.0));
}

void Particle::display() {
	//ofSetColor(m_color);
	ofSetColor(m_color[0], m_color[1], m_color[2]);
	ofDrawCircle(m_location, m_size);
	//ofLog(OF_LOG_NOTICE, "DISPLAY: .....Drawn at location " + ofToString(m_location));
}

void Particle::step() {
	//m_velocity += m_acceleration;

	// Adapt to neighbors

	vec3 avgColor = vec3(0.0, 0.0, 0.0);
	vec3 cosineSimilarityVec;
	float cosineSimilarity = 0.0;
	int num_attractors = 0;
	int num_repulsors = 0;
	vec2 avgLocation = vec2(0.0, 0.0);

	vec2 avgAttractorLocation = vec2(0.0, 0.0);
	vec2 avgRepulsorLocation = vec2(0.0, 0.0);

	vec2 avgAttractorVelocity = vec2(0.0, 0.0);
	vec2 avgRepulsorVelocity = vec2(0.0, 0.0);


	//ofLog(OF_LOG_NOTICE, "STEP: Working through: " + ofToString(m_num_neighbors) + " neighbors");
	Particle** p = m_neighbors.data();
	for (int i = 0; i < m_num_neighbors; i++) {
		vec3 neighborColor = (**p).get_color();
		float neighborNorm = (**p).get_color_norm();
		vec2 neighborLocation = (**p).get_location();
		vec2 neighborVelocity = (**p).get_velocity();


		cosineSimilarityVec = neighborColor * m_color;
		cosineSimilarity = cosineSimilarityVec[0] + cosineSimilarityVec[1] + cosineSimilarityVec[2];
		cosineSimilarity = cosineSimilarity / neighborNorm / m_color_norm;

		/*if (i == 0) {
			ofLog(OF_LOG_NOTICE, "STEP: Cosine similarity between " + ofToString(neighborColor) + " : " + ofToString(cosineSimilarity));
			ofLog(OF_LOG_NOTICE, "STEP: and " + ofToString(m_color) + " : " + ofToString(cosineSimilarity));	
			
			ofLog(OF_LOG_NOTICE, "STEP: My and Neighbor norm: " + ofToString(m_color_norm) + " : " + ofToString(neighborNorm));
		}*/

		if ((cosineSimilarity > 0.5) & (cosineSimilarity < 0.95)) {
			avgAttractorLocation += neighborLocation;
			avgAttractorVelocity += neighborVelocity;
			num_attractors++;
		}
		else {
			avgRepulsorLocation += neighborLocation;
			avgRepulsorVelocity += neighborVelocity;
			num_repulsors++;
		}

		avgColor += neighborColor;
		avgLocation += neighborLocation;
		p++;
	}

	avgColor = avgColor / (float)m_num_neighbors;
	//m_color = (1 - m_beta/10) * m_color + m_beta/10 * avgColor;
	update_norm();


	// Random Velocity
	//m_velocity = vec2(ofRandom(10)-5, ofRandom(10)-5);

	// Velocity in direction of neighbors
	avgLocation = avgLocation / (float)m_num_neighbors;

	if (num_repulsors > 0) {
		avgRepulsorLocation = avgRepulsorLocation / (float)num_repulsors;
		avgRepulsorVelocity = avgRepulsorVelocity / (float)num_repulsors;
	} 
	if (num_attractors > 0) {
		avgAttractorLocation = avgAttractorLocation / (float)num_attractors;
		avgAttractorVelocity = avgAttractorVelocity / (float)num_attractors;
	}
	float attract_strength = (float)num_attractors / (float)m_num_neighbors;

	/*ofLog(OF_LOG_NOTICE, "STEP: ........Attract Strength: " + ofToString(attract_strength));
	ofLog(OF_LOG_NOTICE, "STEP: ........Average Location of repulsors: " + ofToString(avgRepulsorLocation));
	ofLog(OF_LOG_NOTICE, "STEP: ........Average Location of attractors: " + ofToString(avgAttractorLocation));*/
	/*vec2 resultLocation = attract_strength * avgAttractorLocation - (1 - attract_strength) * avgRepulsorLocation;*/
	vec2 resultLocation = avgAttractorLocation;

	/*vec2 attractVelocity = avgAttractorLocation - m_location;
	vec2 repulseVelocity = - (avgRepulsorLocation - m_location);
	vec2 resultVelocity = attract_strength * attractVelocity + (1 - attract_strength) * repulseVelocity;*/


	vec2 resultVelocity = attract_strength * avgAttractorVelocity - (1 - attract_strength) * avgRepulsorVelocity;

	/*ofLog(OF_LOG_NOTICE, "STEP: ........Number of attractors: " + ofToString(num_attractors) + " / " + ofToString(m_num_neighbors));
	ofLog(OF_LOG_NOTICE, "STEP: ........Result location: " + ofToString(m_location + resultVelocity));*/

	/*
	ofLog(OF_LOG_NOTICE, "STEP: ........Average Location of neighors: " + ofToString(avgLocation));*/

	/*ofLog(OF_LOG_NOTICE, "STEP: Number of repulsors: " + ofToString(num_repulsors) + " / " + ofToString(m_num_neighbors));*/

	//m_velocity = (1 - m_beta) * m_velocity + m_beta * resultVelocity;
	ofLog(OF_LOG_NOTICE, "STEP: ........Average Velocity of attractors: " + ofToString(avgAttractorVelocity));
	m_velocity = (1 - m_beta) * m_velocity + m_beta * avgAttractorVelocity;

	//m_velocity = (1 - m_beta) * m_velocity + m_beta * (avgLocation - m_location);
	m_location += m_velocity;




	if (m_location[0] + m_size*2 > m_environment_width) {
		m_location[0] = m_environment_width - m_size;
		m_velocity[0] *= -1;
	}
	if (m_location[0] - m_size*2 < 0) {
		m_location[0] = m_size;
		m_velocity[0] *= -1;
	}
	if (m_location[1] + m_size*2 > m_environment_height) {
		m_location[1] = m_environment_height - m_size;
		m_velocity[1] *= -1;
	}
	if (m_location[1] - m_size*2 < 0) {
		m_location[1] = m_size;
		m_velocity[1] *= -1;
	}

	m_acceleration = vec2(0, 0);
	//m_lifespan--;
}

void Particle::grow(int growth) {
	m_size += growth;
	m_lifespan--;
	//ofLog(OF_LOG_NOTICE, "Element, size: " + ofToString(m_size) + " Lifespan :" + ofToString(m_lifespan));
}

bool Particle::isDead() {
	return m_lifespan < 0;
}

void Particle::test(list<Particle>& elements, float radius) {
	int i = 0;
	for (list<Particle>::iterator it = elements.begin(); it != elements.end(); it++)
	{
		ofLog(OF_LOG_NOTICE, "TEST..........address of neighbor " + ofToString(i + 1) + " : " + ofToString(&(*it)));
		i++;
	}
};

void Particle::get_neighbors(list<Particle>& elements, float radius) {
	float sq_radius = pow(radius, 2.0);
	float closest = 100000;
	float sq_distance;
	Particle** pointer = m_neighbors.data();
	m_num_neighbors = 0;
	int i = 0;

	for (list<Particle>::iterator it = elements.begin(); it != elements.end(); it++)
	{
		i++;
		vec2 el_location = (*it).get_location();
		sq_distance = pow(m_location[0] - el_location[0], 2.0) + pow(m_location[1] - el_location[1], 2.0);

		if (sq_distance < sq_radius) {
			*pointer = &(*it);
			pointer++;
			m_num_neighbors++;
		}
	};
};