#ifndef EVENT_H
#define EVENT_H

#include <list>
#include <vector>
#include<iostream>

using namespace std;

class Event
{
public:
	int execute_time_;
	explicit Event(int execute_time_);
	explicit Event(int execute_time_, int event_priority);
	~Event();
	virtual void Execute() = 0;
	int event_priority_;
};

#endif

// PRIORITY OF A EVENT'S
//GeneratePacket 1.
//FinishSending 2.
//ScanConnection 3.
//ReceivePacket  4.
//
//most important is in situation when packet release a wireless_connection (FinishSending), other can scanning and check that one time wireless_connection was free
//and the second one is if more than 1 packet want to access to connection, is possible to detect a collision

