//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

package videostream;

simple VideoServer
{
    
    parameters:
        //產生影片分段的間隔時間(秒)
        //影片分段總數
        //分段預設品質
        
        double segmentInterval @unit(s);                
        int numSegments;                
        string quality ;
    
    gates:
        
        // 模組的輸出門，將影片分段訊息送出
        output out;
    
}

simple Router
{

	parameters:
	    // 模擬連線傳輸時的延遲(秒)
	    //丟包機率，介於0 -1 之間，代表封包的丟失比率
	    //排隊容量限制
	    
	    double linkDelay @unit(s);	   
	    double dropProbability;	   
	    int queueCapacity;
	
	gates:
	    
	    // 輸入門:接收來自上游模組的封包
	    // 輸出門:將轉發後的封包送出
	    
	    input in;
	    output out;
	    
}

simple VideoClient
{
	parameters:
	    
	    // 播放緩衝區的門檻值(秒)，用來模擬撥放時需要維持的最低緩衝秒數
	    double bufferThreshold @unit(s);
	    
	gates:
	    
	    //輸入門:接受影片分段訊息
	    input in;
}

network VideoStreamingNetwork
{
	
	submodules:
	    
	    //影片伺服器模組，負責產生影像片段
	    server : VideoServer {
	        
	    	parameters:
	    	    //每個兩秒產生一個影像片段，總共產生30個分段，品質為HD
	    	    segmentInterval = 2s;
	    	    numSegments = 30;
	    	    quality = "HD";
	    	    @display("p=100,100;i=block/server");
	    }
	    //路由器模組，模擬網路連線上的延遲，排隊與丟包行為
	    
	    router: Router{
	    	
	    	 parameters:
	    	    // 模擬傳送延遲為0.5秒，10%丟包率，容量限制10封包
	    	    linkDelay = 0.5s;
	    	    dropProbability = 0.1;
	    	    queueCapacity = 10;
	    	    @display("p=300,100;i=block/router");
	    }
	    // 影片用戶端模組，負責接收影片分段並模擬播放緩衝
        client: VideoClient {
            parameters:
                // 播放緩衝最低要求為 5 秒
                bufferThreshold = 5s;
                @display("p=500,100;i=block/client");
                
        }		    
	connections:
	    
	    //連線設定
	    server.out --> { delay = 10ms; } --> router.in;
	    router.out --> { delay = 10ms; } --> client.in;
	    
}

