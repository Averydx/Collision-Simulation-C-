#pragma once
#include <SFML/Graphics.hpp>
#include "SFML_VECTOR.h"
#include "Collision.h"
class Particle
{


	Vector2D acc;
	Vector2D vel;
	Vector2D pos;
	float mass; 
	int x_size; 
	int y_size; 

public:
	sf::CircleShape c;
public:
	Particle(Vector2D pos, Vector2D vel, sf::Color color,float radius,float mass,int x_size,int y_size)
	{
		this->acc = sf::Vector2f(0, 0);
		this->vel = vel; 
		this->pos = pos;
		c.setRadius(radius);
		c.setFillColor(color);
		c.setOrigin(c.getRadius(), c.getRadius());
		this->mass = mass;
		this->x_size = x_size; 
		this->y_size = y_size; 
		 
	}

	void update_physics()
	{

		//integrate velocity 
		this->vel = this->vel + this->acc;
		if (this->vel.x() > 100)
		{
			this->vel.setX(100);
		}
		if (this->vel.y() > 100)
		{
			this->vel.setY(100);
		}

		

		//integrate position
		this->pos = this->pos + this->vel;


		this->acc = this->acc * 0; 

	}

	void apply_force(Vector2D force)
	{
		this->acc = this->acc + (force * (1 / mass)); 

	}

	void collision_velocity(Particle& collider)
	{
		if (this->getMass() == 0 && collider.getMass() == 0) return; 

		//compute unit normal and unit tangent vectors
		Vector2D v_n = this->getPos() - collider.getPos(); 
		Vector2D v_un = v_n.unitVector(); // unit normal vector
		Vector2D v_ut(-v_un.y(), v_un.x()); // unit tangent vector

		// Compute scalar projections of velocities onto v_un and v_ut
		double v1n = v_un * this->getVel(); // Dot product
		double v1t = v_ut * this->getVel();
		double v2n = v_un * collider.getVel();
		double v2t = v_ut * collider.getVel();

		printf("old_vel: %f\n", v1n);
		printf("old_collider_vel: %f\n", v2n);

		// Compute new tangential velocities
		double v1tPrime = v1t; // Note: in reality, the tangential velocities do not change after the collision
		double v2tPrime = v2t;

		// Compute new normal velocities using one-dimensional elastic collision equations in the normal direction
	// Division by zero avoided. See early return above.
		double v1nPrime = (v1n * ((double)this->getMass() - (double)collider.getMass()) + 
			2. * collider.getMass() * v2n) / ((double)this->getMass() + (double)collider.getMass());

		double v2nPrime = (v2n * ((double)collider.getMass() - (double)this->getMass()) + 
			2. * this->getMass() * v1n) / ((double)this->getMass() + (double)collider.getMass());

		printf("\n");
		printf("new_vel: %f\n", v1nPrime); 
		printf("new_collider_vel: %f\n", v2nPrime);


		// Compute new normal and tangential velocity vectors
		Vector2D v_v1nPrime = v1nPrime * v_un; // Multiplication by a scalar
		Vector2D v_v1tPrime = v1tPrime * v_ut;
		Vector2D v_v2nPrime = v2nPrime * v_un;
		Vector2D v_v2tPrime = v2tPrime * v_ut;

		this->vel.setX(v_v1nPrime.x() + v_v1tPrime.x());
		this->vel.setY(v_v1nPrime.y() + v_v1tPrime.y());
		collider.vel.setX(v_v2nPrime.x() + v_v2tPrime.x()); 
		collider.vel.setY(v_v2nPrime.y() + v_v2tPrime.y());

		
	}

	void collision_check(Particle& collider)
	{
		 Collision* collision = findTimeUntilBallCollidesWithWall();

		 
		 if (collision->timeToCollision <= 1)
		 {
			 switch (collision->wall)
			 {
			 case 1:
				 
				 this->vel.setX(this->vel.x() * -1);
				 break;

			 case 2:
				 this->vel.setX(this->vel.x() * -1);
				 break;

			 case 3:
				 this->vel.setY(this->vel.y() * -1);
				 break;

			 case 4:
				 this->vel.setY(this->vel.y() * -1);
				 break;

			 };

			 

			  
		 }

		 double distance = (this->getPos() - collider.getPos()).magnitude();
		 //compute unit normal 
		 Vector2D v_n = this->getPos() - collider.getPos();
		 Vector2D v_un = v_n.unitVector(); // unit normal vector

		 // Dot product
		 double v1n = v_un * this->getVel(); 
		 double v2n = v_un * collider.getVel();


		 if (distance <= ((double)c.getRadius() + (double)collider.c.getRadius()) && ((v1n - v2n) < 0))
		 {

			collision_velocity(collider);

		 }
		 delete collision;
		
	}

	enum whichWall { none, x1, x2, y1, y2 };
	Collision* findTimeUntilBallCollidesWithWall()
	{
		Collision* collision = new Collision();
		double timeToCollision = 0; 
		whichWall wall = none; 

		
		if (this->vel.x() < 0.)
		{
			double t = (c.getRadius() - this->pos.x() + 0) / this->vel.x();
			if (t >= 0)
			{
				timeToCollision = t; 
				wall = x1; 
			}
		}

		if (this->vel.y() < 0.)
		{
			double t = (c.getRadius() - this->pos.y() + 0) / this->vel.y();

			if (t < timeToCollision || wall == none)
			{
				timeToCollision = t; 
				wall = y1; 
			}
		}

		if (this->vel.x() > 0.)
		{
			double t = (x_size - (double)c.getRadius() - this->pos.x()) / this->vel.x(); 
			if (t < timeToCollision || wall == none)
			{
				timeToCollision = t;
				wall = x2;
			}
		}

		if (this->vel.y() > 0)
		{
			double t = (y_size - (double)c.getRadius() - this->pos.y()) / this->vel.y();
			if (t < timeToCollision || wall == none)
			{
				timeToCollision = t;
				wall = y2;
			}
		}

		

		collision->wall = wall;
		collision->timeToCollision = timeToCollision;
		return collision; 
		
	}
	

	void render(sf::RenderWindow& wind)
	{
		c.setPosition(sf::Vector2f(this->getPos().x(),this->getPos().y()));
		wind.draw(c);
	}

	Vector2D getPos()
	{
		return this->pos; 
	}

	Vector2D getVel()
	{
		return this->vel;
	}


	float getMass()
	{
		return this->mass; 
	}

	void setPos(Vector2D newPos)
	{
		this->pos = newPos; 
	}


};


