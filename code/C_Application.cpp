///////////////////////////  I N C L U D E S  ////////////////////////////

// Header.
#include "C_Application.h"

// Local.
#include "Cannon.h"
#include "graphics.h"
#include "Projectile.h"

////////////////  F U N C T I O N  D E F I N I T I O N S  ////////////////

C_Application::C_Application( const float screenWidth, const float screenHeight )
	: m_ScreenWidth( screenWidth )
	, m_ScreenHeight( screenHeight )
	, m_ScreenBackgroundColor( Color::k_BLACK )
	, m_DeltaTime( 0 )
	, m_Cannon( nullptr )
	, m_Entities()
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

void C_Application::RequestSpawnClock( const Vector2D& position /*= Vector2D::s_ZERO*/, 
	const Vector2D& velocity /*= Vector2D::s_ZERO*/, const Vector2D& halfDiag /*= k_CLOCK_DEFAULT_BBHALFDIAG*/ )
{
	std::unique_ptr< Entity > projectileUniquePtr =
		std::make_unique< Clock >( this, position, velocity, halfDiag );
	m_ToBeSpawnedEntities.push_back( std::move( projectileUniquePtr ) );
}

void C_Application::ProcessInput( const unsigned int pressedKeys )
{
	if ( !m_Cannon ) return;

	if ( pressedKeys & Key::k_LEFT )
	{
		m_Cannon->SetAngularVelocity( k_CANNON_DEFAULT_ANGULARVELOCITY );
	}
	else if ( pressedKeys & Key::k_RIGHT )
	{
		m_Cannon->SetAngularVelocity( -k_CANNON_DEFAULT_ANGULARVELOCITY );
	}
	else
	{
		m_Cannon->SetAngularVelocity( 0.f );
	}

	if ( pressedKeys & Key::k_SPACE )
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
