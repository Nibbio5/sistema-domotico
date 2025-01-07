#include "../include/time.h"
#include <iostream>
#include <string>

Time::Time() : hour(0), minute(0) {}

Time::Time(int hour, int minute) : hour(hour), minute(minute) {}

Time::Time(const Time &time) : Time(time.hour, time.minute) {}

int Time::getHour() const {
    return hour;
}

int Time::getMinute() const {
    return minute;
}

void Time::setHour(int hour) {
    this->hour = hour;
}

void Time::setMinute(int minute) {
    this->minute = minute;
}

std::string Time::getHourString() const {
    std::string hourString = std::to_string(hour);
    if(hourString.length() == 1) {
        hourString = "0" + hourString;
    }
    return hourString;
}

std::string Time::getMinuteString() const {
    std::string minuteString = std::to_string(minute);
    if(minuteString.length() == 1) {
        minuteString = "0" + minuteString;
    }
    return minuteString;
}

Time Time::fromString(const std::string &time) {
    std::string delimiter = ":";
    std::string token;
    std::string tmpTime = time;
    size_t pos = tmpTime.find(delimiter);

    if(pos == std::string::npos) {
        throw std::invalid_argument("Invalid time format");
    }
    int hour = std::stoi(tmpTime.substr(pos - 2, pos));
    int minute = std::stoi(tmpTime.substr(pos + 1, 2));
    if(hour < 0 || hour > 23 || minute < 0 || minute > 59) {
        throw std::invalid_argument("Invalid time format");
    }

    return Time(hour, minute);
}

bool Time::operator==(const Time &time) const {
    return hour == time.hour && minute == time.minute;
}

bool Time::operator!=(const Time &time) const {
    return hour != time.hour || minute != time.minute;
}

bool Time::operator<(const Time &time) const {
    if(hour < time.hour) {
        return true;
    } else if(hour == time.hour) {
        return minute < time.minute;
    }
    return false;
}

bool Time::operator>(const Time &time) const {
    if(hour > time.hour) {
        return true;
    } else if(hour == time.hour) {
        return minute > time.minute;
    }
    return false;
}

Time Time::operator+(const Time &time) const {
    if(*this > time) {
        throw std::invalid_argument("Invalid time addition");
    }
    int newHour = hour + time.hour;
    int newMinute = minute + time.minute;
    if(newMinute >= 60) {
        newHour++;
        newMinute -= 60;
    }
    if(newHour >= 24) {
        newHour -= 24;
    }
    return Time(newHour, newMinute);
}

Time& Time::operator=(const Time &time) {
    if (this == &time) {
        return *this;
    }
    hour = time.hour;
    minute = time.minute;
    return *this;
}

Time& Time::operator+=(const Time &time) {
    if(*this > time) {
        throw std::invalid_argument("Invalid time addition");
    }
    hour += time.hour;
    minute += time.minute;
    if(minute >= 60) {
        hour++;
        minute -= 60;
    }
    if(hour >= 24) {
        hour -= 24;
    }
    return *this;
}

Time Time::operator-(const Time &time) const {
    if(*this < time) {
        throw std::invalid_argument("Invalid time subtraction");
    }
    int newHour = hour - time.hour;
    int newMinute = minute - time.minute;
    if(newMinute < 0) {
        newHour--;
        newMinute += 60;
    }
    if(newHour < 0) {
        newHour += 24;
    }
    return Time(newHour, newMinute);
}

bool Time::operator>=(const Time &time) const {
    return *this > time || *this == time;
}

bool Time::operator<=(const Time &time) const {
    return *this < time || *this == time;
}

std::istream& operator>>(std::istream &in, Time &time) {
    std::string tmpTime;
    in >> tmpTime;
    time = Time::fromString(tmpTime);
    return in;
}

std::ostream& operator<<(std::ostream &out, const Time &time) {
    out << time.getHourString() << ":" << time.getMinuteString();
    return out;
}