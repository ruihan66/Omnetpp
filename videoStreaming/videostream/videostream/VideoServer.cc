#include <omnetpp.h>

using namespace omnetpp;

class VideoServer : public cSimpleModule{

    private:
    // 產生影片分段數量，分段總數，產生間隔，影片品質標記
        int segmentsSent;
        int totalSegments;
        double segmentInterval;
        std::string quality;
    protected:
        // 模組初始化 : 在此設定初始變數與排程第一個事件

        virtual void initialize() override{
            segmentsSent = 0 ;
            totalSegments = par("numSegments");
            segmentInterval = par("segmentInterval");
            quality = par("quality").stringValue();

            //立即排程第一個產生分段的事件，利用內部 message觸發 hendleMessage()
            scheduleAt(simTime(), new cMessage("generateSegment"));

        }
        //處理訊息事件:用來產生影片分段並排成下一個事件
        virtual void handleMessage(cMessage *msg) override{
            if (segmentsSent < totalSegments){

                //建立一個影片分段訊息
                cMessage *segment = new cMessage("videoSegment");

                segment -> addPar("segmentId") = segmentsSent;
                segment -> addPar("quality") = quality.c_str();
                segment -> setTimestamp();

                send(segment, "out");
                segmentsSent++;

                //根據分段產生間隔排程下一個產生分段的事件
                scheduleAt(simTime() + segmentInterval, msg);
            }else {

                delete msg;
            }
        }

};

Define_Module(VideoServer);
