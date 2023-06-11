///////////////////////////  I N C L U D E S  ////////////////////////////

// Header.
#include "C_Application.h"

// Local.
#include "Cannon.h"
#include "graphics.h"

//////////////  S T A T I C  M E M B E R  V A R I A B L E S  /////////////

const C_Application::T_PressedKey C_Application::s_KeyLeft = 0x01;
const C_Application::T_PressedKey C_Application::s_KeyUp = 0x02;
const C_Application::T_PressedKey C_Application::s_KeyRight = 0x04;
const C_Application::T_PressedKey C_Application::s_KeyDown = 0x08;
const C_Application::T_PressedKey C_Application::s_KeySpace = 0x10;

const unsigned int C_Application::s_Color_Red = GetRGB( 255, 0, 0 );
const unsigned int C_Application::s_Color_Green = GetRGB( 0, 255, 0 );
const unsigned int C_Application::s_Color_Blue = GetRGB( 0, 0, 255 );
const unsigned int C_Application::s_Color_Black = GetRGB( 0, 0, 0 );
const unsigned int C_Application::s_Color_White = GetRGB( 255, 255, 255 );

////////////////  F U N C T I O N  D E F I N I T I O N S  ////////////////

C_Application::C_Application( const float screenWidth, const float screenHeight )
	: m_ScreenWidth( screenWidth )
	, m_ScreenHeight( screenHeight )
	, m_ScreenBackgroundColor( s_Color_Black )
	, m_DeltaTime( 0 )
	, m_Cannon( nullptr )
	, m_Entities()
{
	// Spawning the cannon

	std::unique_ptr< Entity > cannonUniquePtr = std::make_unique< Cannon >( this );
	m_Entities.push_back( std::move( cannonUniquePtr ) );
	// Keeping a separate raw pointer to the cannon entity for accessing it with ease.
	m_Cannon = static_cast< Cannon* >( m_Entities.back().get() );
}

void C_Application::Tick( const T_PressedKey pressedKeys, const float deltaTime )
{
	// Update cached delta time.
	m_DeltaTime = deltaTime;

	// Key processing.
	ProcessInput( pressedKeys );

	// Clear the screen before drawing the entities.
	ClearScreen();

	// Make all the entities "tick", and then draw them on screen.
	for ( const auto& entity : m_Entities ) 
	{
		entity->Tick( m_DeltaTime );
		entity->Render();
	}

	// Check the collision between entities and the screen borders, and handle them.
	CheckCollisions();
}

void C_Application::ProcessInput( const T_PressedKey pressedKeys )
{
	if ( pressedKeys & s_KeyLeft )
	{
		if ( m_Cannon )
		{
			m_Cannon->SetAngularVelocity( -Cannon::s_Default_AngularVelocity );
		}
	}
	else if ( pressedKeys & s_KeyRight )
	{
		if ( m_Cannon )
		{
			m_Cannon->SetAngularVelocity( Cannon::s_Default_AngularVelocity );
		}
	}
	else
	{
		if ( m_Cannon )
		{
			m_Cannon->SetAngularVelocity( 0.f );
		}
	}

	if ( pressedKeys & s_KeySpace )
	{
		// TODO: Implement shooting.
	}
	// TODO: Implement graceful exit on esc key.
}

void C_Application::ClearScreen()
{
	const int screenWidth = static_cast< int >( m_ScreenWidth );
	const int screenHeight = static_cast< int >( m_ScreenHeight );
	FillRect( 0, 0, screenWidth, screenHeight, m_ScreenBackgroundColor );
}

void C_Application::CheckCollisions()
{
	// Iterate through the collection of entities.

	for ( auto it1 = m_Entities.begin(); it1 != m_Entities.end(); ++it1 )
	{
		/*
		// Avoid checking any collision for the cannon.
		if ( it1->get() == m_Cannon ) continue;
		*/

		if ( ( *it1 )->IsCollidingWithScreenBorders() )
		{
			// Handle the collision between entity1 and the screen borders.
			( *it1 )->HandleScreenBordersCollision();
		}

		for ( auto it2 = std::next( it1 ); it2 != m_Entities.end(); ++it2 )
		{
			if ( ( *it1 )->IsCollidingWith( **it2 ) )
			{
				// Handle the collision between entity1 and entity2.
				( *it1 )->HandleCollision( **it2 );
			}
		}
	}
}
