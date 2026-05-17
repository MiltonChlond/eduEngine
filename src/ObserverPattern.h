#pragma once

#include <entt/fwd.hpp>
#include <entt/entt.hpp>
#include <vector>
#include <algorithm>

class Source;

enum class Event
{
	Event_1,
	Event_2,
	Taken_Damage
};

class Observer
{
public:

	virtual ~Observer() {};
	virtual void OnNotify(Source* source, Event event) = 0;
};

class Source
{
private:

	std::vector<Observer*> observers;

public:

	void Notify(Event event)
	{
		for (auto observer : observers)
		{
			observer->OnNotify(this, event);
		}
	}

	void AddObserver(Observer* observer)
	{
		observers.push_back(observer);
	}

	void RemoveObserver(Observer* observer)
	{
		observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
	}
};



