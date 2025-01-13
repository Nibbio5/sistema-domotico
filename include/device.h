/**
 * @file device.h
 * @author Eros Menin (eros.menin@studenti.unipd.it)
 * @brief Header file for the Device abstract class.
 * @date 2025-01-13
 * 
 */

#ifndef DEVICE_H
#define DEVICE_H

#include "time.h"
#include "singleton_id.h"
#include <string>
#include <ostream>
#include <memory>

/**
 * @brief Abstract class used to define the basic properties of a domotic device
 *
 */
class Device{
    public: 
        /**
         * @brief The id of the device, managed with a singleton in order to be an
         * unique identifier
         * 
         */
        const int KId;

        /**
         * @brief The name of the device
         * 
         */
        const std::string KName;

        /**
         * @brief The power of the device measured in kW/h.
         * 
         * The value indicates either an energy consumption if negative or a energy
         * production if positive.
         *  
         */
        const double KPower;

        /**
         * @brief Defines whether the device is part of a white list (true) or not (false).
         * 
         * The white list contains a list of devices that in case of electrical overload are not 
         * turned off, if possible.
         * 
         */
        const bool KIsOnWhiteList;

        /**
         * @brief Destroy the Device object
         *
         */
        virtual ~Device() = default;

        /**
         * @brief Return the Time object which defines the activity start time
         *
         * @return The Time object 
         */
        virtual std::shared_ptr<const Time> get_start_time() const;

        /**
         * @brief Set the start Time object
         * 
         * @param new_time the new time for the start Time object
         */
        void set_start_time(const Time& new_time);

        /**
         * @brief Return the Time object which defines the last time point at which 
         * the device was turned on.
         * 
         * If the device is turned off, this holds a nullptr.
         * 
         * @return std::shared_ptr<const Time>: the dynamic pointer to the Time object
         */
        std::shared_ptr<const Time> get_last_activation_time() const;

        /**
         * @brief Checks whether the device is active
         * 
         * @return true if the device is on
         * @return false if the device is off
         */
        bool is_on() const;

        /**
         * @brief Activate the device and updates the last activation Time object
         *
         */
        void switch_on(const Time& current_time);

        /**
         * @brief Deactivate the device and updates the total consumption/production
         * of power
         *
         */
        void switch_off(const Time& current_time);

        /**
         * @brief Return the total power consumerd/produced by the device
         * 
         * @param current_time the time point at which is required to calculate 
         * the power consumption/production
         * @return double: the total power consumed/produced until the given time
         */
        double get_total_power(const Time& current_time) const;

        /**
         * @brief Remove the start Time object of the device
         * 
         */
        virtual void removeTimer();

        /**
         * @brief Reset the amount of power consumed/produced and shut off the 
         * device. Doesn't remove the timer.
         * 
         */
        void resetDevice();

        /**
         * @brief Reset the amount of power consumed/produced, shut off the 
         * device and remove the timer.
         * 
         */
        void resetDeviceAndTimer();

        
    protected:
        /**
         * @brief Stores the state of the device.
         * 
         * true (1) if the device is on, false (0) if the device is off.
         * 
         */
        bool is_on_ = false;

        /**
         * @brief The total power consumed (negative) or produced (positive) by the device.
         * 
         * The values updates when the device is turned off.
         * 
         */
        double total_power_ = 0;

        /**
         * @brief The time point at which the devices turns on and starts its activity.
         * 
         */
        std::shared_ptr<Time> start_time_ = nullptr;

        /**
         * @brief The last time point at which the device was turned on
         * 
         */
        std::shared_ptr<Time> last_activation_time_ = nullptr;

        /**
         * @brief Construct a new Device object
         *
         * @param name the name of the device
         * @param power the power consumption (negative) or production (positive) of the device
         * @param start_time the time point at which the devices turns on
         * @param is_on_white_list whether the device is part of a white list of devices that won't 
         * be turned off in case of electrical overload (if possible).
         */
        Device(const std::string& name, const double power, const Time& start_time, const bool is_on_white_list = false);

        /**
         * @brief Construct a new Device object without a start Time object
         *
         * @param name the name of the device
         * @param power the power consumption (negative) or production (positive) of the device
         * @param is_on_white_list whether the device is part of a white list of devices that won't 
         * be turned off in case of electrical overload (if possible).
         */
        Device(const std::string& name, const double power, const bool is_on_white_list = false);

        /**
         * @brief Copy constuctor disabled for base class
         *
         * @param device the Device to use for the copy
         */
        Device(const Device &device) = delete;

        /**
         * @brief Copy assignment disabled for base class
         *
         * @param device the Device to use for the copy
         * @return Device&: the given device
         */
        Device &operator=(const Device &device) = delete;

        /**
         * @brief Output the Device object to the output stream.
         *
         * The data injected in the ostream object contains the class name and a list of
         * all the class variables with the respective value.
         * If the dynamic pointer to the Time object holds a nullptr, the start time
         * is "NOT_SET". 
         *
         * @param out the stream where the data are injected
         * @param device the Device object which provides the data to inject
         * @return std::ostream&: the given output stream
         */
        friend std::ostream &operator<<(std::ostream &out, const Device &device);
};

/**
 * @brief Calculates the amount of power consumed/produced by a device in a time period.
 * 
 * @param start_time the time point at which the device started its consumption/production
 * @param stop_time the time point at which the device stopped its consumption/production
 * @param power the power consumption/production is kW/h
 * @return double: the total power consumed/produced
 */
static double calculateTimePeriodPower(const Time& start_time, const Time& stop_time, const double power);

#endif