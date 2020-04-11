// -*- coding: gb2312-dos -*-
#include "stdint.h"
#include <stdlib.h>
#include <fstream>

#if defined __WIN32 || defined __WIN64 || defined WIN32 || defined WIN64
#define WINDOWS
#endif

#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "VirtualDevice.h"

using namespace std;
using namespace MVComponent;
using namespace MvCamCtrl;

// GetLocalIp, GetLocalMac从pInfo结构体中读取ip与mac地址并存入uint变量
// inet_addr将xxx.xxx.xxx.xxx形的字符串形ip地址转换为uint形变量
// pInfo结构体为_IP_ADAPTER_INFO类型，_IP_ADAPTER_INFO为windows sdk中存储网卡信息的结构体
// GetAdaptersInfo函数用于获取全部网卡信息，并存入pInfo结构体：https://www.cnblogs.com/L-hq815/archive/2012/08/04/2622829.html
// 这里只实现了windows下获取本地ip和mac的函数，没有实现linux下的，TODOYJP
/* @fn          GetLocalIp
 * @brief       Get local IP
 * @param[in]   pInfo
 * @param[out]  nCurrentIp
 * @param[out]  nCurrentSubNetMask
 * @param[out]  nDefultGateWay
 * @return      none
 */
void GetLocalIp(void* pInfo, unsigned int& nCurrentIp, unsigned int& nCurrentSubNetMask, unsigned int& nDefultGateWay)
{
#ifdef WINDOWS
    if (pInfo)
    {
        nCurrentIp         = inet_addr(((IP_ADAPTER_INFO*)pInfo)->IpAddressList.IpAddress.String);
        nCurrentSubNetMask = inet_addr(((IP_ADAPTER_INFO*)pInfo)->IpAddressList.IpMask.String);
        nDefultGateWay     = inet_addr(((IP_ADAPTER_INFO*)pInfo)->GatewayList.IpAddress.String);
        nCurrentIp         = ntohl(nCurrentIp);
        nCurrentSubNetMask = ntohl(nCurrentSubNetMask);
        nDefultGateWay     = ntohl(nDefultGateWay);
    }
#endif
}

/* @fn          GetLocalMac
 * @brief       Get local MAC
 * @param[in]   pInfo
 * @param[out]  nMacAddrHigh
 * @param[out]  nMacAddrLow
 * @return      none
 */
void GetLocalMac(void* pInfo, unsigned int& nMacAddrHigh, unsigned int& nMacAddrLow)
{
#ifdef WINDOWS
    if (pInfo)
    {
        long long nMacAddr;
        memcpy((unsigned char*)&nMacAddr, (unsigned char*)&(((IP_ADAPTER_INFO*)pInfo)->Address[0]), sizeof(((IP_ADAPTER_INFO*)pInfo)->Address));
        nMacAddrHigh = (nMacAddr & 0x0000000000ffff);
        nMacAddrHigh = ntohl(nMacAddrHigh);
        nMacAddrLow = (nMacAddr & 0xffffffffff0000) >> 16;
        nMacAddrLow = ntohl(nMacAddrLow);
    }
#endif
}

// 初始化_bCancel, _strXmlFileName,_VtMemSize等变量
// 初始化_Gvcp, _Gvsp, _pThreadGvcp, _pThreadGvsp, _pVtMem等指针
// 初始化INI::Parser类_iniDeviceInfo，解析入参的ini文件
VirtualDevice::VirtualDevice(const char* szInDataDir,const char* szXmlFileName, const char* szDeviceIni):
_Gvcp((Device*)this),  _Gvsp((Device*)this, (StreamConverter*)&_Strm), _Strm(szInDataDir), _iniDeviceInfo(szDeviceIni)
{
	_bCancel = false;
	_strXmlFileName = szXmlFileName;
//	_Gvcp = (Device*)this;
//    _Gvsp = (Device*)this;
	_pThreadGvcp = NULL;
	_pThreadGvsp = NULL;
//	_iniDeviceInfo = INI::Parser(szDeviceIni);
	_pVtMem = NULL;
	_VtMemSize = MV_GEV_REG_MEMORY_SIZE+MV_GEV_XML_FILE_MAX_SIZE;

    ;
}

VirtualDevice::~VirtualDevice()
{
    ;
}

int VirtualDevice::Init()
{
    int nRet = MV_OK;

    // Virtual memory
    _pVtMem = new unsigned char[_VtMemSize];

    // Init stream converter
    if ((nRet = _Strm.Init()) != MV_OK)
	{
	   cout << "Init stream converter fail!!" << endl;
	   return nRet;
	}


    // Init GVCP socket
    if ((nRet = _Gvcp.Init()) != MV_OK)
    {
        cout << "[WARN]";
        cout << "Init GVCP socket fail!!" << endl;
        return nRet;
    }


    // Init GVSP socket
    if ((nRet = _Gvsp.Init()) != MV_OK)
    {
        cout << "[WARN]";
        cout << "Init GVSP socket fail!!" << endl;
        return nRet;
    }


    // Init virtual device memory
    // CCP
    memset(_pVtMem, 0, _VtMemSize);
    // TODOYJP:看到这里了
    if ((nRet = InitVtMem()) != MV_OK)
    {
        cout << "[WARN]";
        cout << "Init virtual device memory fail!!" << endl;
        return nRet;
    }


    return nRet;
}

int VirtualDevice::InitVtMem()
{
    int nRet = MV_OK;

    // Init default Mac, Ip info etc.
    // void* pInfo = NULL;
#ifdef WINDOWS
    uint32_t uSize = 0;
    GetAdaptersInfo((PIP_ADAPTER_INFO)pInfo, (PULONG)&uSize);    // Get uSize
    pInfo = (IP_ADAPTER_INFO*)malloc(uSize);
    GetAdaptersInfo((PIP_ADAPTER_INFO)pInfo, (PULONG)&uSize);
#endif

    unsigned int nDefMacAddrHigh, nDefMacAddrLow, nDefCurrentIp, nDefCurrentSubNetMask, nDefDefultGateWay;
    unsigned int nCurrentIp, nCurrentSubNetMask, nDefultGateWay, nMacAddrHigh, nMacAddrLow;
    // GetLocalMac(pInfo, nDefMacAddrHigh, nDefMacAddrLow);
    // GetLocalIp(pInfo, nDefCurrentIp, nDefCurrentSubNetMask, nDefDefultGateWay);
//    nCurrentIp         = inet_addr("192.168.3.131");
//    nCurrentSubNetMask = inet_addr("255.255.255.0");
//    nDefultGateWay     = inet_addr("192.168.3.1");
    nCurrentIp         = inet_addr("192.168.2.248");
    nCurrentSubNetMask = inet_addr("255.255.255.0");
    nDefultGateWay     = inet_addr("192.168.2.255");
    nCurrentIp         = ntohl(nCurrentIp);
    nCurrentSubNetMask = ntohl(nCurrentSubNetMask);
    nDefultGateWay     = ntohl(nDefultGateWay);
    nMacAddrHigh = (0xbfe093290c00 & 0x0000000000ffff);
    nMacAddrHigh = ntohl(nMacAddrHigh);
    nMacAddrLow = (0xbfe093290c00 & 0xffffffffff0000) >> 16;
    nMacAddrLow = ntohl(nMacAddrLow);


    // Init device info. Read from .ini file
    INI::Level info = _iniDeviceInfo.top()("INFO_CC");
    int Tmp;
    _DeviceInfo.nMajorVer          = ::atoi(info["MajorVer"].c_str());
    _DeviceInfo.nMinorVer          = ::atoi(info["MinorVer"].c_str());
    _DeviceInfo.nDeviceMode        = ::atoi(info["DeviceMode"].c_str());
    _DeviceInfo.nMacAddrHigh       = nMacAddrHigh;
    _DeviceInfo.nMacAddrLow        = nMacAddrLow;
    _DeviceInfo.nTLayerType        = MV_GIGE_DEVICE;

    info = _iniDeviceInfo.top()("INFO_GEV");
    _DeviceInfo.stGigEInfo.nIpCfgOption       = ::atoi(info["IpCfgOption"].c_str());
    _DeviceInfo.stGigEInfo.nIpCfgCurrent      = ::atoi(info["IpCfgCurrent"].c_str());
    _DeviceInfo.stGigEInfo.nCurrentIp         = nCurrentIp;
    _DeviceInfo.stGigEInfo.nCurrentSubNetMask = nCurrentSubNetMask;
    _DeviceInfo.stGigEInfo.nDefultGateWay     = nDefultGateWay;

    memcpy(_DeviceInfo.stGigEInfo.chManufacturerName, info["ManufacturerName"].c_str(), 32);
    memcpy(_DeviceInfo.stGigEInfo.chModelName, info["ModelName"].c_str(), 32);
    memcpy(_DeviceInfo.stGigEInfo.chDeviceVersion, info["DeviceVersion"].c_str(), 32);
    memcpy(_DeviceInfo.stGigEInfo.chManufacturerSpecificInfo, info["ManufacturerSpecificInfo"].c_str(), 48);
    memcpy(_DeviceInfo.stGigEInfo.chSerialNumber, info["SerialNumber"].c_str(), 16);
    memcpy(_DeviceInfo.stGigEInfo.chUserDefinedName, info["UserDefinedName"].c_str(), 16);

    // Device info
    // 根据协议手册设置虚拟寄存器的值
    Tmp = (_DeviceInfo.nMinorVer << 16) + _DeviceInfo.nMajorVer;
    SetMem((virtual_addr_t)MV_REG_Version, (&Tmp), 4);
    SetMem((virtual_addr_t)MV_REG_DeviceMode, (&_DeviceInfo.nDeviceMode), 4);
    SetMem((virtual_addr_t)MV_REG_DeviceMACAddressHigh0, (&_DeviceInfo.nMacAddrHigh), 4);
    SetMem((virtual_addr_t)MV_REG_DeviceMACAddressLow0, (&_DeviceInfo.nMacAddrLow), 4);
    SetMem((virtual_addr_t)MV_REG_NetworkInterfaceCapability0, (&_DeviceInfo.stGigEInfo.nIpCfgOption), 4);
    SetMem((virtual_addr_t)MV_REG_NetworkInterfaceConfiguration0, (&_DeviceInfo.stGigEInfo.nIpCfgCurrent), 4);
    SetMem((virtual_addr_t)MV_REG_CurrentIPAddress0, (&_DeviceInfo.stGigEInfo.nCurrentIp), 4);
    SetMem((virtual_addr_t)MV_REG_CurrentSubnetMask0, (&_DeviceInfo.stGigEInfo.nCurrentSubNetMask), 4);
    SetMem((virtual_addr_t)MV_REG_CurrentDefaultGateway0, (&_DeviceInfo.stGigEInfo.nDefultGateWay), 4);
    SetMem((virtual_addr_t)MV_REG_ManufacturerName, (&_DeviceInfo.stGigEInfo.chManufacturerName), 32);
    SetMem((virtual_addr_t)MV_REG_ModelName, (&_DeviceInfo.stGigEInfo.chModelName), 32);
    SetMem((virtual_addr_t)MV_REG_DeviceVersion, (&_DeviceInfo.stGigEInfo.chDeviceVersion), 32);
    SetMem((virtual_addr_t)MV_REG_ManufacturerInfo, (&_DeviceInfo.stGigEInfo.chManufacturerSpecificInfo), 48);
    SetMem((virtual_addr_t)MV_REG_SerialNumber, (&_DeviceInfo.stGigEInfo.chSerialNumber), 16);
    SetMem((virtual_addr_t)MV_REG_UserdefinedName, (&_DeviceInfo.stGigEInfo.chUserDefinedName), 16);

    // MISC
//    SetReg((virtual_addr_t)MV_REG_ControlChannelPrivilege, 1);
    SetReg((virtual_addr_t)MV_REG_ControlChannelPrivilege, 0);
    SetReg((virtual_addr_t)MV_REG_HeartbeatTimeout, 1000);
    SetReg((virtual_addr_t)MV_REG_TimestampTickFrequencyHigh, 0);
    SetReg((virtual_addr_t)MV_REG_TimestampTickFrequencyLow, 1000000000);
    SetReg((virtual_addr_t)MV_REG_StreamChannelPacketSize0, 2000);
    SetReg((virtual_addr_t)MV_REG_NumberofStreamChannels, 1);


    // XML defined register
//    SetReg((virtual_addr_t)REG_XML_AcquisitionStart_RegAddr, 0x0000);
//    SetReg((virtual_addr_t)REG_XML_AcquisitionStop_RegAddr, 0x0001);
    SetReg((virtual_addr_t)REG_XML_AcquisitionStart_RegAddr, 0x0001);
    SetReg((virtual_addr_t)REG_XML_AcquisitionStop_RegAddr, 0x0000);
    SetReg((virtual_addr_t)REG_XML_Width_RegAddr, 0x0c00);
    SetReg((virtual_addr_t)REG_XML_Height_RegAddr, 0x0800);
    SetReg((virtual_addr_t)REG_XML_OffsetX_RegAddr, 0x0000);
    SetReg((virtual_addr_t)REG_XML_OffsetY_RegAddr, 0x0000);


    // XML file
    char szXmlUrl[MV_GEV_XML_URL_LEN];
    fstream XmlFile;
    XmlFile.open(_strXmlFileName.c_str(), ios::in | ios::binary);
    XmlFile.seekg(0, ios::end);
    _nXmlFileSize = XmlFile.tellg();
    XmlFile.seekg(0, ios::beg);
    if (_nXmlFileSize <= MV_GEV_XML_FILE_MAX_SIZE)
    {
        sprintf(szXmlUrl, "Local:%s;%x;%x", _strXmlFileName.c_str(), MV_GEV_REG_MEMORY_SIZE, _nXmlFileSize);
        SetMem(reinterpret_cast<virtual_addr_t>(MV_REG_FirstURL), szXmlUrl, strlen(szXmlUrl));
        XmlFile.read((char*)_pVtMem + MV_GEV_REG_MEMORY_SIZE, _nXmlFileSize);
    }
    else
    {
        cout << "[WARN]";
        cout << "Read XML file (" << _strXmlFileName << ") fail!!" << endl;
        return -1;
    }

    return nRet;
}

int VirtualDevice::DeInit()
{
    int nRet = MV_OK;

    if (_pVtMem)
    {
        delete[] _pVtMem;
        _pVtMem = NULL;
    }

//    _Strm.DeInit();
    _Gvcp.DeInit();
    _Gvsp.DeInit();


    return nRet;
}


int VirtualDevice::Starting()
{
    // TODOYJP：Linux平台下创建线程未实现，_pThreadGvcp, HandlingControlPacket,  (&(this->_Gvcp)) 这三个要有，其他无所谓
    pthread tGvcp(DeviceGVCP::HandlingControlPacket, (&(this->_Gvcp)));
    pthread tGvsp(DeviceGVSP::HandlingStreamPacket, (&(this->_Gvsp)));

    cout << "Starting!" << endl;
    // Wait for cancel
    // _bCancel
    while (!IsCancel())
    {
//        UpdateStreamNextFrame();
        sleep(100);
    }
    while(1);
//    MV_WaitForThreadEnd(_pThreadGvcp);
//    MV_WaitForThreadEnd(_pThreadGvsp);

    return MV_OK;
}

const MV_CC_DEVICE_INFO* VirtualDevice::GetDeviceInfo()
{
    return &_DeviceInfo;
}

int VirtualDevice::GetReg(virtual_addr_t RegAddr, uint32_t& Data)
{
    return GetMem(RegAddr, &Data, 4);
}

inline uint32_t VirtualDevice::GetReg(virtual_addr_t RegAddr)
{
    uint32_t Data = -1;

    GetMem(RegAddr, &Data, 4);
    return Data;
}

int VirtualDevice::SetReg(virtual_addr_t RegAddr, const uint32_t Data)
{
    return SetMem(RegAddr, &Data, 4);
}

int VirtualDevice::GetMem(virtual_addr_t MemAddr, void* Data, const size_t Count)
{
    int nRet = MV_OK;

    if (reinterpret_cast<uintptr_t>(MemAddr) <= _VtMemSize)
    {
        MemAddr = _pVtMem + reinterpret_cast<uintptr_t>(MemAddr);
        memcpy(Data, (unsigned char*)MemAddr, Count);
    }
    else
    {
        nRet = MV_E_PARAMETER;
    }

    return nRet;
}

int VirtualDevice::SetMem(virtual_addr_t MemAddr, const void* Data, const size_t Count)
{
    int nRet = MV_OK;

    if (reinterpret_cast<uintptr_t>(MemAddr) <= _VtMemSize)
    {
        MemAddr = _pVtMem + reinterpret_cast<uintptr_t>(MemAddr);
        memcpy((unsigned char*)MemAddr, Data, Count);
    }
    else
    {
        nRet = MV_E_PARAMETER;
    }

    return nRet;
}
/*
int VirtualDevice::UpdateStreamNextFrame(StreamConverter::PixelFormats OutFmt)
{
    int nRet = MV_OK;
    string strFileName;

    if ((nRet = _Strm.GetNextFrame(strFileName, OutFmt)) != MV_OK)
    {
        return nRet;
    }

    Device::virtual_addr_t pData;
    size_t                 nLen;
    uint32_t               nSizeX, nSizeY;
    _Strm.GetImageData(pData, nLen, nSizeX, nSizeY);
    SetReg((virtual_addr_t)REG_XML_Width_RegAddr, nSizeX);
    SetReg((virtual_addr_t)REG_XML_Height_RegAddr, nSizeY);

    return nRet;
}
*/

uint32_t VirtualDevice::GetAcquisitionState()
{
    return GetReg((virtual_addr_t)REG_XML_AcquisitionStart_RegAddr);
}

void VirtualDevice::SetTriggerFrequency(double frequency)
{
    _fTriggerFrequency = frequency;
}

uint32_t VirtualDevice::GetControlChannelPrivilege()
{
    return GetReg((virtual_addr_t)MV_REG_ControlChannelPrivilege);
}

void VirtualDevice::SetControlChannelPrivilege(uint32_t privilege)
{
    SetReg((virtual_addr_t)MV_REG_ControlChannelPrivilege, privilege);
}

uint32_t VirtualDevice::GetPayload()
{
    int width = GetReg((virtual_addr_t)REG_XML_Width_RegAddr);
    int height = GetReg((virtual_addr_t)REG_XML_Height_RegAddr);

    return width * height;
}

uint32_t VirtualDevice::GetHeartbeatTimeout()
{
    return GetReg((virtual_addr_t)MV_REG_HeartbeatTimeout);
}

bool VirtualDevice::IsCancel()
{
    return this->_bCancel;
}
