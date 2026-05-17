#include "EventQueue.h"

EventQueue::EventQueue()
{
	int id = 0;
	for (auto& pair : listeners)
	{
		pair.first = id++;
	}
}

int EventQueue::Subscribe(Listener listener)
{
	for (auto& pair : listeners)
	{
		if (!pair.second)
		{
			pair.second = listener;
			return pair.first;
		}
	}
	return 256;
};

void EventQueue::UnSubscribe(int id)
{
	if (id < 0 || id >= listeners.size())
		return;
	listeners[id].second = nullptr;
}

void EventQueue::QueueEvent(EventType event)
{
	events.push(event);
}

void EventQueue::BroadCast()
{
	while(!events.empty())
	{
		EventType event = events.front();
		for (auto& pair : listeners)
		{
			if (pair.second == nullptr) continue;
			pair.second(event);
		}
		events.pop();
	}
}

EventQueue eventQueue;