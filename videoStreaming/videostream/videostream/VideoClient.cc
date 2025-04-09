#include <omnetpp.h>

using namespace omnetpp;

class VideoClient : public cSimpleModule{

    private :
        double bufferThreshold;     //播放緩衝區的門檻
        double totalBuffer;         // 當前累積的緩衝時間
        int segmentsReceived;       //以接收影分段數量

    protected:
        //初始化方法，設定參數跟狀態

        virtual void initialize() override{

            //從NED參數抓取

            bufferThreshold = par("bufferThreshold");
            totalBuffer = 0 ;
            segmentsReceived = 0;
        }

        //處理收到的訊息

        virtual void handleMessage(cMessage *msg) override{
            // 假設每個影片分段代表一定長度的影片，如 2 秒
            double segmentDuration = 2;

            //當收到影片分段時，累加緩衝區時間

            totalBuffer += segmentDuration;
            segmentsReceived++ ;

            //輸出日誌
            EV << "時間" << simTime() << "接收到第" << segmentsReceived << "個分段，累計緩衝 = " << totalBuffer << "秒\n";

            if(totalBuffer > bufferThreshold){
                EV << "緩衝區充足(可考慮提高畫質)";

            }else{
                EV << "ˇ緩衝區不足(可能需要降低畫質)";
            }

            //模擬播放過程:假收每收到一個分段後會撥放1秒的影片，所以緩衝區少1秒

            if (totalBuffer >= 1)
                totalBuffer -= 1;
            else
                totalBuffer = 0;


            delete msg;
        }

};

Define_Module(VideoClient);
