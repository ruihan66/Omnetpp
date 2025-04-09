#include <omnetpp.h>
#include <queue>

using namespace omnetpp;

class Router : public cSimpleModule{

    private:
        double linkDelay;           //服務延遲時間
        double dropProbability;     //丟包機率
        int queueCapacity;          //排隊容量
        std :: queue< cMessage*> packetQueue ;// 封包序列
    protected:
        //初始化方法，從NED檔案取得參數

        virtual void initialize() override{

            linkDelay = par("linkDelay");
            dropProbability = par("dropProbability");
            queueCapacity = par("queueCapacity");
        }

        //處理訊息的方法，用來模擬接收訊息丟包跟排隊服務

        virtual void handleMessage(cMessage *msg) override{
            //先檢查收到的訊息是否為自訊息(自己排程的服務完成事件)

            if (!msg -> isSelfMessage()){
                //模擬丟包
                if(uniform(0,1) < dropProbability){
                    EV << "Router 丟棄封包，時間" << simTime() << "\n";
                    delete msg;

                }else{
                    //若沒丟包檢查序列還有沒有空間

                    if ((int)packetQueue.size() < queueCapacity){
                        //將訊息加入
                        packetQueue.push(msg);
                        scheduleAt(simTime() + linkDelay, new cMessage("serverPacket"));
                    }else{
                        EV << "Router 序列已滿，丟棄封包，時間:" << simTime() << "\n";
                        delete msg;
                    }
                }
            } else{
                //若是收到的是自訊息
                if(!packetQueue.empty()){
                    cMessage *pkt = packetQueue.front();
                    packetQueue.pop();

                    send(pkt, "out");
                }
                delete msg;
            }
        }
};

Define_Module(Router);
