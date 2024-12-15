#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <string>
#include "./logging.h"
using namespace std;

#define HOUR_TO_MS (60*60*1000)

enum Aircraft_states {
    GROUNDED,
    FLYING,
    CHARGING,
    MAX_AIRCRAFT_STATES
};

string aircraftStateToString(Aircraft_states state);

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
        void processTime(int step);
        void beginCharging();
        // returns true if done charging
        bool charge(int step);
        // accessor function
        float getRemainingCharge() {return remainingCharge;}
        Aircraft_states getCurrentState() {return currentState;}
        int getFlightTime() {return flightTime;}
        float getDistanceTraveled() {return distanceTraveled;}
        int getTimeCharging() {return timeCharging;}
        int getNumFaults() {return numFaults;}
        string getName() {return name;}
        float getPassengerMiles() {return passengerCount*flightTime/HOUR_TO_MS*cruiseSpeed;}
};


#endif // AIRCRAFT_H