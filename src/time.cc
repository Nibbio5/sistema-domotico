/**
 * @file time.cc
 * @author Francesco Dotoli (francesco.dotoli@studenti.unipd.it)
 * @brief Implementation of time class
 * @date 2025-01-13
 * 
 */
#include "../include/time.h"

Time::Time() : hour_(0), minute_(0) {}

Time::Time(int hour, int minute) : hour_(hour), minute_(minute) {}

Time::Time(const Time &time) : Time(time.hour(), time.minute()) {}

void Time::set_hour(int hour) {
    this->hour_ = hour;
}

void Time::set_minute(int minute) {
    this->minute_ = minute;
}

std::string Time::getHourString() const {
    std::string hourString = std::to_string(hour_);
    if(hourString.length() == 1) {
        hourString = "0" + hourString;
    }
    return hourString;
}

std::string Time::getMinuteString() const {
    std::string minuteString = std::to_string(minute_);
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
        throw std::invalid_argument("Formato del tempo non valido (hh:mm)");
    }

    try {
        std::string hour_str = tmpTime.substr(pos - 2, 2);
        std::string minute_str = tmpTime.substr(pos + 1, 2);
        int hour = std::stoi(hour_str);
        int minute = std::stoi(minute_str);
        if(hour < 0 || hour > 23 || minute < 0 || minute > 59) {
            throw std::invalid_argument("Formato del tempo non valido (hh:mm)");
        }
        return Time(hour, minute);

    } catch(const std::invalid_argument &e) {
        throw std::invalid_argument("Formato del tempo non valido (hh:mm)");
    } catch(const std::out_of_range &e) {
        throw std::invalid_argument("Formato del tempo non valido (hh:mm)");
    }
}

bool operator==(const Time &a, const Time &b) {
    return  a.hour() == b.hour() && a.minute() == b.minute();
}

bool operator!=(const Time &a, const Time &b) {
    return a.hour() != b.hour() || a.minute() != b.minute();
}

bool operator<(const Time &a, const Time &b) {
    if(a.hour() < b.hour()) {
        return true;
    } else if(a.hour() == b.hour()) {
        return a.minute() < b.minute();
    }
    return false;
}

bool operator>(const Time &a, const Time &b) {
    if(a.hour() > b.hour()) {
        return true;
    } else if(a.hour() == b.hour()) {
        return a.minute() > b.minute();
    }
    return false;
}

bool operator<=(const Time &a, const Time &b) {
    return a < b || a == b;
}

bool operator>=(const Time &a, const Time &b) {
    return a > b || a == b;
}

Time operator+(const Time &a, const Time &b) {
    int newHour = a.hour() + b.hour();
    int newMinute = a.minute() + b.minute();
    if(newMinute >= 60) {
        newHour++;
        newMinute -= 60;
    }
    if(newHour >= 24) {
        newHour -= 24;
    }
    return Time(newHour, newMinute);
}

Time operator-(const Time &a, const Time &b) {
    if(a < b) {
        throw std::invalid_argument("Sottrazione di tempi non valida");
    }

    int newHour = a.hour() - b.hour();
    int newMinute = a.minute() - b.minute();
    if(newMinute < 0) {
        newHour--;
        newMinute += 60;
    }
    if(newHour < 0) {
        newHour += 24;
    }
    return Time(newHour, newMinute);
}

Time& Time::operator=(const Time &time) {
    if(this == &time) {
        return *this;
    }
    hour_ = time.hour();
    minute_ = time.minute();
    return *this;
}

Time& Time::operator+=(const Time &time) {
    if(*this > time) {
        throw std::invalid_argument("Addizione di tempi non valida");
    }
    hour_ += time.hour();
    minute_ += time.minute();
    if(minute_ >= 60) {
        hour_++;
        minute_ -= 60;
    }
    if(hour_ >= 24) {
        hour_ -= 24;
    }
    return *this;
}


std::ostream& operator<<(std::ostream &out, const Time &time) {
    out << time.getHourString() << ":" << time.getMinuteString();
    return out;
}