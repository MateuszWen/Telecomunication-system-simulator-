#include "Event.h"
#include <list>

using namespace std;

Event::Event(int execute_time_) : execute_time_(execute_time_) {};

Event::Event(int execute_time_, int event_priority) : execute_time_(execute_time_), event_priority_(event_priority){};


Event::~Event() {};

void Event::Execute() {};

