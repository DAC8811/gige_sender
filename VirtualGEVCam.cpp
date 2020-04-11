// -*- coding: gb2312-dos -*-
#include "VirtualDevice.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    VirtualDevice VtlDev("/home/ash-1/my_gv_pj/camDev/data","virtual-camera.xml", "VirtualDevice.ini");

    if (VtlDev.Init() != MV_OK)
    {
        cout << "VirtualDevice init error" << endl;
        return -1;
    }
    cout << "Init OK!" << endl;

    if (VtlDev.Starting() != MV_OK)
    {
        cout << "VirtualDevice starting error" << endl;
        return -1;
    }


    VtlDev.DeInit();

    return 0;
}
