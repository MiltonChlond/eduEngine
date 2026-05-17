#pragma once

#include "ObserverPattern.h"

struct HealthComponent : public Source
{
	int currentHealth = 0;
	int maxHealth = 100;

	HealthComponent(int curHealth, int maxHealth) : currentHealth(curHealth), maxHealth(maxHealth) {}

	void TakeDamage(int damage)
	{
		currentHealth -= damage;
		Notify(Event::Taken_Damage);
	}
};