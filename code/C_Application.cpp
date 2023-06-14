///////////////////////////  I N C L U D E S  ////////////////////////////

// Header.
#include "C_Application.h"

// Local.
#include "Cannon.h"
#include "graphics.h"
#include "Projectile.h"

//////////////  S T A T I C  M E M B E R  V A R I A B L E S  /////////////

const unsigned int C_Application::s_LEFT  = 0x01;
const unsigned int C_Application::s_UP    = 0x02;
const unsigned int C_Application::s_RIGHT = 0x04;
const unsigned int C_Application::s_DOWN  = 0x08;
const unsigned int C_Application::s_SPACE = 0x10;

////////////////  F U N C T I O N  D E F I N I T I O N S  ////////////////

unsigned int C_Application::GetRed()
{
	static const unsigned int k_RED = GetRGB( 255, 0, 0 );
	return k_RED;
}

unsigned int C_Application::GetGreen()
{
	static const unsigned int k_GREEN = GetRGB( 0, 255, 0 );
	return k_GREEN;
}

unsigned int C_Application::GetBlue()
{
	static const unsigned int k_BLUE = GetRGB( 0, 0, 255 );
	return k_BLUE;
}

unsigned int C_Application::GetBlack()
{
	static const unsigned int k_BLACK = GetRGB( 0, 0, 0 );
	return k_BLACK;
}

unsigned int C_Application::GetWhite()
{
	static const unsigned int k_WHITE = GetRGB( 255, 255, 255 );
	return k_WHITE;
}

C_Application::C_Application( const float screenWidth, const float screenHeight )
	: m_ScreenWidth( screenWidth )
	, m_ScreenHeight( screenHeight )
	, m_ScreenBackgroundColor( GetBlack() )
	, m_DeltaTime( 0 )
	, m_Cannon( nullptr )
	, m_Entities()
	, m_ToBeSpawnedEntities()
{
	SpawnStartingEntities();
}

void C_Application::Tick( const unsigned int pressedKeys, const float deltaTime )
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

void C_Application::RequestSpawnCannon()
{
	// There can be only one.
	if ( !m_Cannon )
	{
		std::unique_ptr< Entity > cannonUniquePtr = std::make_unique< Cannon >( this );
		// Go straight to the entities vector, without passing through the spawning list.
		m_Entities.push_back( std::move( cannonUniquePtr ) );

		m_Cannon = static_cast<Cannon*>( m_Entities.back().get() );
	}
}

void C_Application::RequestSpawnProjectile( const Vector2D& position, const Vector2D& facing )
{
	std::unique_ptr< Entity > projectileUniquePtr =
		std::make_unique< Projectile >( this, position, facing );
	m_ToBeSpawnedEntities.push_back( std::move( projectileUniquePtr ) );
}

void C_Application::RequestSpawnClock( const Vector2D& position /*= Vector2D::GetZero()*/, 
	const Vector2D& velocity /*= Vector2D::GetZero()*/, const Vector2D& halfDiag /*= Clock::s_DEFAULT_BBHALFDIAG*/ )
{
	std::unique_ptr< Entity > projectileUniquePtr =
		std::make_unique< Clock >( this, position, velocity, halfDiag );
	m_ToBeSpawnedEntities.push_back( std::move( projectileUniquePtr ) );
}

void C_Application::ProcessInput( const unsigned int pressedKeys )
{
	if ( !m_Cannon ) return;

	if ( pressedKeys & s_LEFT )
	{
		m_Cannon->SetAngularVelocity( Cannon::s_DEFAULT_ANGULARVELOCITY );
	}
	else if ( pressedKeys & s_RIGHT )
	{
		m_Cannon->SetAngularVelocity( -Cannon::s_DEFAULT_ANGULARVELOCITY );
	}
	else
	{
		m_Cannon->SetAngularVelocity( 0.f );
	}

	if ( pressedKeys & s_SPACE )
	{
		m_Cannon->SetFiring( true );
	}
	else
	{
		m_Cannon->SetFiring( false );
	}
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
				// Handle the collision between entity1 and entity2 (let them handle it themselves, on both sides).
				( *it1 )->HandleCollision( it2->get() );
				( *it2 )->HandleCollision( it1->get() );
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
	CheckRestart();

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

void C_Application::ClearScreen()
{
	const int screenWidth = static_cast<int>( m_ScreenWidth );
	const int screenHeight = static_cast<int>( m_ScreenHeight );
	FillRect( 0, 0, screenWidth, screenHeight, m_ScreenBackgroundColor );
}

void C_Application::SpawnStartingEntities()
{
	// Spawning the cannon
	RequestSpawnCannon();

	// Spawning the first two clocks randomly.
	RequestSpawnClock();
	RequestSpawnClock();
}

void C_Application::CheckRestart()
{
	if ( m_Entities.size() == 1 && m_ToBeSpawnedEntities.empty() )
	{
		SpawnStartingEntities();
	}
}
