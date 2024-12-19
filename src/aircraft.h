#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <string>
#include <vector>
#include "./logging.h"
using namespace std;

#define HOUR_TO_MS (60*60*1000)

enum Aircraft_states {
    GROUNDED,
    FLYING,
    DOCKED,
    CHARGING,
    MAX_AIRCRAFT_STATES
};

// function used to express Aircraft_states in testing
string aircraftStateToString(Aircraft_states state);


// NOTE: I considered making inheriated classes for differing aircraft types.
// This would allow for custom behavior and parameters for each aircraft.
// Given more time I would try implementing this to allow for further development
// of the simulation models for each aircraft and addition of new aircrafts.
class Aircraft {
    private:
        // aircraft parameters - things that shouldn't change
        string name;
        float cruiseSpeed;        // mph
        float batteryCapacity;    // kWh
        float timeToCharge;       // hours
        float energyUsage;        // kWh/mile
        int passengerCount;       // people
        float faultProb;          // probabily/hour

        // aircraft stats - variable for keep track of current processes
        float remainingCharge;
        int currentChargingTime;
        Aircraft_states currentState;

        // the following should be tracked per aircraft type (name) when reported.  
        int flightTime;         // miliseconds
        float distanceTraveled; // miles
        int timeCharging;       // miliseconds
        int timeWaiting;        // miliseconds
        int numFaults;

        // variables for keeping track of previous processes
        vector<int> pastFlightTimes;
        vector<int> pastFlightDistances;
        vector<int> pastChargingTimes;

        // function used to calculate if a fault occurs
        // bases on the probability you give it (prob)
        bool doesFaultOccur(float prob);

    public:
        // constructor
        Aircraft(string name,
                 float cruiseSpeed,
                 float batteryCapacity,
                 float timeToCharge,
                 float energyUsage,
                 int passengerCount,
                 float faultProb);

        // proceed 1 'step' of time (miliseconds)
        void processTime(int step);

        // change aircraft state to charging
        void beginCharging();

        // change aircraft state to flying
        void beginFlying();

        // change aircraft state to grounded
        void dockIntoCharger();

        // returns true if done charging
        bool charge(int step);

        // accessor functions
        float getChargeTime() {return timeToCharge*HOUR_TO_MS;}
        float getRemainingCharge() {return remainingCharge;}
        Aircraft_states getCurrentState() {return currentState;}
        int getFlightTime() {return flightTime;}
        float getDistanceTraveled() {return distanceTraveled;}
        int getCurrentChargingTime() {return currentChargingTime;}
        int getNumFaults() {return numFaults;}
        string getName() {return name;}

        // accessor functions for past data
        float getPassengerMiles();
        vector<int> getPastFlightTimes() {return pastFlightTimes;}
        vector<int> getPastDistanceTraveled() {return pastFlightDistances;}
        vector<int> getPastChargingTimes() {return pastChargingTimes;}

        float getAvgFlightTime();
        float getAvgDistanceTraveled();
        float getAvgChargingTimes();
};


#endif // AIRCRAFT_H