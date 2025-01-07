#ifndef TIME_H
#define TIME_H

#include <iostream>

/**
 * @brief Manage time with hour and minute
 *
 */
class Time {
  private:
    /**
     * @brief hour of the time
     *
     */
    int hour;

    /**
     * @brief minute of the time
     *
     */
    int minute;

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
    int getHour() const;

    /**
     * @return int the minute of the time
     */
    int getMinute() const;

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
    void setHour(int hour);

    /**
     * @param minute The minute of the time
     */
    void setMinute(int minute);


    /**
     * @brief Compare two Time objects
     *
     * @param time The time object
     * @return true if the two Time objects are equal
     * @return false if the two Time objects are not equal
     */
    bool operator==(const Time &time) const;

    /**
     * @brief Compare two Time objects
     *
     * @param time The time object
     * @return true if the two Time objects are not equal
     * @return false if the two Time objects are equal
     */
    bool operator!=(const Time &time) const;

    /**
     * @brief Compare two Time objects
     *
     * @param time The time object
     * @return true if the first Time object is less than the second Time object
     * @return false if the first Time object is not less than the second Time object
     */
    bool operator<(const Time &time) const;

    /**
     * @brief Compare two Time objects
     *
     * @param time The time object
     * @return true if the first Time object is greater than the second Time object
     * @return false if the first Time object is not greater than the second Time object
     */
    bool operator>(const Time &time) const;

    /**
     * @brief Add two Time objects
     *
     * @param time The time object
     * @return Time object
     */
    Time operator+(const Time &time) const;

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

        /**
         * @brief Subtract two Time objects
         * 
         * @param time The time object
         * @return Time object
         */
        Time operator-(const Time &time) const;

        //>=
        bool operator>=(const Time &time) const;
        //<=
        bool operator<=(const Time &time) const;
    /**
    * @brief Output the Time object to the output stream
    *
    * @param out The output stream
    * @param time The Time object
    * @return std::ostream& the output stream
    */
    friend std::ostream& operator<<(std::ostream &out, const Time &time);

    /**
     * @brief Input the Time object from the input stream
     *
     * @param in The input stream
     * @param time The Time object
     * @return std::istream& the input stream
     */
    friend std::istream& operator>>(std::istream &in, Time &time);
};




#endif