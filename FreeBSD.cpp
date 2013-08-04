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


#include <unistd.h>
#include <iostream>
#include <ifstream>
#include <string>

// Based on: https://github.com/freebsd/freebsd/blob/master/usr.bin/top/machine.c

float cpu_percentage( unsigned int cpu_usage_delay )
{
	unsigned long long current_user;
	unsigned long long current_system;
	unsigned long long current_nice;
	unsigned long long current_idle;
	unsigned long long next_user;
	unsigned long long next_system;
	unsigned long long next_nice;
	unsigned long long next_idle;
	unsigned long long diff_user;
	unsigned long long diff_system;
	unsigned long long diff_nice;
	unsigned long long diff_idle;

    long total;
    struct loadavg sysload;
    int mib[2];
    struct timeval boottime;
    uint64_t arc_stat, arc_stat2;
    int i, j;
    size_t size;

    /* get the CPU stats */
    size = (maxid + 1) * CPUSTATES * sizeof(long);
    if (sysctlbyname("kern.cp_times", pcpu_cp_time, &size, NULL, 0) == -1)
        err(1, "sysctlbyname kern.cp_times");
    GETSYSCTL("kern.cp_time", cp_time);
    GETSYSCTL("vm.loadavg", sysload);
    GETSYSCTL("kern.lastpid", lastpid);

    /* convert load averages to doubles */
    for (i = 0; i < 3; i++)
        si->load_avg[i] = (double)sysload.ldavg[i] / sysload.fscale;

    /* convert cp_time counts to percentages */
    for (i = j = 0; i <= maxid; i++) {
        if ((cpumask & (1ul << i)) == 0)
            continue;
        percentages(CPUSTATES, &pcpu_cpu_states[j * CPUSTATES],
                &pcpu_cp_time[j * CPUSTATES],
                &pcpu_cp_old[j * CPUSTATES],
                &pcpu_cp_diff[j * CPUSTATES]);
        j++;
    }
}

std::string mem_string()
{

}

std::string load_string()
{

}

