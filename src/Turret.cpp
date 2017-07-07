#include "Turret.h"

#include "AIMovingOnGroudGraphicsComponent.h"
#include "StageState.h"

Turret::Turret(int x, int y):
	Character(x,y),
	radius(),
	looking(1500),
	idle(1500)
{
	graphicsComponent = new AIMovingOnGroudGraphicsComponent("Turret");
	name = "Turret";
	box.SetWH(graphicsComponent->GetSize());
	attackCD.SetLimit(0);

	idle.Start();
}

Turret::~Turret()
{

}

void Turret::Attack()
{
	//Starts attack timer
	attacking.Start();
}

void Turret::NotifyTileCollision(int tile, GameObject::Face face)
{
	if(tile >= SOLID_TILE && (face == LEFT || face == RIGHT))
	{
		if(physicsComponent.velocity.y >= 0.6)
		{
			physicsComponent.velocity.y = -0.5;
		}
	}
}

void Turret::UpdateTimers(float dt)
{
	Character::UpdateTimers(dt);
	if(looking.IsRunning())
	{
		looking.Update(dt);
		if(!looking.IsRunning())
			idle.Start();
	}
	else if(idle.IsRunning())
	{
		idle.Update(dt);
		if(!idle.IsRunning())
			looking.Start();
	}
}

void Turret::UpdateAI(float dt)
{
	radius = Rect(box.x-200, box.y-200, box.w+400, box.h+400);
	if(StageState::GetPlayer())
	{
		Rect player = StageState::GetPlayer()->box;
		bool visible = true;
		if(StageState::GetPlayer()->Is("Player"))
		{
			Gallahad* p = (Gallahad*) StageState::GetPlayer();
			if(p->Hiding())
			{
				visible = false;
			}
		}
		if(player.OverlapsWith(radius) && visible)
		{
			if(player.x < box.x )
			{
				physicsComponent.velocity.x -= 0.003*dt;
				if(box.x - physicsComponent.velocity.x*dt < player.x)
				{
					box.x = player.x;
					physicsComponent.velocity.x = 0;
				}
				facing = LEFT;
			}
			else
			{
				physicsComponent.velocity.x += 0.003*dt;
				if(box.x + physicsComponent.velocity.x*dt > player.x)
				{
					box.x = player.x;
					physicsComponent.velocity.x = 0;
				}
				facing = RIGHT;
			}
			clamp(physicsComponent.velocity.x,-0.4f,0.4f);

			if(!Attacking() && !Cooling())
			{
				Attack();
			}
		}
		else if(looking.IsRunning() && looking.GetElapsed() == 0)
		{
			if(facing == LEFT)
				physicsComponent.velocity.x = -0.2;
			else
				physicsComponent.velocity.x = 0.2;
		}
		else
		{
			if(idle.IsRunning() && idle.GetElapsed() == 0)
			{
				physicsComponent.velocity.x = 0;
				if(facing == LEFT)
				{
					facing = RIGHT;
				}
				else
				{
					facing = LEFT;
				}
			}
		}
	}
}

void Turret::Update(TileMap* world, float dt)
{
	UpdateTimers(dt);
	UpdateAI(dt);
	physicsComponent.Update(this,world,dt);
	if(OutOfBounds(world))
		SetPosition(Vec2(startingX,startingY));
	graphicsComponent->Update(this,dt);
}

bool Turret::Is(std::string type)
{
	return (type=="Enemy");
}
