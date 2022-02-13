cmake_minimum_required(VERSION 3.0.0)
project(poker_hands VERSION 0.1.0)

find_package(jsoncpp CONFIG REQUIRED)
add_executable(poker_hands main.cpp)
target_link_libraries(poker_hands PRIVATE jsoncpp_object jsoncpp_static JsonCpp::JsonCpp)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
# --- auto-ignore build directory
if(NOT EXISTS ${PROJECT_BINARY_DIR}/.gitignore)
  file(WRITE ${PROJECT_BINARY_DIR}/.gitignore "*")
endif()