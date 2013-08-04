#include <unistd.h>
#include <iostream>
#include <ifstream>
#include <string>

std::string mem_string()
{
	unsigned int total_mem;
	unsigned int used_mem;
	unsigned int unused_mem;
	size_t line_start_pos;
	size_t line_end_pos;
	std::istringstream iss;
	std::ostringstream oss;
	std::string mem_line;

	std::ifstream meminfo_file( "/proc/meminfo" );
	std::getline( meminfo_file, mem_line );
	line_start_pos = mem_line.find_first_of( ':' );
	line_start_pos++;
	line_end_pos = mem_line.find_first_of( 'k' );
	iss.str( mem_line.substr( line_start_pos, line_end_pos - line_start_pos ) );
	iss >> total_mem;

	used_mem = total_mem;

	for( unsigned int i = 0; i < 3; i++ )
	{
		std::getline( meminfo_file, mem_line );
		line_start_pos = mem_line.find_first_of( ':' );
		line_start_pos++;
		line_end_pos = mem_line.find_first_of( 'k' );
		iss.str( mem_line.substr( line_start_pos, line_end_pos - line_start_pos ) );
		iss >> unused_mem;
		used_mem -= unused_mem;
	}
	meminfo_file.close();

	oss << used_mem / 1024 << '/' << total_mem / 1024 << "MB";

	return oss.str();
}

float cpu_percentage( unsigned int cpu_usage_delay )
{
	std::string stat_line;
	size_t line_start_pos;
	size_t line_end_pos;
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
	std::istringstream iss;

	std::ifstream stat_file("/proc/stat");
	std::getline(stat_file, stat_line);
	stat_file.close();

	// skip "cpu"
	line_start_pos = stat_line.find_first_not_of(" ", 3);
	line_end_pos = stat_line.find_first_of(' ', line_start_pos);
	line_end_pos = stat_line.find_first_of(' ', line_end_pos + 1);
	line_end_pos = stat_line.find_first_of(' ', line_end_pos + 1);
	line_end_pos = stat_line.find_first_of(' ', line_end_pos + 1);

	iss.str( stat_line.substr( line_start_pos, line_end_pos - line_start_pos ) );
	iss >> current_user >> current_nice >> current_system >> current_idle;
	iss.clear();

	usleep( cpu_usage_delay );

	stat_file.open("/proc/stat");
	std::getline(stat_file, stat_line);
	stat_file.close();

	// skip "cpu"
	line_start_pos = stat_line.find_first_not_of(" ", 3);
	line_end_pos = stat_line.find_first_of(' ', line_start_pos);
	line_end_pos = stat_line.find_first_of(' ', line_end_pos + 1);
	line_end_pos = stat_line.find_first_of(' ', line_end_pos + 1);
	line_end_pos = stat_line.find_first_of(' ', line_end_pos + 1);

	iss.str( stat_line.substr( line_start_pos, line_end_pos - line_start_pos ) );
	iss >> next_user >> next_nice >> next_system >> next_idle;
	iss.clear();

	diff_user   = next_user - current_user;
	diff_system = next_system - current_system;
	diff_nice   = next_nice - current_nice;
	diff_idle   = next_idle - current_idle;

	return static_cast<float>(diff_user + diff_system + diff_nice)/static_cast<float>(diff_user + diff_system + diff_nice + diff_idle)*100.0;
}

std::string load_string()
{
	std::ifstream loadavg_file( "/proc/loadavg" );
	std::string load_line;
	std::getline( loadavg_file, load_line );
	loadavg_file.close();

	return load_line.substr( 0, 14 );
}
