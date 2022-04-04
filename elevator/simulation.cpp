//Log4cxx stuff
#include "log4cxx/logger.h"
#include "log4cxx/basicconfigurator.h"
#include "log4cxx/helpers/exception.h"
using namespace log4cxx;
using namespace log4cxx::helpers;

//Going to capture some statistics to be used
//Reusing the class made earlier in the semester
#include "../statistics/statistics.cpp"
#include "../helpers/input_helper.h"
//Stuff for our simulation
#include "passenger.h"
#include "Floor.h"
#include "Elevator.h"

#include <array>


//@Brief - Run the simulation with a set of parameters
//@Param[in] passFilePath - Path to the passenger file to read
//@Param[in] logger - Pointer to the logger of the running program
//@Param[in] simName - name of simulation for output messages
//@Param[in] timeBtwFloors - Time it takes an elevator to move between floors
//@Param[in] numFloors = number of floors in the simulation
//@Param[in] debug = Flag for debug messages
//@return - Statistics objects for wait and travel times
std::vector<statistics<int>> runSimulation(
  const std::string & passFilePath,
  const LoggerPtr & logger,
  const std::string & simName,
  const int & timeBtwFloors = 10,
  const int & numFloors = 100,
  const bool & debug=false) {
    //Create statistics to hold our end times
    statistics<int> waitTimes = statistics<int>();
    statistics<int> travelTime = statistics<int>();

      //Add all the floors
    std::vector<Floor> floors;
    for (unsigned int i = 0; i <= numFloors; ++i) {
      floors.push_back(Floor(i));
    }
    std::array<Elevator, 4> elevators = {
      Elevator(1, timeBtwFloors), Elevator(2, timeBtwFloors),
        Elevator(3, timeBtwFloors), Elevator(4, timeBtwFloors)};

    //Need to read in the CSV
    //Going to swap between these two vector for the passengers that
    //Are handled vs being needed to be handled. Want to limit the number
    // of time I need to loop through the entire list
    std::vector<Passenger> passengers;
    //Read in the passenger information
    try { 
      if (not input_helper::parse_csv(passFilePath, passengers)) {
        LOG4CXX_ERROR(logger, "Error Parsing Passenger List");
      }
    }
    catch (std::runtime_error & e) {
      LOG4CXX_ERROR(logger, e.what());
      return std::vector<statistics<int>>();
    }
    //Sort passengers by their start times
    std::sort(passengers.begin(), passengers.end());

    int passengersInSim = passengers.size();
    //Current Time to run through our event loop
    int currTime = passengers[0].getStartTime();
    int count = 0;

    LOG4CXX_INFO(logger, simName << " Start");
    LOG4CXX_INFO(logger, "Number of Passengers to handle: " << passengers.size());

    //Create stop variable to end the simulation
    bool stop = false;
    //Each iteration of the loop is a second
    while (not stop) {
      std::vector<Passenger>::iterator i = passengers.begin();
      for (Passenger & pass : passengers) {
        //If no passenger is waiting there is nothing to do
        if (currTime < pass.getStartTime()) {
          break;
        }
        else {
          if (pass.getStartFloor() == pass.getEndFloor()) {
            //Add in wait time of 0, and travel time of 0
            waitTimes.enqueue(0);
            travelTime.enqueue(0);
            passengers.erase(i);
            count++;
            continue;
          }
          floors.at(pass.getStartFloor()).queuePassenger(pass);
          //Remove the passengers from the list we need to handle
          passengers.erase(i);
        }
        i++;
      }
      for (Elevator & elevator : elevators) {
        auto sortFloors = floors;
        // elevator
        switch (elevator.getElevatorState()) {
          case ElevatorOp::STOPPED_NO_PASS: {
            //Choose the highest priority floor
            std::sort(sortFloors.begin(), sortFloors.end());
            if (sortFloors.at(0).getNumPassenger() == 0) {
              break;
            }
            //Set that destination
            elevator.setNextDestination(sortFloors.at(0).getFloorNumber());
            //Ensure that floor is no longer a priority to be selected
            floors.at(
                sortFloors.at(0).getFloorNumber()).setState(
                    FloorState::SELECTED);
            break;
          }
          case ElevatorOp::STOPPED_UP:
          case ElevatorOp::STOPPED_DOWN: {
            //Unload passengers that are at this end floor
            //Pick up passengers on the floor
            //Add unloaded passengers to completed list
            //Start moving down to the closest downward floor
            //Unload passengers that are at this end floor
            std::deque<Passenger> temp = elevator.removePassengers();
            for (Passenger & pass : temp) {
              //Capture the times of the passenger getting off
              travelTime.enqueue(currTime - pass.getStartTravelTime());
              count++;
            }
            //Pick up passengers on the floor
            int numAdded = 0;
            for (Passenger & pass :
              floors.at(elevator.getCurrFloor()).getPassengers()) {
              if (elevator.getCurrNumPassengers() == Elevator::MAX_CAPACITY) {
                break;
              }
              else {
                //Capture the passenger's wait time and bookkeepping 
                // variable for travel times
                pass.setStartTravelTime(currTime);
                waitTimes.enqueue(currTime - pass.getStartTime());
                elevator.addPassenger(pass);
                numAdded++;
              }
            }
            //Set the floor to unselected
            if (numAdded > 0) {
              floors.at(elevator.getCurrFloor()).dequePassengers(numAdded);
            }
            //Allow the floor to be prioritized again
            floors.at(
              elevator.getCurrFloor()).setState(FloorState::NOT_SELECTED);
            //Determine the closest destination and move to it
            if (not (elevator.getCurrNumPassengers() == 0)) {
              elevator.determineNextDestination();
            }
            else {
              elevator.setElevatorState(ElevatorOp::STOPPED_NO_PASS);
            }
            break;
          }
          case ElevatorOp::STOPPING_UP:
          case ElevatorOp::STOPPING_DOWN:
          case ElevatorOp::MOVING_UP:
          case ElevatorOp::MOVING_DOWN: {
            //Check to transition to stopping down
            elevator.checkStateTransition();
            break; 
          }
          default: {
            LOG4CXX_ERROR(logger, "Elevator: " << elevator.getElevNum()
              << " Invalid Elevator Operation State!");
            stop = true;
            break;
          }
        }
      }
      //Stop the simulation if all passengers are handled
      if (passengersInSim == count) {
        stop = true;
      }
      if (debug) {
        if (currTime % 4000 == 0) {
          for (Elevator & elevator : elevators) {
            LOG4CXX_DEBUG(logger, "" << elevator);
          }
          LOG4CXX_DEBUG(logger, "Number of passengers satisfied: " 
            << count << " \n "
            << "currTime: " << currTime
            << "\n PassengersLeft in list: " << passengers.size() << "\n");
        }
      }
      currTime++;
    }
    LOG4CXX_INFO(logger, simName << " Stop");
    std::vector<statistics<int>> ret;
    ret.push_back(waitTimes);
    ret.push_back(travelTime);
    return ret;
}

int main(int argc, char const *argv[])
{
  //Setup log4cxx
  BasicConfigurator::configure();
  LoggerPtr logger(Logger::getLogger("Simulation"));

  std::vector<statistics<int>> stats10 =
    runSimulation("./dataset/Mod10_Assignment_Elevators.csv", logger,
      "Time Between Floors is 10 seconds");

  LOG4CXX_INFO(logger, "\nWaitTimes: \n" 
    << stats10[0].get_descriptive_statistics());
  LOG4CXX_INFO(logger, "\nTravelTimes: \n" 
    << stats10[1].get_descriptive_statistics());
  
  std::vector<statistics<int>> stats5 =
    runSimulation("./dataset/Mod10_Assignment_Elevators.csv", logger,
    "Time Between Floor is 5 seconds", 5);

  LOG4CXX_INFO(logger, "\nWaitTimes: \n" 
    << stats5[0].get_descriptive_statistics());
  LOG4CXX_INFO(logger, "\nTravelTimes: \n" 
    << stats5[1].get_descriptive_statistics());
  
  int waitDecrease = stats10[0].get_mean() - stats5[0].get_mean();
  int travelDecrease = stats10[1].get_mean() - stats5[1].get_mean();

  LOG4CXX_INFO(logger, "% Decrease for wait times: " 
    << waitDecrease / stats10[0].get_mean() * 100);

  LOG4CXX_INFO(logger, "% Decrease for travel times: "
    << travelDecrease / stats10[1].get_mean() * 100);
  return 0;
}