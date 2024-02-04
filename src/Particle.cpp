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
	vec3 color = vec3(ofRandom(255), ofRandom(255), ofRandom(255)); // vec3(0,0, ofRandom(255)); //
	m_location = vec2(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
	m_acceleration = vec2(0, 0);
	m_size = 5;
	m_color = color;
	//m_velocity = vec2(0.0, 0.0);
	m_velocity = vec2(ofRandom(2) - 1, ofRandom(2) - 1);
	m_velocity[0] = m_velocity[0] * 50;
	m_velocity[1] = m_velocity[1] * 50;
	m_mass = pow(m_size, 3.0) / 10000;
	m_lifespan = 1024;
	m_environment_width = ofGetWidth();
	m_environment_height = ofGetHeight();
	m_num_neighbors = 0;
	m_max_neighbors = 1000;
	m_neighbors.resize(m_max_neighbors);
	m_beta = 1.0;
	update_norm();

	m_new_location = m_location;
	m_new_velocity = m_velocity;
	m_new_color = m_color;

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
	float cosineScore = 0.0;
	int num_attractors = 0;
	int num_repulsors = 0;
	vec2 avgLocation = vec2(0.0, 0.0);
	vec2 avgVelocity = vec2(0.0, 0.0);

	vec2 avgAttractorLocation = vec2(0.0, 0.0);
	vec2 avgRepulsorLocation = vec2(0.0, 0.0);

	vec2 avgAttractorVelocity = vec2(0.0, 0.0);
	vec2 avgRepulsorVelocity = vec2(0.0, 0.0);

	vec2 resultVelocity = m_velocity;
	vec2 resultLocation = m_location;
	vec3 resultColor = m_color;


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
		cosineScore += cosineSimilarity;

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
		avgVelocity += neighborVelocity;
		p++;
	}

	avgColor = avgColor / (float)m_num_neighbors;
	avgVelocity = avgVelocity / (float)m_num_neighbors;
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


	// Logs
	/*ofLog(OF_LOG_NOTICE, "STEP: ........Attract Strength: " + ofToString(attract_strength));
	ofLog(OF_LOG_NOTICE, "STEP: ........Average Location of repulsors: " + ofToString(avgRepulsorLocation));
	ofLog(OF_LOG_NOTICE, "STEP: ........Average Location of attractors: " + ofToString(avgAttractorLocation));
    ofLog(OF_LOG_NOTICE, "STEP: ........Cosine score: " + ofToString(cosineScore));
	ofLog(OF_LOG_NOTICE, "STEP: ........Number of attractors: " + ofToString(num_attractors) + " / " + ofToString(m_num_neighbors));
	ofLog(OF_LOG_NOTICE, "STEP: ........Average Velocity of attractors: " + ofToString(avgAttractorVelocity));*/
	
	
	// Weighted attraction&repulsion average location
	/*vec2 attractVelocity = avgAttractorLocation - m_location;
	vec2 repulseVelocity = - avgRepulsorLocation - m_location;
	resultVelocity = attract_strength * attractVelocity + (1 - attract_strength) * repulseVelocity;*/

	// Weighted attraction&repulsion average velocity
	//resultVelocity = attract_strength * avgAttractorVelocity - (1 - attract_strength) * avgRepulsorVelocity;


	// Discrete attraction or repulsion winning majority velocity
	/*if (num_repulsors > num_attractors) {
		resultVelocity = avgRepulsorVelocity;
	}
	else {
		resultVelocity = avgAttractorVelocity;
	}*/

	// Average location with cosine score activation
	/*if ((cosineScore > 3) & (cosineScore < 30)) {
		resultVelocity = avgLocation - m_location;
	}
	else {
		resultVelocity = -(avgLocation - m_location);
	}*/

	// Average velocity & color with cosine score activation
	if ((cosineScore > 3) & (cosineScore < 30)) {
		resultVelocity = avgVelocity;
		resultColor = m_color + vec3(1.0, 1.0, 1.0)*20.;
		//resultColor = avgColor + vec3(1.0, 1.0, 1.0) * 20;
		for (int i = 0; i < 3; ++i) {
			resultVelocity[i] += sign(resultVelocity[i]) * 20;
		}
	}
	else {
		resultVelocity = -(avgVelocity);
		resultColor = m_color - vec3(1.0, 1.0, 1.0)*20.;
		//resultColor = vec3(1.0, 1.0, 1.0) * 255 - avgColor - vec3(1.0, 1.0, 1.0) * 20;
		for (int i = 0; i < 3; ++i) {
			resultVelocity[i] -= sign(resultVelocity[i]) * 20;
		}
		
	}

	// Store new properties
	m_new_velocity = (1 - m_beta) * m_velocity + m_beta * resultVelocity;
	m_new_location += m_new_velocity;
	m_new_color = resultColor;
	//m_new_color = (1 - m_beta / 10) * m_color + m_beta / 10 * resultColor;


	// Correct new location to handle behavior at boundaries 
	if (m_location[0] + m_size > m_environment_width) {
		m_new_location[0] = 0;
		//m_velocity[0] *= -1;
	}
	else if (m_location[0] + m_size < 0) {
		m_new_location[0] = m_environment_width;
		//m_velocity[0] *= -1;
	}
	else if (m_location[1] + m_size > m_environment_height) {
		m_new_location[1] = 0;
		//m_velocity[1] *= -1;
	}
	else if (m_location[1] + m_size < 0) {
		m_new_location[1] = m_environment_height;
		//m_velocity[1] *= -1;
	}

	m_new_acceleration = vec2(0, 0);
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

void Particle::update() {
	m_location = m_new_location;
	m_velocity = m_new_velocity;
	m_acceleration = m_new_acceleration;
	m_color = m_new_color;
	update_norm();
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