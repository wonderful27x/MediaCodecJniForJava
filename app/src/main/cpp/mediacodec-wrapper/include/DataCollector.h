//
// Created by 123 on 2020/12/18.
//

#ifndef OMAFPLAYER_DATACOLLECTOR_H
#define OMAFPLAYER_DATACOLLECTOR_H

#include <string.h>
#include <list>
using namespace std;

class DataCollector{

public:

   DataCollector(string markName,bool on_off){
       int position = hasSameMark(markName);
       if(position != -1){
           markName += "$" + to_string(systemMillisecondTime());
       }
       markNameList.push_back(markName);
       this->markName = markName;
       this->TURN_ON = on_off;
   }

   void startTrance(){

       if(!TURN_ON)return;

       if (!isTrance){
           isTrance = true;
           reset();
           timeStart = systemMillisecondTime();
       }
   }

   void captureInputBuffTimes(){
       if(!TURN_ON)return;
       getInputBuffTimes++;
   }

   void captureInputBuffNum(){
       if(!TURN_ON)return;
       getInputBuffNum++;
   }

   void captureQueueInputBuffTimes(){
       if(!TURN_ON)return;
       queueInputBuffTimes++;
   }

   void captureQueueInputBuffNum(){
       if(!TURN_ON)return;
       queueInputBuffNum++;
       lastQueueTime = queueTime;
       queueTime = systemMillisecondTime();
   }

   void captureDequeueOutputBuffTimes(){
       if(!TURN_ON)return;
       dequeueOutputBuffTimes++;
   }

   void captureDequeueOutputBuffNum(){
       if(!TURN_ON)return;
       dequeueOutputBuffNum++;
       lastDequeTime = dequeTime;
       dequeTime = systemMillisecondTime();
   }

   const char* takeSample(long period,long frequency){

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
           queueDelayTime = queueTime - lastQueueTime;
           dequeDelayTime = dequeTime - lastDequeTime;
           getFrequencyInfo();
           return frequencyInfo.c_str();
       } else{
           queueFps = queueInputBuffNum * 1000.0 / timeContinue;
           dequeueFps = dequeueOutputBuffNum * 1000.0 / timeContinue;
           getPeriodInfo();
           isTrance = false;
           return periodInfo.c_str();
       }
   }

   void turnOn(){
       TURN_ON = true;
   }

   void turnOff(){
       TURN_ON = false;
   }

private:
   int64_t timeStart = 0;          //起始时间
   int64_t timeEnd = 0;            //终止时间
   int64_t timeContinue = 0;       //统计时长
   int getInputBuffTimes = 0;      //inputBuff的次数
   int getInputBuffNum = 0;        //inputBuff成功的数量
   int queueInputBuffTimes = 0;    //queueInputBuff的次数
   int queueInputBuffNum = 0;      //queueInputBuff成功的数量
   int dequeueOutputBuffTimes = 0; //dequeOutputBuff的次数
   int dequeueOutputBuffNum = 0;   //dequeOutputBuff成功的数量
   float queueFps = 0.0f;          //入队帧率-基于成功
   float dequeueFps = 0.0f;        //出队帧率-基于成功
   int64_t queueTime = 0;          //入队时间-基于成功
   int64_t lastQueueTime = 0;      //上次入队时间
   int64_t queueDelayTime = 0;     //入队时延
   int64_t dequeTime = 0;          //出队时间-基于成功
   int64_t lastDequeTime = 0;      //上次出队时间
   int64_t dequeDelayTime = 0;     //出队时延
   string periodInfo = "";
   string frequencyInfo = "";
   bool isTrance = false;
   bool TURN_ON = false;
   string markName;
   static list<string> markNameList;

   void reset(){
       timeStart = 0;
       timeEnd = 0;
       timeContinue = 0;
       getInputBuffTimes = 0;
       getInputBuffNum = 0;
       queueInputBuffTimes = 0;
       queueInputBuffNum = 0;
       dequeueOutputBuffTimes = 0;
       dequeueOutputBuffNum = 0;
       queueFps = 0.0f;
       dequeueFps = 0.0f;
       periodInfo = "";
       frequencyInfo = "";
   }

   void getPeriodInfo(){
       periodInfo = "==========#" + markName + "#=====Period Info=====================\n";
       periodInfo += "起始时间：" + to_string(timeStart) + "\n";
       periodInfo += "终止时间：" + to_string(timeEnd) + "\n";
       periodInfo += "统计时长：" + to_string(timeContinue) + "ms\n";
       periodInfo += "getInputBuff的次数：" + to_string(getInputBuffTimes) + "\n";
       periodInfo += "getInputBuff成功的数量：" + to_string(getInputBuffNum) + "\n";
       periodInfo += "queueInputBuff的次数：" + to_string(queueInputBuffTimes) + "\n";
       periodInfo += "queueInputBuff成功的数量：" + to_string(queueInputBuffNum) + "\n";
       periodInfo += "dequeOutputBuff的次数：" + to_string(dequeueOutputBuffTimes) + "\n";
       periodInfo += "dequeOutputBuff成功的数量：" + to_string(dequeueOutputBuffNum) + "\n";
       periodInfo += "入队帧率：" + to_string(queueFps) + "\n";
       periodInfo += "出队帧率：" + to_string(dequeueFps) + "\n";
   }

   void getFrequencyInfo(){
       frequencyInfo = "==========#" + markName + "#=====Frequency Info=====================\n";
       frequencyInfo += "统计时长：" + to_string(timeContinue) + "\n";
       frequencyInfo += "入队时间：" + to_string(queueTime) + "\n";
       frequencyInfo += "上次入队时间：" + to_string(lastQueueTime) + "\n";
       frequencyInfo += "入队时延：" + to_string(queueDelayTime) + "ms\n";
       frequencyInfo += "出队时间：" + to_string(dequeTime) + "\n";
       frequencyInfo += "上次出队时间：" + to_string(lastDequeTime) + "\n";
       frequencyInfo += "出队时延：" + to_string(dequeDelayTime) + "ms\n";
   }

   int64_t systemMillisecondTime() {
       struct timeval tv;
       gettimeofday(&tv, NULL);
       return ((long long)tv.tv_sec) * 1000 + tv.tv_usec / 1000;
   }

   int hasSameMark(string mark){
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
};

#endif //OMAFPLAYER_DATACOLLECTOR_H
