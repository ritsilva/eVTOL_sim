#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <string>
#include "../src/logging.h"
using namespace std;

#define HOUR_TO_MS (60*60*1000)

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
        float cruiseSpeed;        // mph
        float batteryCapacity;    // kWh
        float timeToCharge;       // hours
        float energyUsage;        // kWh/mile
        int passengerCount;       // people
        float faultProb;          // probabily/hour

        // aircraft stats
        float remainingCharge;
        int currentChargingTime;
        Aircraft_states currentState;

        // the following should be tracked per aircraft type (name) when reported.  
        int flightTime;         // miliseconds
        float distanceTraveled; // miles
        int timeCharging;       // miliseconds
        int numFaults;

    bool doesFaultOccur(float prob);

    public:
        Aircraft(string name,
                 float cruiseSpeed,
                 float batteryCapacity,
                 float timeToCharge,
                 float energyUsage,
                 int passengerCount,
                 float faultProb);

        // proceed 'step' (miliseconds)
        void updateParameters(int step);
        // returns true if done charging
        bool charging(int step);
        // accessor function
        inline float getRemainingCharge() {return remainingCharge;}
        inline Aircraft_states getCurrentState() {return currentState;}
        inline int getFlightTime() {return flightTime;}
        inline float getDistanceTraveled() {return distanceTraveled;}
        inline int getTimeCharging() {return timeCharging;}
        inline int getNumFaults() {return numFaults;}
};


#endif // AIRCRAFT_H