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

#include <iostream>

#include <dds/pub/ddspub.hpp>
#include <rti/util/util.hpp>  // for sleep()
#include <rti/config/Logger.hpp>  // for logging

#include "gpspubsub.hpp"
#include "application.hpp"  // Argument parsing

#include <CppLinuxSerial/SerialPort.hpp>
#include <cstring>
#include <stdlib.h>		// stoi
#include <locale>		// std::locale, std::tolower
#include <unistd.h>		// sleep

using namespace mn::CppLinuxSerial;

using namespace application;

const float VERSION=0.2;

void run_example(unsigned int domain_id, unsigned int sample_count, bool simulation_mode, int gpsport)
{
    // DomainParticipant QoS is configured in USER_QOS_PROFILES.xml
    dds::domain::DomainParticipant participant(domain_id);

    // Create Topic
    dds::topic::Topic<Position> topic(participant, "GPS Topic");

    // Publisher QoS is configured in USER_QOS_PROFILES.xml
    dds::pub::Publisher publisher(participant);

    // This DataWriter will write data on Topic "GPS Topic"
    // DataWriter QoS is configured in USER_QOS_PROFILES.xml
    dds::pub::DataWriter<Position> writer(publisher, topic);


	if (simulation_mode) {
		while (1) {
			//"$GPGGA,165125.568,,,,,0,00,,,M,0.0,M,,0000*59"
			std::string lat = "33.70024";  //PHX
			std::string lon = "112.09462";  //PHX
			std::string prefix = "$GPGGA,165125.568,";
			std::string suffix = "333,00,,,M,0.0,M,,0000*99";
			std::string rando = std::to_string(rand() % 10);
			std::string sentence = prefix + lat + rando + ",N," + lon + rando + ",W," + suffix;
			std::cout << sentence << std::endl;
			
			sleep(1);
		}

	} else {
		// Create serial port object 
		SerialPort serialPort("/dev/ttyUSB" + std::to_string(gpsport), BaudRate::B_4800, NumDataBits::EIGHT, Parity::NONE, NumStopBits::ONE);
		serialPort.SetTimeout(-1); // Block when reading until any data is received
		
		std::cout << "opening serial port" << std::endl;
		
		serialPort.Open();

		std::cout << "waiting for first endline character" << std::endl;
		
		int length = 0;
		char char_array[1028];
		// Read some data back (will block until at least 1 byte is received due to the SetTimeout(-1) call above).
		// TODO: add timer to allow graceful exit from while(1)
		// TODO: use udev library to detect device removal
		while (1) {
			std::string readData;
			serialPort.Read(readData);
			strcpy(char_array, readData.c_str());

			// look for endline trigger and take action when found
			length = sizeof(readData);
			for (int i = 0; i < length; i++) {
				if (char_array[i] == '\n') {
					// do action(s) here
					std::cout << std::endl;
					char_array[0] = '\0';  // fix issue with printing newline with every char by clearing buffer

                    // Create data sample for writing
                    Position posn;
                    for (unsigned int count = 0;
                        !shutdown_requested && count < sample_count;
                        count++) {
                        // Modify the data to be written here
                        //posn.msg("Hello GPS World! " + std::to_string(count));
                        posn.providerID(1);
                        posn.lat(12.34567);
                        posn.lon(123.45678);
                        if (simulation_mode) {
                            std::cout << "Writing fake GPS, count " << count << std::endl;
                        } else {
                            std::cout << "Writing new GPS, count " << count << std::endl;
                        }
                        writer.write(posn);

                        rti::util::sleep(dds::core::Duration(4));
                    }


				}
			}
			
			std::cout << readData;
		}
		
		// Close the serial port
		serialPort.Close();

	}

}

int main(int argc, char *argv[])
{
    // Parse arguments and handle control-C
    auto arguments = parse_arguments(argc, argv);
    if (arguments.parse_result == ParseReturn::exit) {
        return EXIT_SUCCESS;
    } else if (arguments.parse_result == ParseReturn::failure) {
        return EXIT_FAILURE;
    }
    setup_signal_handlers();

    std::cout << "GPS publisher example version " << std::to_string(VERSION) << std::endl;
    std::cout << "ProviderID = " << std::to_string(arguments.provider_id) << std::endl;
	if (arguments.simulation_mode) {
		std::cout << "Simulation Mode = ON" << std::endl;
	} else {
        std::cout << "Using port /dev/ttyUSB" << std::to_string(arguments.gpsport) << std::endl;
 	}    
    
    

    // Sets Connext verbosity to help debugging
    rti::config::Logger::instance().verbosity(arguments.verbosity);

    try {
        run_example(arguments.domain_id, arguments.sample_count, arguments.simulation_mode, arguments.gpsport);
    } catch (const std::exception& ex) {
        // This will catch DDS exceptions
        std::cerr << "Exception in run_example(): " << ex.what()
                  << std::endl;
        return EXIT_FAILURE;
    }

    // Releases the memory used by the participant factory.  Optional at
    // application shutdown
    dds::domain::DomainParticipant::finalize_participant_factory();

    return EXIT_SUCCESS;
}
