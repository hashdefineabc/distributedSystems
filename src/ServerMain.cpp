#include <iostream>
#include <thread>
#include <future>
#include <queue>

#include "ServerStub.cpp"
#include "LaptopInfo.h" 


struct Task {
    LaptopInfo *laptopInfo;
    std::promise<bool>* promise;
};

template <typename T>
class TaskMtxQueue {
    private:
        std::queue<T> queue;
        std::mutex mtx;
        std::condition_variable variable;
    
    public:
        void push(const T& value) {
            std::unique_lock<std::mutex> lock(mtx);
            queue.push(value);
            variable.notify_one();
        }

        T pop() {
            std::unique_lock<std::mutex> lock(mtx);
            variable.wait(lock, [this] {
                return !queue.empty();
            });
            T val = queue.front();
            queue.pop();
            return val;
        }

};

void ExpertThread(int expertId, TaskMtxQueue<Task> &queue) {
    while(true) {
        Task task = queue.pop();
        std::this_thread::sleep_for(std::chrono::microseconds(100));
        task.laptopInfo->expert_id = expertId;
        task.promise->set_value(true);
    }
}

void engineerThread(int engineerId, int expertId, TaskMtxQueue<Task> &queue, ServerStub server) {
    Order order;
    while(server.ReceiveOrder(order)) {

        LaptopInfo laptopInfo(order.customer_id, order.order_number, order.laptop_type, engineerId, expertId);
        if(order.laptop_type == 1) {
            std::promise<bool> promise;
            queue.push(Task{&laptopInfo, &promise});
            promise.get_future().get();
            //server.ShipLaptop(laptopInfo);
        }
        if(!server.ShipLaptop(laptopInfo)) {
            break;
        }
        // try{
        //     order = server.ReceiveOrder();
        //     if(order.MarshalOrder().empty()) {
        //         flag = false;
        //         break;
        //     }
        //     LaptopInfo laptopInfo(order.customer_id, order.order_number, order.laptop_type, engineerId, expertId);
        //     if(order.laptop_type == 1) {
        //         std::promise<bool> promise;
        //         queue.push(Task{&laptopInfo, &promise});
        //         promise.get_future().get();
        //         //server.ShipLaptop(laptopInfo);
        //     }
        //     server.ShipLaptop(laptopInfo);
        //     if(laptopInfo.MarshalLaptopInfo().empty()){
        //         break;
        //     }
        // }
        // catch(const SocketException& e) {
        //     std::cerr << "SocketException in engineer function: " << e.what() << std::endl;
        //     break;
        // }
    }
    std::cout<<"EngineerId " << engineerId << " completed" << std::endl;
}

int main(int argc, char* argv[]) {

    if (argc < 2)
    {
        std::cerr << "Usage " << argv[0] << " [port]" << std::endl;
        return 1;
    }

    int serverPort = atoi(argv[1]);

    int numExperts = 0;
    if(argc == 3) 
        numExperts = atoi(argv[2]);

    signal(SIGPIPE, SIG_IGN);


    Socket serverSocket = Socket::listen(serverPort);
    std::vector<std::thread> expertThreads;
    TaskMtxQueue<Task> taskQueue;
    int engineerId = 1;

    for(int expertNum = 1; expertNum <= numExperts; ++expertNum) {
        expertThreads.emplace_back(ExpertThread, expertNum, std::ref(taskQueue));
    }

    while (true)
    {
        try {
        Socket clientSocket = serverSocket.accept();
        std::cout << "Connection accepted from " << clientSocket.getFd() << std::endl;

        ServerStub serverStub(std::move(clientSocket));
        std::thread serverThread(engineerThread, engineerId, -1, std::ref(taskQueue), std::move(serverStub));
        engineerId += 1;
        serverThread.detach();
        }
        catch(const SocketException& e) {
            std::cerr << "SocketException in ServerMain: " << e.what() << std::endl;
            continue;
        }

    }

    for (auto& expertThread : expertThreads) {
        expertThread.join();
    }

    return 0;
}
