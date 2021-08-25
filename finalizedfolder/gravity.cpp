#include "gravity.h"
#include <math.h>
gravity::gravity(float mass, float dist):m_mass(mass),m_dist(dist)
{
}

float gravity::getfieldintensity()
{
	return 6.67*pow(10,-11)*m_mass/(pow(m_dist,2));
}
