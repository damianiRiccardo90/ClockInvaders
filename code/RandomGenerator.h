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

	// Private constructor to prevent instantiation, and deleted copy and move constructors as the class is 
	// a singleton.
	RandomGenerator();
	RandomGenerator( const RandomGenerator& ) = delete;
	RandomGenerator& operator=( const RandomGenerator& ) = delete;
	RandomGenerator( RandomGenerator&& ) = delete;
	RandomGenerator& operator=( RandomGenerator&& ) = delete;

	std::mt19937 m_Generator;
};
