/**
 * @file time.h
 * @author Francesco Dotoli (francesco.dotoli@studenti.unipd.it)
 * @brief
 * @date 2024-12-22
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef TIME_H
#define TIME_H

#include <iostream>

/**
 * @brief Manage time with hour and minute
 *
 */
class Time {
  private:
    int hour_; ///< The hour of the time
    int minute_; ///< The minute of the time

  public:
    /**
     * @brief Default constructor of Time
     *
     */
    Time();

    /**
     * @brief Construct a new Time object
     *
     * @param hour The hour of the time
     * @param minute The minute of the time
     */
    Time(int hour, int minute);

    /**
     * @brief Copy constructor of Time
     *
     * @param time The time object
     */
    Time(const Time &time);

    /**
     * @brief Construct a new Time object from string
     *
     * @param time The time string
     * @return Time object
     */
    static Time fromString(const std::string &time);

    /**
     * @return int the hour of the time
     */
    int hour(void) const {
        return hour_;
    }

    /**
     * @return int the minute of the time
     */
    int minute() const {
        return minute_;
    }

    /**
     * @return std::string the hour of the time as string
     */
    std::string getHourString() const;

    /**
     * @return std::string the minute of the time as string
     */
    std::string getMinuteString() const;

    /**
     * @param hour The hour of the time
     */
    void set_hour(int hour);

    /**
     * @param minute The minute of the time
     */
    void set_minute(int minute);

    /**
     * @brief Assign a Time object to another Time object
     *
     * @param time The time object
     * @return Time& the assigned Time object
     */
    Time& operator=(const Time &time);

    /**
     * @brief Add a Time object to another Time object
     *
     * @param time The time object
     * @return Time& the added Time object
     */
    Time& operator+=(const Time &time);

};

/**
 * @brief Add two Time objects
 *
 * @param time The time object
 * @return Time object
 */
Time operator+(const Time &a, const Time &b);

/**
* @brief Subtract two Time objects
*
* @param time The time object
* @return Time object
*/
Time operator-(const Time &a, const Time &b);

/**
 * @brief Compare two Time objects
 *
 * @param time The time object
 * @return true if the two Time objects are equal
 * @return false if the two Time objects are not equal
 */
bool operator==(const Time &a, const Time &b);

/**
 * @brief Compare two Time objects
 *
 * @param time The time object
 * @return true if the two Time objects are not equal
 * @return false if the two Time objects are equal
 */
bool operator!=(const Time &a, const Time &b);

/**
 * @brief Compare two Time objects
 *
 * @param time The time object
 * @return true if the first Time object is less than the second Time object
 * @return false if the first Time object is not less than the second Time object
 */
bool operator<(const Time &a, const Time &b) ;

/**
 * @brief Compare two Time objects
 *
 * @param time The time object
 * @return true if the first Time object is greater than the second Time object
 * @return false if the first Time object is not greater than the second Time object
 */
bool operator>(const Time &a, const Time &b);

/**
 * @brief Compare two Time objects
 *
 * @param time The Time object
 * @return true if the first Time object is greater or equal to the second Time object
 * @return false if the first Time object is not greater than the second Time object
 */
bool operator>=(const Time &a, const Time &b);

/**
 * @brief Compare two Time objects
 *
 * @param time The Time object
 * @return true if the first Time object is not greater or equal to the second Time object
 * @return false if the first Time object is greater than the second Time object
 */
bool operator<=(const Time &a, const Time &b);

/**
* @brief Output the Time object to the output stream
*
* @param out The output stream
* @param time The Time object
* @return std::ostream& the output stream
*/
std::ostream& operator<<(std::ostream &out, const Time &time);

#endif
