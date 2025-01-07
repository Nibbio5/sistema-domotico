// Eros Menin

#ifndef CPDEVICE_H
#define CPDEVICE_H

#include "device.h"

/**
 * @brief Subclass of Device used to define a domotic device which has a duration that
 * defines for how much time the device stays active before shutting off.
 * The device is manually turned on by the user or automatically according to the start time.
 *
 */
class CPDevice: public Device {
  public:
    /**
     * @brief The amount of time the devices has to work before shutting off
     *
     */
    const Time KDuration;

    /**
     * @brief Construct a new CPDevice object
     *
     * @param name the name of the device
     * @param power the power consumption (negative) or production (positive) of the device
     * @param duration the amount of time the devices has to work before shutting off
     * @param startTime the time point at which the devices turns on
     */
    CPDevice(const std::string& name, const double power, const Time& duration, const Time& start_time);

    /**
     * @brief Construct a new CPDevice object without a start time
     *
     * @param name the name of the device
     * @param power the power consumption (negative) or production (positive) of the device
     * @param duration the amount of time the devices has to work before shutting off
     */
    CPDevice(const std::string& name, const double power, const Time& duration);

    /**
     * @brief Set the start Time object
     *
     * @param newTime the new time for the start Time object
     */
    void set_start_time(const Time& new_time);

    /**
     * @brief Deletes the start timer
     *
     */
    void reset_start_time();

    /**
     * @brief Output the CPDevice object to the output stream.
     *
     * The data injected in the ostream object contains the class name and a list of all the
     * class variables with the respective value.
     *
     * @param out the stream where the data are injected
     * @param device the CPDevice object which provides the data to inject
     * @return std::ostream& the given output stream
     */
    friend std::ostream& operator<<(std::ostream& out, const CPDevice& device);
};

#endif