#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <string>
using namespace std;


class Aircraft {
    private:
        // aircraft parameters
        string name;
        int cruiseSpeed;        // mph
        int batteryCapacity;    // kWh
        int timeToCharge;       // hours
        int energyUsage;        // kWh/mile
        int passengerCount;
        int faultProb;

        // this should be tracked per aircraft type (name). 
        // will need to get sum from all aircraft of specific type 

        // aircraft stats
        int flightTime;         // miliseconds
        float distanceTraveled; // miles
        int timeCharging;       // miliseconds
        int numFaults;
        // float passengerMiles;   // miles

        float remainingCharge;
        bool isCharging;

    public:
        Aircraft(int cruiseSpeed,
                 int batteryCapacity,
                 int timeToCharge,
                 int energyUsage,
                 int passengerCount,
                 int faultProb) {

                    cruiseSpeed = cruiseSpeed;
                    batteryCapacity = batteryCapacity;
                    timeToCharge = timeToCharge;
                    energyUsage = energyUsage;
                    passengerCount = passengerCount;
                    faultProb = faultProb;
                    
                    flightTime = 0;
                    distanceTraveled = 0;
                    timeCharging = 0;
                    numFaults = 0;
                    remainingCharge = 0; // calc what this is
                    // passengerMiles = 0;
                 }

        // proceed 'step' miliseconds
        // return false if out of charge?
        void updateParameters(int step) {
            flightTime += step;
            distanceTraveled += cruiseSpeed * step/60/60/1000; // converting step(miliseconds) to hours
            // numFaults += rand(faultProb);
            remainingCharge -= step;
        }

        bool charging(int step) {
            timeCharging += step;
            if(timeCharging % timeToCharge == 0) {
                remainingCharge = 1;
                return true;
            }
            return false;
        }
};


#endif // AIRCRAFT_H