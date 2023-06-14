#pragma once

///////////////////////////  I N C L U D E S  ////////////////////////////

// System.
#include <random>

//////////////////  C L A S S  D E C L A R A T I O N S  //////////////////

class RandomGenerator
{
public:

	static RandomGenerator& GetInstance();

	// Generate a random float between min and max
	float GenerateFloat( const float min, const float max );
	// Generate a random boolean
	bool GenerateBool();

private:

	// Private constructor to prevent instantiation
	RandomGenerator();

	std::mt19937 m_Generator;
};
