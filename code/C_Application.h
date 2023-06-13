#pragma once

///////////////////////////  I N C L U D E S  ////////////////////////////

// System.
#include <memory>
#include <vector>

// Local.
#include "Entity.h"

////////////////  F O R W A R D  D E C L A R A T I O N S  ////////////////

class Cannon;

//////////////////  C L A S S  D E C L A R A T I O N S  //////////////////

class C_Application
{
public:
	
	typedef unsigned int T_PressedKey;

	// Keys.
	static const T_PressedKey s_KeyLeft;
	static const T_PressedKey s_KeyUp;
	static const T_PressedKey s_KeyRight;
	static const T_PressedKey s_KeyDown;
	static const T_PressedKey s_KeySpace;

	// Colors
	static const unsigned int s_Color_Red;
	static const unsigned int s_Color_Green;
	static const unsigned int s_Color_Blue;
	static const unsigned int s_Color_Black;
	static const unsigned int s_Color_White;

	C_Application( const float screenWidth, const float screenHeight );
	~C_Application() {}

	// Tick is called on fixed framerate (50fps).
	void Tick( const T_PressedKey pressedKeys, const float deltaTime );

	float GetScreenWidth() const { return m_ScreenWidth; }
	float GetScreenHeight() const { return m_ScreenHeight; }

	void RequestSpawnEntity( const Entity::Type type, const Vector2D& position = Vector2D::s_Zero, 
		const Vector2D& facing = Vector2D::s_Up, const Vector2D& velocity = Vector2D::s_Zero );

private:

	// Process the input from the keyboard.
	void ProcessInput( const T_PressedKey pressedKeys );
	// Check the collision between entities and the screen borders, and handle them.
	void CheckEntityCollisions();
	// Make all the entities "tick".
	void UpdateEntities();
	// Spawns all the entities that were requested during this frame.
	void SpawnEntities();
	// Draw every entity on screen.
	void RenderEntities();
	// Iterate over each entity to check if we should remove them from the game.
	void CheckEntityDestruction();

	void ClearScreen();
	void SpawnStartingEntities();

	const float m_ScreenWidth;
	const float m_ScreenHeight;
	const int m_ScreenBackgroundColor;

	float m_DeltaTime;

	// Keeping a separate raw pointer to the cannon entity for accessing it with ease.
	Cannon* m_Cannon;
	// All the entities currently in game.
	std::vector< std::unique_ptr< Entity > > m_Entities;
	// Entities that are waiting to be spawned (avoids invalidating iterators if, for instance, an entity wants to
	// spawn another one while updating).
	std::vector< std::unique_ptr< Entity > > m_ToBeSpawnedEntities;
};
