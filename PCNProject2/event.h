//
//  event.h
//  PCNProject2
//
//  Created by Rahul R Dhanendran on 4/21/14.
//  Copyright (c) 2014 Rahul R Dhanendran. All rights reserved.
//

#ifndef EV_H
#define EV_H

struct Event {
    double time;            // Time at which Event takes place
    int type;            // Type of Event
    int queue;
    Event* next;            // Points to next event in list
    Event(double t, int i, int q) {
        time = t;
        type = i;
        queue = q;
        next = 0;
    }
};

class EventList {
    Event* head;           // Points to first Event in EventList
    int event_count;       // Total number of Events in EventList
    public:
    ~EventList() { clear();}
    EventList() { event_count = 0; head = 0;}
    void insert(double time, int type, int q);  // Insert new event into EventList
    Event* get();                        // Returns first Event in EventList
    void clear();                        // Removes all Events from EventList
    Event* remove(int type);             // Returns first Event of given type
};

#endif
