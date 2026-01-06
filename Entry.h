#ifndef ENTRY_H
#define ENTRY_H

#endif // ENTRY_H
#include<iostream>
#include<QDate>
#include<QTime>



class Entry {

public:
    Entry(){}

    Entry(std::string text, std::string mood, std::string weather, std::string description):
        text_(text),mood_(mood), weather_(weather), description_(description){
    this->date_ = QDate::currentDate();
    this->time_ = QTime::currentTime();
    }

    std::string getText()const{
        return text_;
    }
    void setText(std::string text){
        this->text_ = text;
    }

    QDate getQDate() const {
        return date_;
    }
    void setQDate(QDate date){
        date_ = date;
    }

    std::string getMood() const {
        return mood_;
    }
    void setMood(std::string mood){
        mood_ = mood;
    }

    std::string getWeather() const{
        return weather_;
    }
    void setWeather(std::string weather){
        weather_ = weather;
    }

    std::string getDescription() const {
        return description_;
    }
    void setDescription(std::string description){
        description_ = description;
    }
    QTime getQTime() const{
        return time_;
    }
    void setQTime(QTime time){
        time_ = time;
    }

    bool operator==(const Entry& other) const{
        if(this->text_ == other.text_
            && this->date_ == other.date_
            && this->weather_ == other.weather_
            && this->mood_ == other.mood_
            && this->description_ == other.description_){
            return true;
        }
        else{
            return false;
        }
    }


private:
    std::string text_;
    std::string mood_;
    std::string weather_;
    std::string description_;
    QDate date_;
    QTime time_;

};
