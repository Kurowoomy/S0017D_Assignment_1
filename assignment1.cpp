#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <sys/wait.h>
#include <unistd.h>
#include <cmath>
#include <thread>
#include <pthread.h>
#include <vector>
#include <mutex>


int numOfProcessors()
{
    std::string line;
    std::ifstream finfo("/proc/cpuinfo");
    int amount = 0;
    while(getline(finfo,line)) {
        std::stringstream str(line);
        std::string itype;
        std::string info;
        if (getline( str, itype, ':' ) && getline(str,info) && itype.substr(0,9) == "processor" ) {
            amount++;
        }
    }
    return amount;
}

std::string hostname()
{
    std::string line;
    std::ifstream finfo("/proc/sys/kernel/hostname");
    std::string hostname = "none";
    while(getline(finfo,line)) {
        std::stringstream str(line);
        std::string itype;
        std::string info;
        if (getline(str,info)) {
            hostname = info;
        }
    }
    return hostname;
}

std::string hardwarePlatform()
{
    std::string line;
    std::ifstream finfo("/proc/cpuinfo");
    std::string hardwarePlatform = "none";
    while(getline(finfo,line)) {
        std::stringstream str(line);
        std::string itype;
        std::string info;
        if (getline( str, itype, ':' ) && getline(str,info) && itype.substr(0,10) == "model name" ) {
            hardwarePlatform = info;
            break;
        }
    }
    return hardwarePlatform;
}

std::string totalMemory()
{
    std::string line;
    std::ifstream finfo("/proc/meminfo");
    std::string totalMemory = "none";
    while(getline(finfo,line)) {
        std::stringstream str(line);
        std::string itype;
        std::string info;
        if (getline( str, itype, ':' ) && getline(str,info) && itype.substr(0,8) == "MemTotal") {
            totalMemory = info;
            break;
        }
    }
    return totalMemory;
}

int sum;
std::mutex mtx;
void threadFunction(int i, int j)
{
    std::cout << i << " : " << gettid() << std::endl;
    for(j = 1; j <= i; j++)
    {
        std::cout << "Sum: " << sum << std::endl;
        sum += sqrt(i*j);
    }
}

int main(int argc, char **argv)
{
    std::vector<std::string> arguments;
    std::vector<char> flags;
    int X;

    if(argc <= 1)
        return 0;
    for(int i = 0; i < argc; i++)
    {
        arguments.push_back(argv[i]);
    }

    arguments[1].erase(0, 1);
    for(int i = 0; i < arguments[1].size(); i++)
    {
        flags.push_back(arguments[1][i]);
    }
    
    for(int i = 0; i < flags.size(); i++)
    {
        if(flags[i] == 'i')
        {
            // print out the number of processors, the hostname, the hardware platform, and the
            // total amount of memory
            std::cout << std::endl;
            // get number of processors somehow
            std::cout << "Number of processors: " << numOfProcessors() << std::endl;
            // get hostname
            std::cout << "Hostname: " << hostname() << std::endl;
            // get hardware platform
            std::cout << "Hardware platform: " << hardwarePlatform() << std::endl;
            // get total amount of memory
            std::cout << "Total amount of memory: " << totalMemory() << std::endl;
            std::cout << std::endl;
        }
        else if(flags[i] == 'f' && argc == 3)
        {
            int i = 1;
            int j = 1;
            pid_t pid;
            sum = 0;
            std::cout << "forking processes" << std::endl;

            std::stringstream Xstream(argv[2]);
            Xstream >> X;

            for(i = 1; i <= X; i++)
            {
                pid = fork();

                if(pid)
                    break;

                std::cout << i << " : " << getpid() << std::endl;
                for(j = 1; j <= i; j++)
                {
                    std::cout << "Sum: " << sum << std::endl;
                    sum += sqrt(i*j);
                }
            }
            wait(NULL);
            if(pid)
                return 0;
            std::cout << "Total sum: " << sum << std::endl;

        }
        else if(flags[i] == 't' && argc == 3)
        {
            std::cout << "threading processes" << std::endl;
            // skapa tråd-funktion som utför samma sak som i -f, ta in argument för X.
            int i = 1;
            sum = 0;

            std::stringstream Xstream(argv[2]);
            Xstream >> X;

            std::thread threadList[X];
            for(i = 1; i <= X; i++)
            {
                int index = i;
                threadList[i-1] = std::thread(threadFunction, i, 1);
            }
            for(std::thread& t : threadList)
            {
                t.join();
            }
            std::cout << "Total sum: " << sum << std::endl;
        }
    }
}