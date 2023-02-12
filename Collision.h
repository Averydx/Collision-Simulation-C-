#pragma once
struct Collision
{
	int wall; 
	double timeToCollision; 
	bool ballCollision; 


public: 
	Collision()
	{
		wall = 0; 
		timeToCollision = 0.; 
		ballCollision = false; 
	}

};

