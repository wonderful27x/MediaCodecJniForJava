//
// Created by 123 on 2020/12/18.
//


#ifndef CYBER_DATACOLLECTOR_H
#define CYBER_DATACOLLECTOR_H

#include <string>
#include <list>
#include <map>

using namespace std;

class DataCollector{

public:

    DataCollector(string markName, bool on_off);

    virtual void capture(string key);

    virtual void startTrance();

    virtual const char* takeSample(long period,long frequency);

    void turnOn();

    void turnOff();

private:
    int64_t timeStart = 0;          //起始时间
    int64_t timeEnd = 0;            //终止时间
    int64_t timeContinue = 0;       //统计时长

    map<string,int> dataInfoPeriod;
    map<string,int> dataInfoFrequency;

    string periodInfo = "";
    string frequencyInfo = "";

    bool isTrance = false;
    bool TURN_ON = false;

    string markName;
    static list<string> markNameList;

    virtual void reset();

    virtual void getPeriodInfo();

    virtual void getFrequencyInfo();

    virtual int64_t systemMillisecondTime();

    int hasSameMark(string mark);
};

#endif //CYBER_DATACOLLECTOR_H
