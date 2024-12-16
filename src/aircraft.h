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
        int timeWaiting;        // miliseconds
        int numFaults;

        vector<int> pastFlightTimes;
        vector<int> pastFlightDistances;
        vector<int> pastChargingTimes;

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
        void beginFlying();
        void dockIntoCharger();
        // returns true if done charging
        bool charge(int step);

        // accessor function
        float getRemainingCharge() {return remainingCharge;}
        Aircraft_states getCurrentState() {return currentState;}
        int getFlightTime() {return flightTime;}
        float getDistanceTraveled() {return distanceTraveled;}
        int getCurrentChargingTime() {return currentChargingTime;}
        int getNumFaults() {return numFaults;}
        string getName() {return name;}

        float getPassengerMiles();
        vector<int> getPastFlightTimes() {return pastFlightTimes;}
        vector<int> getPastDistanceTraveled() {return pastFlightDistances;}
        vector<int> getPastChargingTimes() {return pastChargingTimes;}

        float getAvgFlightTime();
        float getAvgDistanceTraveled();
        float getAvgChargingTimes();
};


#endif // AIRCRAFT_H