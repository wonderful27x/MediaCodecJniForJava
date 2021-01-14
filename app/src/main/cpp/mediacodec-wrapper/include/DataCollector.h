//
// Created by 123 on 2020/12/18.
//
#ifndef CYBER_DATACOLLECTOR_H
#define CYBER_DATACOLLECTOR_H

#include <string>
#include <list>
#include <map>

using namespace std;

/**
 * @author wonderful
 * @date 2021-1-6
 * @version 1.0
 * @descreption 数据统计工具类
 */
class DataCollector{

public:

    DataCollector(string markName,long period_ms = 1000,bool on_off = true);

    virtual void capture(string key);

    virtual void startTrance();

    virtual const char* takeSample();

    string fetchPeriodInfo();

    void turnOn();

    void turnOff();

    bool isTurnOn();

protected:

    string markName;
    virtual int64_t systemMillisecondTime();

private:

    int64_t timeStart = 0;          //起始时间
    int64_t timeEnd = 0;            //终止时间
    int64_t timeContinue = 0;       //统计时长
    int64_t period = 0;             //takeSample的输出周期
    map<string,int> dataInfoPeriod; //统计数据集合
    string periodInfo = "";         //统计数据的字符串格式内容

    bool isTrance = false;
    bool TURN_ON = false;

    //统计对象标识，当有多个统计对象时可以区分它们
    static list<string> markNameList;

    virtual void reset();

    virtual string getPeriodInfo();

    int hasSameMark(string mark);
};

#endif //CYBER_DATACOLLECTOR_H

