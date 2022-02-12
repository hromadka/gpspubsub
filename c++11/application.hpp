/*
 * The software is provided "as is", with no warranty of any
 * type, including any warranty for fitness for any purpose. Author is under no
 * obligation to maintain or support the software. Author shall not be liable for
 * any incidental or consequential damages arising out of the use or inability
 * to use the software.
 * DDS portions of software licensed solely for use with RTI Connext DDS.
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

	int gpsport;
	bool simulation_mode;
	int provider_id;
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

	int gpsport = 0;
	bool simulation_mode = false;
	int provider_id = 0; 

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
                && (strcmp(argv[arg_processing], "-p") == 0
                || strcmp(argv[arg_processing], "--port") == 0)) {
            gpsport = atoi(argv[arg_processing + 1]);
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
            std::cout << "Bad parameter: " << argv[arg_processing] << std::endl;
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
                    "    -i, --id-provider  <int>   ProviderID.\n"
                    "                               Range: 0-7 \n"
                    "                               Default: 0\n"
                    "    -p, --port         <int>   GPS port at /dev/ttyUSB{n}\n"
                    "                               Range: the last digit n of 'ttyUSB{n}' \n"
                    "                               Default: 0 \n"
                    "    -s, --sample-count <int>   Number of samples to receive before\n"\
                    "                               cleanly shutting down. \n"
                    "                               Default: infinite\n"
                    "    -v, --verbosity    <int>   How much debugging output to show.\n"\
                    "                               Range: 0-5 \n"
                    "                               Default: 0\n"
                << std::endl;
    }

    return { parse_result, domain_id, sample_count, verbosity, gpsport, simulation_mode, provider_id };
}

}  // namespace application

#endif  // APPLICATION_HPP
