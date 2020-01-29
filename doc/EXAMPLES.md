# Examples of how to use stationsimcpp

## Link stationsimcpp using CMake

To link stationsimcpp in your cmake project after installing stationsimcpp use the following CMake code

```bash
find_package(Python3 COMPONENTS Development NumPy)
find_package(stationsimcpp REQUIRED)
target_link_libraries(your_target PRIVATE stationsimcpp::stationsimcpp Python3::Python Python3::NumPy)
```

## Code examples

C++ source code examples can be found in the examples folder in the main repository directory.

### Simple example

The example below run a single simulation and plots the location of the agents every 300 steps.

```c++
#include "ModelParameters.hpp"
#include "Model.hpp"
#include "ModelPlotting.hpp"
#include "Timer.hpp"

int main()
{
	station_sim::ModelParameters model_parameters;
	model_parameters.set_population_total(100);
	station_sim::Model model(0, model_parameters);

	Chronos::Timer timer("timer1");
	timer.start();

	for (int i = 0; i<model_parameters.get_step_limit(); i++) {
		model.step(model, model_parameters);

		if (i%300==0) {
			station_sim::ModelPlotting::plot_agents_locations(model);
		}
	}

	timer.stop_timer(true);

}
```

To build this example on Ubuntu with CMake store the code in a file called example.cpp and use the following CMakeLists.txt file

```cmake
cmake_minimum_required(VERSION 3.15.3)
find_package(Python3 COMPONENTS Development NumPy)
find_package(stationsimcpp REQUIRED)
add_executable(example)
target_sources(example PRIVATE plot_example.cpp)
target_link_libraries(plot_example PRIVATE stationsimcpp::stationsimcpp Python3::Python Python3::NumPy)
```

then use the following in the directory with the CMakeLists.txt and example.cpp to build and run the example

```bash
cmake .
make
./example
```
