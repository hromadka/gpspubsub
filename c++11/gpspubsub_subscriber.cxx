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

#include <algorithm>
#include <iostream>

#include <dds/sub/ddssub.hpp>
#include <dds/core/ddscore.hpp>
#include <rti/config/Logger.hpp>  // for logging

#include "gpspubsub.hpp"
#include "application.hpp"  // Argument parsing

using namespace application;

const int BEST_PROVIDERID = 1;      // hard-coded for this demo only
const int FAILOVER_PROVIDERID = 2;  // hard-coded for this demo only
int counts_since_last_heard_best_provider = 0; 
int g_timeout = 5;

unsigned int process_data(dds::sub::DataReader<Position>& reader)
{
    // Take all samples.  Samples are loaned to application, loan is
    // returned when LoanedSamples destructor called.
    unsigned int samples_read = 0;
    dds::sub::LoanedSamples<Position> samples = reader.take();
    for (const auto& sample : samples) {
        if (sample.info().valid()) {
            samples_read++;
            if (sample.data().providerID() == BEST_PROVIDERID) { 
                std::cout << sample.data() << " *PRIMARY_SOURCE" << std::endl;
                counts_since_last_heard_best_provider = 0;
            } else if (counts_since_last_heard_best_provider > g_timeout && sample.data().providerID() == FAILOVER_PROVIDERID)  {
                std::cout << sample.data() << " *SECONDARY_SOURCE-->failover" << std::endl;
                counts_since_last_heard_best_provider++;
            } else if (counts_since_last_heard_best_provider < g_timeout) {
                // print nothing additional
                counts_since_last_heard_best_provider++;
            } else {
                std::cout << sample.data() << " *UNRECOGNIZED_SOURCE-->filter" << std::endl;
                counts_since_last_heard_best_provider++;
            }

        }
    }


    return samples_read;
}

void run_example(unsigned int domain_id, unsigned int sample_count, int timeout)
{
    // A DomainParticipant allows an application to begin communicating in
    // a DDS domain. Typically there is one DomainParticipant per application.
    // Create a DomainParticipant with default Qos
    dds::domain::DomainParticipant participant(domain_id);

    dds::topic::Topic<Position> topic(participant, "GPS Topic");

    // A Subscriber allows an application to create one or more DataReaders
    // Subscriber QoS is configured in USER_QOS_PROFILES.xml
    dds::sub::Subscriber subscriber(participant);

    // DataReader QoS is configured in USER_QOS_PROFILES.xml
    dds::sub::DataReader<Position> reader(subscriber, topic);

    // Obtain the DataReader's Status Condition
    dds::core::cond::StatusCondition status_condition(reader);

    // Enable the 'data available' status.
    status_condition.enabled_statuses(
            dds::core::status::StatusMask::data_available());

    // Associate a handler with the status condition. This will run when the
    // condition is triggered, in the context of the dispatch call (see below)
    unsigned int samples_read = 0;
    g_timeout = timeout;
    status_condition.extensions().handler([&reader, &samples_read]() {
        samples_read += process_data(reader);
    });

    // Create a WaitSet and attach the StatusCondition
    dds::core::cond::WaitSet waitset;
    waitset += status_condition;

    while (!shutdown_requested && samples_read < sample_count) {
        // Dispatch will call the handlers associated to the WaitSet conditions
        // when they activate
        std::cout << "GPSPubSub subscriber sleeping for 1 sec..." << std::endl;

        waitset.dispatch(dds::core::Duration(1));  
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

    // Sets Connext verbosity to help debugging
    rti::config::Logger::instance().verbosity(arguments.verbosity);

    try {
        run_example(arguments.domain_id, arguments.sample_count, arguments.timeout);
    } catch (const std::exception& ex) {
        // All DDS exceptions inherit from std::exception
        std::cerr << "Exception in run_example(): " << ex.what()
                  << std::endl;
        return EXIT_FAILURE;
    }

    // Releases the memory used by the participant factory.  Optional at
    // application shutdown
    dds::domain::DomainParticipant::finalize_participant_factory();

    return EXIT_SUCCESS;
}
