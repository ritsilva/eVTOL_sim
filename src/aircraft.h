#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <string>
#include "../src/logging.h"
using namespace std;

#define HOUR_TO_MS 60*60*1000

enum Aircraft_states {
    GROUNDED,
    FLYING,
    CHARGING,
    MAX_AIRCRAFT_STATES
};

class Aircraft {
    private:
        // aircraft parameters
        string name;
        int cruiseSpeed;        // mph
        int batteryCapacity;    // kWh
        int timeToCharge;       // hours
        int energyUsage;        // kWh/mile
        int passengerCount;     // people
        int faultProb;          // probabily/hour

        // aircraft stats
        float remainingCharge;
        Aircraft_states current_state;

        // the following should be tracked per aircraft type (name) when reported.  
        int flightTime;         // miliseconds
        float distanceTraveled; // miles
        int timeCharging;       // miliseconds
        int numFaults;


    public:
        Aircraft(string name,
                 int cruiseSpeed,
                 int batteryCapacity,
                 int timeToCharge,
                 int energyUsage,
                 int passengerCount,
                 int faultProb);

        // proceed 'step' (miliseconds)
        void updateParameters(int step);
        // returns true if done charging
        bool charging(int step);
        // accessor function
        inline float getRemainingCharge() {return remainingCharge;}
        inline Aircraft_states getCurrent_state() {return current_state;}
        inline int getFlightTime() {return flightTime;}
        inline float getDistanceTraveled() {return distanceTraveled;}
        inline int getTimeCharging() {return timeCharging;}
        inline int getNumFaults() {return numFaults;}
};


#endif // AIRCRAFT_H