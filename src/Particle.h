#pragma once
#include "ofMain.h"
#include "glm/glm.hpp"

class Particle

{
public:
	Particle();
	Particle(float init_x, float init_y, float size, glm::vec3 color);
	void display();
	void step();
	void grow(int growth);

	bool isDead();

	glm::vec2 get_location();
	glm::vec2 get_velocity();
	glm::vec3 get_color();
	float get_color_norm();
	float get_mass();
	void get_neighbors(list<Particle>& elements, float radius);
	void test(list<Particle>& elements, float radius);

private:
	glm::vec2 m_location;
	glm::vec2 m_velocity;
	glm::vec2 m_acceleration;
	float m_size;
	float m_mass;
	int m_lifespan;
	glm::vec3 m_color;
	float m_color_norm;
	float m_environment_width;
	float m_environment_height;
	//Particle** m_neighbors;
	vector<Particle*> m_neighbors;
	int m_num_neighbors;
	int m_max_neighbors;
	float m_beta;

	void update_norm();
	
};

