///////////////////////////  I N C L U D E S  ////////////////////////////

// Header.
#include "C_Application.h"

// Local.
#include "Cannon.h"
#include "graphics.h"
#include "Projectile.h"

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
	RequestSpawnEntity( Entity::Type::CANNON );
}

void C_Application::Tick( const T_PressedKey pressedKeys, const float deltaTime )
{
	if ( deltaTime == 0.f ) return;

	// Update cached delta time.
	m_DeltaTime = deltaTime;

	// The following is a common order of execution for a game system.

	ProcessInput( pressedKeys );

	CheckEntityCollisions();

	UpdateEntities();

	SpawnEntities();

	RenderEntities();

	CheckEntityDestruction();
}

void C_Application::ClearScreen()
{
	const int screenWidth = static_cast<int>( m_ScreenWidth );
	const int screenHeight = static_cast<int>( m_ScreenHeight );
	FillRect( 0, 0, screenWidth, screenHeight, m_ScreenBackgroundColor );
}

void C_Application::RequestSpawnEntity( const Entity::Type type, const Vector2D& pos /*= Vector2D::s_Zero*/, 
	const Vector2D& facing /*= Vector2D::s_Up */ )
{
	switch ( type )
	{
		case Entity::Type::CANNON:
		{
			// There can be only one.
			if ( !m_Cannon )
			{
				std::unique_ptr< Entity > cannonUniquePtr = std::make_unique< Cannon >( this );
				// Go straight to the entities vector, without passing through the spawning list.
				m_Entities.push_back( std::move( cannonUniquePtr ) );

				m_Cannon = static_cast< Cannon* >( m_Entities.back().get() );
			}

			break;
		}

		case Entity::Type::PROJECTILE:
		{
			std::unique_ptr< Entity > projectileUniquePtr = std::make_unique< Projectile >( this, pos, facing );
			m_ToBeSpawnedEntities.push_back( std::move( projectileUniquePtr ) );

			break;
		}

		default: break;
	}
}

void C_Application::ProcessInput( const T_PressedKey pressedKeys )
{
	if ( !m_Cannon ) return;

	if ( pressedKeys & s_KeyLeft )
	{
		m_Cannon->SetAngularVelocity( Cannon::s_Default_AngularVelocity );
	}
	else if ( pressedKeys & s_KeyRight )
	{
		m_Cannon->SetAngularVelocity( -Cannon::s_Default_AngularVelocity );
	}
	else
	{
		m_Cannon->SetAngularVelocity( 0.f );
	}

	if ( pressedKeys & s_KeySpace )
	{
		m_Cannon->SetFiring( true );
	}
	else
	{
		m_Cannon->SetFiring( false );
	}

	// TODO: Implement graceful exit on esc key?
}

void C_Application::CheckEntityCollisions()
{
	// Pretty trivial collision check, as I don't have time to do something more efficient. Interesting ideas
	// could have been quad trees to partition the space and optimize the computation for larger entity pools.

	for ( auto it1 = m_Entities.begin(); it1 != m_Entities.end(); ++it1 )
	{
		// Avoid checking any collision for the cannon.
		if ( it1->get() == m_Cannon ) continue;

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

void C_Application::UpdateEntities()
{
	for ( auto it = m_Entities.begin(); it != m_Entities.end(); ++it )
	{
		( *it )->Tick( m_DeltaTime );
	}
}

void C_Application::SpawnEntities()
{
	// Transfer the entities from the to-be-spawned list to the main entity list.
	for ( auto it = m_ToBeSpawnedEntities.begin(); it != m_ToBeSpawnedEntities.end(); ++it )
	{
		m_Entities.push_back( std::move( *it ) );
	}

	// Clear the to-be-spawned list after transferring the entities.
	m_ToBeSpawnedEntities.clear();
}

void C_Application::RenderEntities()
{
	// Clear the screen before drawing the entities.
	ClearScreen();

	for ( auto it = m_Entities.begin(); it != m_Entities.end(); ++it )
	{
		( *it )->Render();
	}
}

void C_Application::CheckEntityDestruction()
{
	for ( auto it = m_Entities.begin(); it != m_Entities.end(); )
	{
		// The cannon is not supposed to be removed at anytime.
		if ( ( *it )->IsPendingDestruction() && it->get() != m_Cannon )
		{
			it = m_Entities.erase( it );
		}
		else
		{
			++it;
		}
	}
}
