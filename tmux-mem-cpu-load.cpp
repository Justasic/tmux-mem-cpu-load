/*
 * Copyright 2012 Matthew McCormick
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h> // sleep
#include <math.h> // for floorf

// if we are on a BSD system
#if defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)
// TODO: Includes and *BSD support
# define BSD_BASED 1
#endif

// Function declarations for global use.
extern float cpu_percentage( unsigned int cpu_usage_delay );
extern std::string mem_string();
extern std::string load_string();

// This function is platform independent and therefore can be placed at the top
std::string cpu_string( unsigned int cpu_usage_delay, unsigned int graph_lines )
{
	std::string meter( graph_lines + 2, ' ' );
	meter[0] = '[';
	meter[meter.length() - 1] = ']';
	int meter_count = 0;
	float percentage;
	std::ostringstream oss;
	oss.precision( 1 );
	oss.setf( std::ios::fixed | std::ios::right );

	percentage = cpu_percentage( cpu_usage_delay );
	float meter_step = 99.9 / graph_lines;
	meter_count = 1;
	while(meter_count*meter_step < percentage)
	{
		meter[meter_count] = '|';
		meter_count++;
	}

	oss << meter;
	oss.width( 5 );
	oss << percentage;
	oss << "%";

	return oss.str();
}

int main(int argc, char** argv)
{
	unsigned int cpu_usage_delay = 900000;
	unsigned int graph_lines = 10;
	try
	{
		std::istringstream iss;
		iss.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
		if( argc > 1 )
		{
			iss.str( argv[1] );
			unsigned int status_interval;
			iss >> status_interval;
			cpu_usage_delay = status_interval * 1000000 - 100000;
		}

		if( argc > 2 )
		{
			iss.str( argv[2] );
			iss.clear();
			iss >> graph_lines;
		}
	}
	catch(const std::exception &e)
	{
		std::cerr << "Usage: " << argv[0] << " [tmux_status-interval(seconds)] [graph lines]" << std::endl;
		return 1;
	}

	std::cout << mem_string() << ' ' << cpu_string( cpu_usage_delay, graph_lines ) << ' ' << load_string();

	return 0;
}

