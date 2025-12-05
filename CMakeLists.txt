cmake_minimum_required(VERSION 3.20)
project(ASC_ode)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

include_directories(src nanoblas/src)

add_subdirectory(nanoblas)
add_subdirectory(mechsystem)

install (
    FILES
    src/autodiff.hpp
    src/implicitRK.hpp
    src/Newton.hpp
    src/nonlinfunc.hpp
    src/ode.hpp
    src/timestepper.hpp
    DESTINATION
    include
)

# Demos
#add_executable(test_ode demos/test_ode.cpp)
#target_link_libraries (test_ode PUBLIC nanoblas)

#add_executable(demo_autodiff demos/demo_autodiff.cpp)
#target_link_libraries (demo_autodiff PUBLIC nanoblas)

# Exercise 17
add_executable(massspring src/exercise17_massspring.cpp)
target_link_libraries(massspring PUBLIC nanoblas)

add_executable(circuit src/exercise17_circuit.cpp)
target_link_libraries(circuit PUBLIC nanoblas)

# Exercise 18
add_executable(legendre src/exercise18_legendre.cpp)

add_executable(pendulum src/exercise18_pendulum.cpp)

# Exercise 19
add_executable(runge_kutta src/exercise19_runge_kutta.cpp)
target_link_libraries(runge_kutta PUBLIC nanoblas)
