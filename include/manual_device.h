// Eros Menin

#ifndef MANUALDEVICE_H
#define MANUALDEVICE_H

#include "device.h"

/**
 * @brief Subclass of Device used to define a domotic device which has a timer that
 * defines the time range in which it operates.
 *
 * The main validity condition for the ManualDevice object is that the start time point
 * is lower (comes before) than the stop time point. To ensure this condition is
 * always valid a check is performed every time the Time objects are updated.
 *
 */
class ManualDevice : public Device {
    public:
        /**
         * @brief Construct a new Manual Device object.
         *
         * Throws an exception if the start time is higher than the stop time.
         *
         * @param name the name of the device
         * @param power the power consumption (negative) or production (positive) of the device
         * @param startTime the time point at which the devices turns on
         * @param stopTime the time point at which the devices turns off
         */
        ManualDevice(const std::string& name, const double power, const Time& start_time, const Time& stop_time);

        ManualDevice(const std::string& name, const double power, const Time& start_time);

        ManualDevice(const std::string& name, const double power);

        /**
         * @brief Get the Time object which defines the activty stop time
         *
         * @return Time
         */
        std::shared_ptr<const Time> get_stop_time() const;

        /**
         * @brief Set the start time object.
         * 
         * If the new start Time object is higher than the current stop Time object, an
         * exception is thrown.
         * 
         * @param new_time the new time for the start Time object
         */
        void set_start_time(const Time& new_time);

        /**
         * @brief Set a new timer for the ManualDevice, modifing both the start and stop Time.
         *
         * Throws an exception if the start time is higher than the stop time.
         *
         * @param newStartTime the new time for the start Time object
         * @param newStopTime the new time for the stop Time object
         */
        void set_new_timer(const Time& new_start_time, const Time& new_stop_time);

        /**
         * @brief Removes the start and stop Time objects of this Device object
         * 
         */
        void removeTimer() override;

    private:
        /**
         * @brief The time point at which the devices turns off and stops its activity.
         * 
         */
        std::shared_ptr<Time> stop_time_;

        /**
         * @brief Output the ManualDevice object to the output stream.
         *
         * The data injected in the ostream object contains the class name and a list of all the
         * class variables with the respective value.
         *
         * @param out the stream where the data are injected
         * @param device the ManualDevice object which provides the data to inject
         * @return std::ostream& the given output stream
         */
        friend std::ostream& operator<<(std::ostream& out, const ManualDevice& device);    
};

#endif