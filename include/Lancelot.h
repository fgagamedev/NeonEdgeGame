#ifndef LANCELOT_H_
#define LANCELOT_H_

#include "Player.h"
#include "Timer.h"
#include "TileMap.h"
#include "LancelotInputComponent.h"
#include "LancelotGraphicsComponent.h"


class Lancelot : public Player
{
public:
	Lancelot(ItensManager* itemManager,int x, int y);
	~Lancelot();
	void Damage(int damage);
	void Attack();
	void Block();
	void Stop();
	void Combo(std::string c);
	bool Blocking();
	std::string WhichCombo();
	void UpdateTimers(float dt);

private:
	bool blocking;
	std::string combo;
	bool done = false;
};

#endif /* LANCELOT_H_ */
