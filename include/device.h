// Eros Menin

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
class Device {
public:
  /**
   * @brief The id of the device, managed with a singleton in order to be an
   * unique identifier
   *
   */
  const int id;

  /**
   * @brief The name of the device
   *
   */
  const std::string name;

  /**
   * @brief The power of the device measured in kW.
   *
   * The value indicates a power consumption if negative, and a
   *
   */
  const double power;

  /**
   * @brief Destroy the Device object
   *
   */
  virtual ~Device() = default;

        /**
         * @brief Get the Time object which defines the activty start time
         * 
         * @return Time 
         */
        std::shared_ptr<const Time> getStartTime() const;

  /**
   * @brief Checks whether the device is active
   *
   * @return true if the device is on
   * @return false if the device is off
   */
  bool isDeviceOn() const;

  /**
   * @brief Activate the device
   *
   */
  void switchOn();

  /**
   * @brief Deactivate the device
   *
   */
  void switchOff();

        
    protected:
        /**
         * @brief Stores the state of the device.
         * 
         * true (1) if the device is on, false (0) if the device is off.
         * 
         */
        bool isOn;

        /**
         * @brief The time point at which the devices turns on and starts its activity.
         * 
         */
        std::shared_ptr<Time> startTime;

        /**
         * @brief Construct a new Device object
         * 
         * @param name the name of the device
         * @param power the power consumption (negative) or production (positive) of the device
         * @param startTime the time point at which the devices turns on
         */
        Device(const std::string& name, const double power, const Time& startTime);
        
        /**
         * @brief Construct a new Device object without a start Time object
         * 
         * @param name the name of the device
         * @param power the power consumption (negative) or production (positive) of the device
         */
        Device(const std::string& name, const double power);

  /**
   * @brief Copy constuctor disabled for base class
   *
   * @param device the Device to use for the copy
   */
  Device(const Device &device) = delete;

  /**
   * @brief Copy assigmnet disabled for base class
   *
   * @param device the Device to use for the copy
   * @return Device& the given device
   */
  Device &operator=(const Device &device) = delete;
};

/**
 * @brief Output the Device object to the output stream.
 *
 * The data injected in the ostream object contains the class name and a list of
 * all the class variables with the respective value.
 *
 * @param out the stream where the data are injected
 * @param device the Device object which provides the data to inject
 * @return std::ostream& the given output stream
 */
std::ostream &operator<<(std::ostream &out, const Device &device);

#endif
