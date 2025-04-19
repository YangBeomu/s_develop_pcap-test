#include <thread>
#include <mutex>
#include <unistd.h>

#include "../../include/networkcontroller.h"

using namespace std;

mutex mtx;
bool controller = true;

void ThreadFunc(NetworkController* nc,char* interface) {
    while(1){
        unique_lock<mutex> t(mtx);
        if(!controller) break;
        t.unlock();

        nc->ShowPacket(interface, EthHdr::Ip4, "", IpHdr::TCP);
        usleep(100);
    }
}

void usage() {
    cout<<"syntax: pcap-test <interface>"<<endl;
    cout<<"sample: pcap-test wlan0"<<endl;
}

bool parse(int argc, char* argv[]) {
    if(argc != 2) {
        usage();
        return false;
    }

    return true;
}

int main(int argc, char *argv[])
{
    if(!parse(argc, argv)) return -1;

    NetworkController nc;
    thread t(ThreadFunc, &nc, argv[1]);

    string input;

    cout<<"if you wnat to end the program, "<<endl;
    cout<<"press ctrl + c or enter \'esc\'"<<endl;

    while(1) {
        cin>>input;
        if(input == "end") {
            unique_lock<mutex> t(mtx);
            controller = false;
            break;
        }
    }

    t.join();
}
