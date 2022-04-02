# include "log4cxx/logger.h"
# include "log4cxx/basicconfigurator.h"
# include "log4cxx/helpers/exception.h"

using namespace log4cxx;
using namespace log4cxx::helpers;

int main(){
	BasicConfigurator::configure();
	LoggerPtr logger(Logger::getLogger("main"));
	LOG4CXX_DEBUG(logger, "Hello World");
}