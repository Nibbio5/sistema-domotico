// Eros Menin

#ifndef MANUALDEVICE_H
#define MANUALDEVICE_H

#include "device.h"

/**
 * @brief Subclass of Device used to define a domotic device which has a timer that
 * defines the time range in which it operates.
 *
 * The device has to be manually turned on by the user or automatically (by the system) 
 * according to the start time.
 * 
 * The main validity condition for the ManualDevice object is that the start time point
 * is lower (comes before) than the stop time point. To ensure this condition is
 * always valid a check is performed every time the Time objects are updated.
 * If the checks fails a std::invalid_argument exception is thrown.
 *
 */
class ManualDevice : public Device {
    public:
        /**
         * @brief Construct a new ManualDevice object.
         *
         * Throw a std::invalid_argument exception if the start time is higher than the stop time.
         *
         * @param name the name of the device
         * @param power the power consumption (negative) or production (positive) of the device
         * @param start_time the time point at which the devices turns on
         * @param stop_time the time point at which the devices turns off
         * @param is_on_white_list whether the device is part of a white list of devices that won't 
         * be turned off in case of electrical overload (if possible).
         */
        ManualDevice(const std::string& name, const double power, const Time& start_time, const Time& stop_time, const bool is_on_white_list = false);
        
        /**
         * @brief Construct a new ManualDevice object without a stop time
         * 
         * @param name the name of the device
         * @param power the power consumption (negative) or production (positive) of the device
         * @param start_time the time point at which the devices turns on
         * @param is_on_white_list whether the device is part of a white list of devices that won't 
         * be turned off in case of electrical overload (if possible).
         */
        ManualDevice(const std::string& name, const double power, const Time& start_time, const bool is_on_white_list = false);

        /**
         * @brief Construct a new Manual Device object without both the start and stop time
         * 
         * @param name the name of the device
         * @param power the power consumption (negative) or production (positive) of the device
         */
        ManualDevice(const std::string& name, const double power);

        /**
         * @brief Return the Time object which defines the activty stop time
         *
         * @return The Time object
         */
        std::shared_ptr<const Time> get_stop_time() const;

        /**
         * @brief Set the start time object.
         * 
         * If the new start Time object is higher than the current stop Time object (if set), 
         * an exception is thrown.
         * 
         * @param new_time the new time for the start Time object
         */
        void set_start_time(const Time& new_time);

        /**
         * @brief Set a new timer for the ManualDevice, modifing both the start and stop Time.
         *
         * Throw a std::invalid_argument exception if the start time is higher than the stop time.
         *
         * @param new_start_time the new time for the start Time object
         * @param new_stop_time the new time for the stop Time object
         */
        void set_new_timer(const Time& new_start_time, const Time& new_stop_time);

        /**
         * @brief Remove the start and stop Time objects of this Device object by 
         * setting them to a nullptr.
         * 
         */
        void removeTimer() override;

    private:
        /**
         * @brief The time point at which the device turns off and stops its activity.
         * 
         */
        std::shared_ptr<Time> stop_time_ = nullptr;

        /**
         * @brief Output the ManualDevice object to the output stream.
         *
         * The data injected in the ostream object contains the class name and a list of
         * all the class variables with the respective value.
         * If the dynamic pointers to the Time objects hold a nullptr, the start and stop
         * time are "NOT_SET". 
         *
         * @param out the stream where the data are injected
         * @param device the ManualDevice object which provides the data to inject
         * @return std::ostream&: the given output stream
         */
        friend std::ostream& operator<<(std::ostream& out, const ManualDevice& device);    
};

#endif