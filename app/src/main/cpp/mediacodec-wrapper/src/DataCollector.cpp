//
// Created by 123 on 2020/12/31.
//
#include "DataCollector.h"

list<string> DataCollector:: markNameList;

/**
 * 构造函数
 * @param markName 标识
 * @param period_ms 统计周期，单位：毫秒
 * @param on_off 是否开启，可已在这里关闭统计，默认值：true
 */
DataCollector::DataCollector(string markName,long period_ms,bool on_off) {
    this->markName = markName;
    this->period = period_ms;
    this->TURN_ON = on_off;

    //如果创建了同名的markName,则使用时间戳区分
    int position = hasSameMark(this->markName);
    if(position != -1){
        struct timeval tv;
        gettimeofday(&tv, nullptr);
        int64_t millisecondTime = ((long long)tv.tv_sec) * 1000 + tv.tv_usec / 1000;
        this->markName += "$" + to_string(millisecondTime);
    }
    markNameList.push_back(this->markName);
}

/**
 * 统计一次，让统计的项加一
 * @param key 需要统计的数据名称/key
 */
void DataCollector::capture(string key) {
    if(dataInfoPeriod.count(key)){
        dataInfoPeriod[key] += 1;
    }else{
        dataInfoPeriod[key] = 1;
    }
}

/**
 * 开始收集数据，循环中使用
 */
void DataCollector::startTrance() {
    if(!TURN_ON)return;

    if (!isTrance){
        reset();
        timeStart = systemMillisecondTime();
        isTrance = true;
    }
}

/**
 * 采样输出，循环中使用
 * @return
 */
const char *DataCollector::takeSample() {

    if(!TURN_ON)return nullptr;

    int64_t currentTime = systemMillisecondTime();
    if((currentTime - timeStart) < period)return nullptr;

    timeEnd = currentTime;
    timeContinue = timeEnd - timeStart;

    isTrance = false;
    getPeriodInfo();
    return periodInfo.c_str();
}

void DataCollector::turnOn() {
    TURN_ON = true;
}

void DataCollector::turnOff() {
    TURN_ON = false;
}

bool DataCollector::isTurnOn() {
    return TURN_ON;
}

void DataCollector::reset() {
    timeStart = 0;
    timeEnd = 0;
    timeContinue = 0;

    map<string,int>::iterator it_p;
    for(it_p = dataInfoPeriod.begin();it_p != dataInfoPeriod.end();it_p++)
    {
        (*it_p).second = 0;
    }
}

/**
 * 将一个周期内的所有统计数据转成string
 * @return
 */
string DataCollector::getPeriodInfo() {
    periodInfo = "==========#" + markName + "#=====Period Info=====================\n";
    periodInfo += "起始时间：" + to_string(timeStart) + "\n";
    periodInfo += "终止时间：" + to_string(timeEnd) + "\n";
    periodInfo += "统计时长：" + to_string(timeContinue) + "ms\n";
    map<string,int> :: iterator it_p;
    for(it_p = dataInfoPeriod.begin(); it_p != dataInfoPeriod.end(); it_p++){
        periodInfo += (*it_p).first + ": " + to_string((*it_p).second) + " | ";
        periodInfo += to_string((*it_p).second  * 1000.0 / timeContinue) + "\n";
    }
    periodInfo += "==============Period end=================\n\n";
    return periodInfo;
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
