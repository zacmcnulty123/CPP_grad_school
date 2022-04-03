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
  //Create statistics to hold our end times
  statistics<unsigned int> stats = statistics<unsigned int>();

  //Current Time to run through our event loop
  unsigned int currTime = 0;

  //Add all the floors
  std::vector<Floor> floors;
  for (unsigned int i = 1; i <= 100; ++i) {
    floors.push_back(Floor(i));
  }
  std::array<Elevator, 4> elevators = {
    Elevator(1), Elevator(2), Elevator(3), Elevator(4)};

  //Need to read in the CSV
  std::vector<Passenger> temp;
  //Setup log4cxx
  BasicConfigurator::configure();
  LoggerPtr logger(Logger::getLogger("Simulation"));
  LOG4CXX_INFO(logger, "Simulation Start");
  
  //Read in the passenger information
  LOG4CXX_DEBUG(logger, "Parsing Elevator.csv returns:"
    << std::boolalpha <<
    input_helper::parse_csv("./dataset/Mod10_Assignment_Elevators.csv", temp));
  
  for (int i = 0; i < floors.size(); ++i) {
    for (int j = 0; j < temp.size(); ++j) {
      if (temp[j].getStartFloor() == (i+1)) {
        floors[i].queuePassenger(temp[j]);
      }
    }
  }
  
  for (Floor & floor : floors) {
    floor.sortQueue();
    LOG4CXX_DEBUG(logger, "" << floor);
  }

  // std::sort(temp.begin(), temp.end());
  // for (auto & pass : temp) {
  //   LOG4CXX_DEBUG(logger, "" << pass);
  // }
  //Create stop variable to end the simulation
  bool stop = false;
  //Each iteration of the loop is a second
  while (not stop) {


    stop = true;
    //Iterate the seconds 
    currTime += 1;
  }

  LOG4CXX_INFO(logger, "Simulation Stop");
  return 0;
}