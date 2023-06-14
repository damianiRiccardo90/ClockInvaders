///////////////////////////  I N C L U D E S  ////////////////////////////

// Header.
#include "RandomGenerator.h"

////////////////  F U N C T I O N  D E F I N I T I O N S  ////////////////

RandomGenerator& RandomGenerator::GetInstance()
{
	static RandomGenerator instance;
	return instance;
}

float RandomGenerator::GenerateFloat( const float min, const float max )
{
	std::uniform_real_distribution< float > distribution( min, max );
	return distribution( m_Generator );
}

bool RandomGenerator::GenerateBool()
{
	std::uniform_int_distribution< int > distribution( 0, 1 );
	return distribution( m_Generator ) == 1;
}

RandomGenerator::RandomGenerator() 
	: m_Generator( std::random_device()( ) )
{}
