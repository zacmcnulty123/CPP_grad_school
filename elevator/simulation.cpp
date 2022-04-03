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

int main(int argc, char const *argv[])
{
  //Setup log4cxx
  BasicConfigurator::configure();
  LoggerPtr logger(Logger::getLogger("Simulation"));
  //Create statistics to hold our end times
  statistics<int> waitTimes = statistics<int>();
  statistics<int> travelTime = statistics<int>();
  //Add all the floors
  std::vector<Floor> floors;
  for (unsigned int i = 0; i <= 100; ++i) {
    floors.push_back(Floor(i));
  }
  int timeBtwFloors;
  try {
    timeBtwFloors = std::stoi(argv[0]);
  }
  catch (std::invalid_argument & e) {}
  std::array<Elevator, 4> elevators = {
    Elevator(1, 5), Elevator(2, 5),
      Elevator(3, 5), Elevator(4, 5)};
  //Need to read in the CSV
  //Going to swap between these two vector for the passengers that
  //Are handled vs being needed to be handled. Want to limit the number
  // of time I need to loop through the entire list
  std::vector<Passenger> passengers;
  //Read in the passenger information
  LOG4CXX_DEBUG(logger, "Parsing Elevator.csv returns:"
    << std::boolalpha <<
    input_helper::parse_csv("/Users/zacharymcnulty/Documents/CPP_grad_school/elevator/dataset/Mod10_Assignment_Elevators.csv", passengers));

  //Sort passengers by their start times
  std::sort(passengers.begin(), passengers.end());

  // for (Passenger & pass : passengers) {
  //   LOG4CXX_INFO(logger, "" << pass);
  // }
  int passengersInSim = passengers.size();
  //Current Time to run through our event loop
  int currTime = passengers[0].getStartTime();
  int count = 0;
  LOG4CXX_INFO(logger, "Simulation Start");
  LOG4CXX_INFO(logger, "Number of Passengers to handle: " << passengers.size());
  //Create stop variable to end the simulation
  bool stop = false;
  //Each iteration of the loop is a second
  while (not stop) {
    std::vector<Passenger>::iterator i = passengers.begin();
    for (Passenger & pass : passengers) {
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
        try {
          passengers.erase(i);
        }
        catch (...) {
          LOG4CXX_ERROR(logger, "Exception Occured!");
        }
      }
      i++;
    }
    for (Elevator & elevator : elevators) {
      auto sortFloors = floors;
      // elevator
      switch (elevator.getElevatorState()) {
        case ElevatorOp::STOPPED_NO_PASS: {
          std::sort(sortFloors.begin(), sortFloors.end());
          if (sortFloors.at(0).getNumPassenger() == 0) {
            break;
          }
          elevator.setNextDestination(sortFloors.at(0).getFloorNumber());
          floors.at(sortFloors.at(0).getFloorNumber()).setState(FloorState::SELECTED);
          // LOG4CXX_DEBUG(logger, "" << elevator);
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
            pass.setTravelTime(currTime);
            travelTime.enqueue(pass.getTravelTime());
            count++;
          }
          //Pick up passengers on the floor
          int numAdded = 0;
          try {
            // LOG4CXX_DEBUG(logger, "" << floors.at(elevator.getCurrFloor()));
          } catch (std::out_of_range & e) {
            // LOG4CXX_ERROR(logger, "ERROR: " << elevator.getCurrFloor());
          }
          for (Passenger & pass : floors.at(elevator.getCurrFloor()).getPassengers()) {
            if (elevator.getCurrNumPassengers() == Elevator::MAX_CAPACITY) {
              break;
            }
            else {
              pass.setWaitTime(currTime);
              waitTimes.enqueue(pass.getWaitTime());
              elevator.addPassenger(pass);
              numAdded++;
            }
          }
          //Set the floor to unselected
          if (numAdded > 0) {
            floors.at(elevator.getCurrFloor()).dequePassengers(numAdded);
          }
          floors.at(elevator.getCurrFloor()).setState(FloorState::NOT_SELECTED);
          //Determine the closest destination and move to it
          if (not (elevator.getCurrNumPassengers() == 0)) {
            elevator.determineNextDestination();
          }
          else {
            elevator.setElevatorState(ElevatorOp::STOPPED_NO_PASS);
          }
          // LOG4CXX_DEBUG(logger, "" << elevator);
          break;
        }
        case ElevatorOp::STOPPING_UP: {
          //Check to transition to stopped UP
          elevator.incrementTimeInState();
          if (not elevator.checkStateTransition()) {
          }
          break;
        }
        case ElevatorOp::STOPPING_DOWN: {
          //Check to transition to stopped down
          elevator.incrementTimeInState();
          if (not elevator.checkStateTransition()) {
          }
          break;
        }
        case ElevatorOp::MOVING_UP: {
          //Check to transition to stopping up
          elevator.incrementTimeInState();
          if (not elevator.checkStateTransition()) {
          }
          break;
        }
        case ElevatorOp::MOVING_DOWN: {
          //Check to transition to stopping down
          elevator.incrementTimeInState();
          if (not elevator.checkStateTransition()) {
          }
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

    if (passengersInSim == count) {
      stop = true;
    }

    
    if (currTime % 4000 == 0) {
      for (Elevator & elevator : elevators) {
        LOG4CXX_DEBUG(logger, "" << elevator);
      }
      LOG4CXX_DEBUG(logger, "Number of passengers satisfied: " 
        << count << " \n "
        << "currTime: " << currTime
        << "\n PassengersLeft in list: " << passengers.size() << "\n");
    }
    currTime++;
  }
  LOG4CXX_INFO(logger, "Simulation Stop");

  LOG4CXX_INFO(logger, "\nWaitTimes: \n" << waitTimes.get_descriptive_statistics());
  LOG4CXX_INFO(logger, "\nTravelTimes: \n" << travelTime.get_descriptive_statistics());
  return 0;
}