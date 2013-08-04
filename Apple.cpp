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
// This file was Authored by Justin Crawford <Justasic@gmail.com>

// Apple specific.
#if !defined(__APPLE__) || !defined(__MACH__)
# error This file should not have been included in the compile
#endif

// Mach kernel includes for getting memory and CPU statistics
#include <mach/vm_statistics.h>
#include <mach/processor_info.h>
#include <mach/mach_types.h>
#include <mach/mach_init.h>
#include <mach/mach_host.h>
#include <mach/mach.h>
#include <sys/sysctl.h> // for sysctl
#include <sys/types.h> // for integer types
// Standard includes
#include <string>
#include <cmath>

// Memory values
std::string mem_string()
{
	// These values are in bytes
	int64_t total_mem;
	int64_t used_mem;
	int64_t unused_mem;
	// blah
	vm_size_t page_size;
	mach_port_t mach_port;
	mach_msg_type_number_t count;
	vm_statistics_data_t vm_stats;
	std::ostringstream oss;

	// Get total physical memory
	int mib[2];
	mib[0] = CTL_HW;
	mib[1] = HW_MEMSIZE;
	size_t length = sizeof(int64_t);
	sysctl(mib, 2, &total_mem, &length, NULL, 0);

	mach_port = mach_host_self();
	count = sizeof(vm_stats) / sizeof(natural_t);
	if (KERN_SUCCESS == host_page_size(mach_port, &page_size) &&
		KERN_SUCCESS == host_statistics(mach_port, HOST_VM_INFO, (host_info_t)&vm_stats, &count))
	{
		unused_mem = (int64_t)vm_stats.free_count * (int64_t)page_size;

		used_mem = ((int64_t)vm_stats.active_count +
		(int64_t)vm_stats.inactive_count +
		(int64_t)vm_stats.wire_count) *  (int64_t)page_size;
	}

	oss << used_mem / 1024 / 1024 << '/' << total_mem / 1024 / 1024 << "MB";

	return oss.str();
}

// Load Averages
std::string load_string()
{
	std::stringstream ss;
	// Only get 3 load averages
	int nelem = 3;
	double averages[3];
	// based on: http://www.opensource.apple.com/source/Libc/Libc-262/gen/getloadavg.c
	if(getloadavg(averages, nelem) < 0)
		ss << "0.00 0.00 0.00"; // couldn't get averages.
	else
	{
		for(int i = 0; i < nelem; ++i)
		{
			// Round to nearest, make sure this is only a 0.00 value not a 0.0000
			float avg = floorf(static_cast<float>(averages[i]) * 100 + 0.5) / 100;
			ss << avg << " ";
		}
	}

	return ss.str();
}


float cpu_percentage( unsigned int cpu_usage_delay )
{
    Get the load times from the XNU kernel
    host_cpu_load_info_data_t load1 = _get_cpu_percentage();
    usleep(cpu_usage_delay);
    host_cpu_load_info_data_t load2 = _get_cpu_percentage();
    // Current load times
    unsigned long long current_user = load1.cpu_ticks[CPU_STATE_USER];
    unsigned long long current_system = load1.cpu_ticks[CPU_STATE_SYSTEM];
    unsigned long long current_nice = load1.cpu_ticks[CPU_STATE_NICE];
    unsigned long long current_idle = load1.cpu_ticks[CPU_STATE_IDLE];
    // Next load times
    unsigned long long next_user = load2.cpu_ticks[CPU_STATE_USER];
    unsigned long long next_system = load2.cpu_ticks[CPU_STATE_SYSTEM];
    unsigned long long next_nice = load2.cpu_ticks[CPU_STATE_NICE];
    unsigned long long next_idle = load2.cpu_ticks[CPU_STATE_IDLE];
    // Difference between the two
    unsigned long long diff_user = next_user - current_user;
    unsigned long long diff_system = next_system - current_system;
    unsigned long long diff_nice = next_nice - current_nice;
    unsigned long long diff_idle = next_idle - current_idle;

    return static_cast<float>(diff_user + diff_system + diff_nice)/static_cast<float>(diff_user + diff_system + diff_nice + diff_idle)*100.0;
}
