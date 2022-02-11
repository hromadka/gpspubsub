/*
 * (c) Copyright, Real-Time Innovations, 2020.  All rights reserved.
 * RTI grants Licensee a license to use, modify, compile, and create derivative
 * works of the software solely for use with RTI Connext DDS. Licensee may
 * redistribute copies of the software provided that all such copies are subject
 * to this license. The software is provided "as is", with no warranty of any
 * type, including any warranty for fitness for any purpose. RTI is under no
 * obligation to maintain or support the software. RTI shall not be liable for
 * any incidental or consequential damages arising out of the use or inability
 * to use the software.
 */

#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <iostream>
#include <csignal>
#include <dds/core/ddscore.hpp>

#include <cstring>

namespace application {

// Catch control-C and tell application to shut down
bool shutdown_requested = false;

inline void stop_handler(int)
{
    shutdown_requested = true;
    std::cout << "preparing to shut down..." << std::endl;
}

inline void setup_signal_handlers()
{
    signal(SIGINT, stop_handler);
    signal(SIGTERM, stop_handler);
}

enum class ParseReturn {
    ok,
    failure,
    exit
};

struct ApplicationArguments {
    ParseReturn parse_result;
    unsigned int domain_id;
    unsigned int sample_count;
    rti::config::Verbosity verbosity;

	//std::string gpsport;
	bool simulation_mode;
	int provider_id;
	//float version_num;
};

// Parses application arguments for example.
inline ApplicationArguments parse_arguments(int argc, char *argv[])
{
    int arg_processing = 1;
    bool show_usage = false;
    ParseReturn parse_result = ParseReturn::ok;
    unsigned int domain_id = 0;
    unsigned int sample_count = (std::numeric_limits<unsigned int>::max)();
    rti::config::Verbosity verbosity;

	std::string gpsport = "ttyUSB0";
	bool simulation_mode = false;
	int provider_id = 0;
	float version_num = 0.2;

    while (arg_processing < argc) {
        if ((argc > arg_processing + 1)
                && (strcmp(argv[arg_processing], "-d") == 0
                || strcmp(argv[arg_processing], "--domain") == 0)) {
            domain_id = atoi(argv[arg_processing + 1]);
            arg_processing += 2;
        } else if ((argc > arg_processing + 1)
                && (strcmp(argv[arg_processing], "-i") == 0
                || strcmp(argv[arg_processing], "--id-provider") == 0)) {
            provider_id = atoi(argv[arg_processing + 1]);
            arg_processing += 2;
        } else if ((argc > arg_processing + 1)
                && (strcmp(argv[arg_processing], "-s") == 0
                || strcmp(argv[arg_processing], "--sample-count") == 0)) {
            sample_count = atoi(argv[arg_processing + 1]);
            arg_processing += 2;
        } else if ((argc > arg_processing + 1)
                && (strcmp(argv[arg_processing], "-v") == 0
                || strcmp(argv[arg_processing], "--verbosity") == 0)) {
            verbosity =
                    static_cast<rti::config::Verbosity::inner_enum>(
                            atoi(argv[arg_processing + 1]));
            arg_processing += 2;
        } else if ((argc > arg_processing + 1)
                && (strcmp(argv[arg_processing], "-f") == 0
                || strcmp(argv[arg_processing], "--fake") == 0)) {
            simulation_mode = true;  // if flag is used at all
            arg_processing += 2;
        } else if (strcmp(argv[arg_processing], "-h") == 0
                || strcmp(argv[arg_processing], "--help") == 0) {
            std::cout << "Example application." << std::endl;
            show_usage = true;
            parse_result = ParseReturn::exit;
            break;
        } else {
            std::cout << "Bad parameter, yo. " << argv[arg_processing] << std::endl;
            show_usage = true;
            parse_result = ParseReturn::failure;
            break;
        }
    }
    if (show_usage) {
        std::cout << "Usage:\n"\
                    "    -d, --domain       <int>   Domain ID this application will\n" \
                    "                               subscribe in.  \n"
                    "                               Default: 0\n"\
                    "    -f, --fake         <1>     Use simulation mode.\n"
                    "                               Range: 1 = true \n"
                    "                               Default: false\n"
                    "    -s, --sample-count <int>   Number of samples to receive before\n"\
                    "                               cleanly shutting down. \n"
                    "                               Default: infinite\n"
                    "    -v, --verbosity    <int>   How much debugging output to show.\n"\
                    "                               Range: 0-5 \n"
                    "                               Default: 0\n"
                << std::endl;
    }

    return { parse_result, domain_id, sample_count, verbosity, simulation_mode, provider_id };
}

}  // namespace application

#endif  // APPLICATION_HPP
