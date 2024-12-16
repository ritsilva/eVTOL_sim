# eVTOL_sim
This is a simple simulation environment for exercising behavoir of electric aircraft and charging stations. For full details on the requirements for this project see [this document](https://github.com/ritsilva/eVTOL_sim/blob/274978f71cffba0e9c7ea2ba12ef77372b367780/eVtol%20Simulation%20Problem%20-%20AnyTimeDomain%20V2.docx).

# Aircraft behavior
Each aircraft has 7 defined characteristics; Cruise Speed (mph), Battery Capacity (kWh), Time to Charge (hours), Energy use at Cruise (kWh/mile), Passenger Count, and Probability of fault per hour. For ease of computation, despite the units of the aircraft's characteristics, all internal computations are converted and done in the time scale of miliseconds. 

Each aircraft shall start fully charged. Once the aircraft runs out of charge it will dock into a charging station and wait to be charged. Once it is fully charged it will begin flying again.


# Charger behavior
Charges whatever aircraft is docked at it. Once the aircraft is fully charged release it and begin charging the next aircraft, if there is any. Each charger has its own queue for aircrafts. If an aircraft docks at the charger but another station opens up while it's waiting, the aircraft can't move to the other station. This is based on the assumption that the aircraft can't move once it is out of charge and the charging stations are far enough apart that the aircraft can't be rolled to another charging station. In the current implmentation an aircraft docks at whichever charger has the shortest line

### <ins>Other potential strategies for docking<ins>
Work can be done to report the total time it would take for all queued aircraft to finish charging at a given station. This would prevent potential bottle necks that are caused by aircraft that have long charging times. However, we are assuming that the aircraft in need of landing does not have this level of detailed information and simply sees how many aircrafts are waiting at a given charging station.

You could potentially argue that all the charging stations are very close to one another such that it doesn't matter where the aircraft lands since it will be able access any of the chargering stations once it has become free. Mimicing the structure of a gas station. If this were the case you may want to implement a shared queue for all the chargering stations rather than the independent queues in the current implementation. This would allow for the shortest charging/waiting time from all the proposed strategies but probably the most difficult to realize in a real environment.

# Building and running unit tests
This project was compiled using g++ 13.2.0

For compiling code execute ```g++ ./src/*.cpp ./sim.cpp -o myprogram.exe```

Running this executable will generate a log file called ```sim.log```

For running unit tests execute: 
- ```g++ ./src/*.cpp ./test/test_aircraft.cpp -o aircraft_test.exe```
- ```g++ ./src/*.cpp ./test/test_charger.cpp -o charger_test.exe```
- ```g++ ./src/*.cpp ./test/test_simloop.cpp -o simloop_test.exe```

Each executable will output the results to the console and create a log file for each unit test with the name of the log being the name of the test.

NOTE: In hindsight it would have been better to implment the unit tests using google test for mocking but I'm unfamiliar with the library and thought it would take too long for the purposes of this project. 
