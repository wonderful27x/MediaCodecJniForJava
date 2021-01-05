//
// Created by 123 on 2020/12/31.
//
#include "DataCollector.h"

list<string> DataCollector:: markNameList;

DataCollector::DataCollector(string markName, bool on_off) {
    int position = hasSameMark(markName);
    if(position != -1){
        markName += "$" + to_string(systemMillisecondTime());
    }
    markNameList.push_back(markName);
    this->markName = markName;
    this->TURN_ON = on_off;
}

void DataCollector::capture(string key) {
    if(dataInfoPeriod.count(key)){
        dataInfoPeriod[key] += 1;
    }else{
        dataInfoPeriod[key] = 1;
    }

    if(dataInfoFrequency.count(key)){
        dataInfoFrequency[key] += 1;
    }else{
        dataInfoFrequency[key] = 1;
    }
}

void DataCollector::startTrance() {
    if(!TURN_ON)return;

    if (!isTrance){
        isTrance = true;
        reset();
        timeStart = systemMillisecondTime();
    }
}

const char *DataCollector::takeSample(long period, long frequency) {

    if(!TURN_ON)return nullptr;

    static int64_t startTime = systemMillisecondTime();
    int64_t currentTime = systemMillisecondTime();

    if((currentTime - startTime) < frequency){
        return nullptr;
    }
    startTime = currentTime;

    timeEnd = currentTime;
    timeContinue = timeEnd - timeStart;
    if (timeContinue < period) {
        getFrequencyInfo();
        return frequencyInfo.c_str();
    } else{
        isTrance = false;
        getPeriodInfo();
        return periodInfo.c_str();
    }
}

void DataCollector::turnOn() {
    TURN_ON = true;
}

void DataCollector::turnOff() {
    TURN_ON = false;
}

void DataCollector::reset() {
    timeStart = 0;
    timeEnd = 0;
    timeContinue = 0;
    periodInfo = "";
    frequencyInfo = "";

    map<string,int>::iterator it_p;
    for(it_p = dataInfoPeriod.begin();it_p != dataInfoPeriod.end();it_p++)
    {
        (*it_p).second = 0;
    }

    map<string,int>::iterator it_f;
    for(it_f = dataInfoPeriod.begin();it_f != dataInfoPeriod.end();it_f++)
    {
        (*it_f).second = 0;
    }
}

void DataCollector::getPeriodInfo() {
    periodInfo = "==========#" + markName + "#=====Period Info=====================\n";
    periodInfo += "起始时间：" + to_string(timeStart) + "\n";
    periodInfo += "终止时间：" + to_string(timeEnd) + "\n";
    periodInfo += "统计时长：" + to_string(timeContinue) + "ms\n";
    map<string,int> :: iterator it_p;
    for(it_p = dataInfoPeriod.begin(); it_p != dataInfoPeriod.end(); it_p++){
        periodInfo += (*it_p).first + ": " + to_string((*it_p).second) + "\n";
    }
}

void DataCollector::getFrequencyInfo() {
    frequencyInfo = "==========#" + markName + "#=====Frequency Info=====================\n";
    frequencyInfo += "统计时长：" + to_string(timeContinue) + "\n";
    map<string,int> :: iterator it_f;
    for(it_f = dataInfoFrequency.begin(); it_f != dataInfoFrequency.end(); it_f++){
        frequencyInfo += (*it_f).first + ": " + to_string((*it_f).second) + "\n";
    }
}

int DataCollector::hasSameMark(string mark) {
    int position = -1;
    list<string>::const_iterator iterator;
    for(iterator = markNameList.begin(); iterator != markNameList.end(); iterator++){
        position++;
        if(*iterator == mark) {
            return position;
        }
    }
    return -1;
}

int64_t DataCollector::systemMillisecondTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((long long)tv.tv_sec) * 1000 + tv.tv_usec / 1000;
}
