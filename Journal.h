#ifndef JOURNAL_H
#define JOURNAL_H

#endif // JOURNAL_H

#include<vector>
#include<iostream>

#include "Entry.h"
#include <QDate>
class Journal {


public:

    Journal(): entry_list_(), name_(){ }

    Journal(std::string name, std::vector<Entry>entry_list): entry_list_(entry_list), name_(name) { }

    Journal(std::string name): name_(name), entry_list_(){ }

    const std::vector<Entry>& GetEntries() const { return entry_list_; }   // ✅ for reading

    std::vector<Entry>& GetEntries() { return entry_list_; }

    std::string getName() const{
        return this->name_;
    }
    void setName(std::string name){
        this->name_ = name;
    }
    QDate getDate()const{
        return this->date_;
    }
    void setDate(QDate date){
        date_ = date;
    }

    void addEntry(Entry entry_param){

        entry_list_.push_back(entry_param);
    }
    void deleteEntry(Entry entry){
        for(std::vector<Entry>::iterator it = entry_list_.begin(); it!=entry_list_.end();it++ ){
            if(*it==entry){
                entry_list_.erase(it);
                    return;
            }
        }
    }
    void deleteEntry(int index){
        entry_list_.erase(entry_list_.begin()+index);
    }

    void deleteEntries(int index_start, int index_stop){ //probably will not need.
    entry_list_.erase(entry_list_.begin()+index_start, entry_list_.begin() + index_stop);
    }
    void setCurrentEntryIndex(int entry_index){
        this->current_entry_index_ = entry_index;
    }
    int getCurrentEntryIndex(){
        return this->current_entry_index_;
    }

private:
    std::string name_;
    std::vector<Entry> entry_list_;
    int current_entry_index_;
    QDate date_;
};
