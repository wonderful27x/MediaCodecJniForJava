//
// Created by 123 on 2020/12/18.
//
#ifndef CYBER_MEDIADATACOLLECTOR_H
#define CYBER_MEDIADATACOLLECTOR_H

#include "DataCollector.h"

struct DataElement_{
    int64_t lastTime;
    int64_t currentTime;
    int count;
};

typedef struct DataElement_ DataElement ;


/**
 * @author wonderful
 * @date 2021-1-6
 * @version 1.0
 * @descreption MediaCodec数据统计工具类
 */
class MediaDataCollector : public DataCollector{
public:
    MediaDataCollector(string markName,long period_ms = 1000,long delayPeriod_ms = 100,bool on_off = true):DataCollector(markName,period_ms,on_off){
        this->delayPeriod = delayPeriod_ms;
    }

    void capture(string key) override{
        DataCollector::capture(key);
        if(delayDataInfoPeriod.count(key)){
            //todo error operation like this:
            //DataElement element = delayDataInfoPeriod[key];
            //element.count++;
            delayDataInfoPeriod[key].count++;
            delayDataInfoPeriod[key].lastTime = delayDataInfoPeriod[key].currentTime;
            delayDataInfoPeriod[key].currentTime = systemMillisecondTime();
        }else{
            DataElement element;
            element.count = 1;
            element.lastTime = element.currentTime = systemMillisecondTime();
            delayDataInfoPeriod.insert(map<string,DataElement> :: value_type(key,element));
        }
    }

    const char * takeSample() override{
        if(!isTurnOn())return nullptr;

        if(delayTimeStart == 0){
            delayTimeStart = systemMillisecondTime();
        }
        delayTimeEnd = systemMillisecondTime();

        //计算时间，一个周期到时才输出
        string delayInfo = "";
        delayTimeContinue = delayTimeEnd - delayTimeStart;
        if(delayTimeContinue >= delayPeriod){
            delayInfo = getDelayPeriodInfo();
            delayTimeStart = delayTimeEnd;
            //统计数量清零
            clearDelayDataInfoPeriod();
        }

        //与父类统计合并一起输出
        const char* info = DataCollector::takeSample();
        if(info != nullptr){
            string infoString = info;
            delayInfo += infoString;
        }

        return delayInfo.empty() ? nullptr : delayInfo.c_str();
    }

    /**
     * 计数清零
     */
    void clearDelayDataInfoPeriod(){
        map<string,DataElement> :: iterator it_p;
        for(it_p = delayDataInfoPeriod.begin(); it_p != delayDataInfoPeriod.end(); it_p++){
            it_p->second.count = 0;
        }
    }

    string getDelayPeriodInfo() {
        delayPeriodInfo = "==========#" + markName + "#=====Period Delay Info=====================\n";
        delayPeriodInfo += "起始时间：" + to_string(delayTimeStart) + "\n";
        delayPeriodInfo += "终止时间：" + to_string(delayTimeEnd) + "\n";
        delayPeriodInfo += "统计时长：" + to_string(delayTimeContinue) + "ms\n";
        map<string,DataElement> :: iterator it_p;
        for(it_p = delayDataInfoPeriod.begin(); it_p != delayDataInfoPeriod.end(); it_p++){
            DataElement element = it_p->second;
            delayPeriodInfo += it_p->first + " capture num: " + to_string(element.count) + " | 时延: " + to_string(element.currentTime - element.lastTime) + "ms\n";
        }
        delayPeriodInfo += "==============Period Delay end=================\n\n";
        return delayPeriodInfo;
    }

private:
    int64_t delayTimeStart = 0;                  //时延起始时间
    int64_t delayTimeEnd = 0;                    //时延终止时间
    int64_t delayTimeContinue = 0;               //时延统计时长
    int64_t delayPeriod = 0;                     //takeSample的输出周期
    map<string,DataElement> delayDataInfoPeriod; //时延统计数据集合
    string delayPeriodInfo = "";                 //时延统计数据的字符串格式内容
};

#endif //CYBER_MEDIADATACOLLECTOR_H
