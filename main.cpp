
#include "CYdLidar.h"
#include <iostream>
#include <string>
#include <signal.h>
#include <memory>
#include <unistd.h>
using namespace std;
using namespace ydlidar;

CYdLidar laser;
static bool running = false;

static void Stop(int signo)   
{  
    printf("Received exit signal\n");
    running = true;
}  

int main(int argc, char * argv[])
{
    bool showHelp  = argc>1 && !strcmp(argv[1],"--help");

    printf(" YDLIDAR C++ TEST\n");
    if (argc<4 || showHelp )
    {
        printf("Usage: %s <serial_port> <baudrate> <intensities>\n\n",argv[0]);
        printf("Example:%s /dev/ttyUSB0 115200 0\n\n",argv[0]);
        if (!showHelp)
        {
            return -1;
        }
        else
            return 0;
    }

    const std::string port  = string(argv[1]);
    const int baud          = atoi(argv[2]);
    const int intensities   = atoi(argv[3]);
    bool hardError;
    LaserScan scan;

    signal(SIGINT, Stop);
    signal(SIGTERM, Stop);

    laser.setSerialPort(port);
    laser.setSerialBaudrate(baud);
    laser.setIntensities(intensities);

    laser.initialize();

    float sector;
    while(!running)
    {
        if(laser.doProcessSimple(scan, hardError))
        {
            printf("\n\n\n");
            for(int i=0; i<(unsigned int)scan.ranges.size(); ++i)
            {
                if( i%10 == 0 )
                {
                    printf("\n");
                    for(int k=0; k<(int)(sector/10.0*40.0); ++k)
                        printf("*");
                    sector = 0.0;
                }
                else
                    sector += scan.ranges[i];
            }
        }
        usleep(50*1000);
    }

    laser.turnOff();
    laser.disconnecting();

    return 0;
}
