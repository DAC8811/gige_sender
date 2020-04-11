// -*- coding: gb2312-dos -*-
/* @file           StreamConverter.h
 * @copyright      HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @brief          StreamConverter ͼƬ�ļ�ת��Ϊ��Ҫ��������
 *
 * @author         zhenglinjun
 * @date           2015-10-10
 *
 * @note
 *
 * @warning
 */

#ifndef _STREAMCONVERTER_H
#define _STREAMCONVERTER_H

#if defined __WIN32 || defined __WIN64 || defined WIN32 || defined WIN64
#define WINDOWS
#endif

#include "stdint.h"
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
//#define WIN32_LEAN_AND_MEAN   // ����windows.hǰ��Ҫ����һ���꣬ȥ��winsock.h
//#include <windows.h>

#define IMAGE_FILE_MAX_SIZE 4 << 20  // 4MB

#include "Inc/MVErrorDefine.h"
#include "Inc/MVGiGEVisionPixelFormatDefine.h"
#include "Inc/MV_CrossPlatform.h"

#include "Device.h"

class StreamConverter
{
  public:
    typedef unsigned int PixelFormats;

  public:
    StreamConverter(const std::string strDirName);
    virtual ~StreamConverter();

    int Init();
    int DeInit();

    int GetNextFrame(std::string& strFileName, PixelFormats OutFmt = MV_GVSP_PIX_MONO8);
    void GetImageData(Device::virtual_addr_t& pData, size_t& nLen, uint32_t& nSizeX, uint32_t& nSizeY);
    void Lock();
    void Unlock();

  private:
    int UpdateImageData();

  private:
//    HANDLE          _hFind;
//    WIN32_FIND_DATA _FindFileData;
//    FILE	_FindFileData;
    DIR*	_DIR;
    dirent* _FindFileData;
    std::string     _strDirName;
    std::string     _strCurFileName;

    PixelFormats    _OutFmt;
    float           _yuvMatrix[9];

    uint32_t    _nFrameId;
    double      _fTriggerFrequency;

//    CMVMutex       _BufMutex;
    pthread_mutex_t _BufMutex;
    unsigned char* _pStreamBuffer;
    unsigned char* _pImageData;
    size_t         _nImageLen;
    uint32_t       _nSizeX;
    uint32_t       _nSizeY;
};


#endif /* _STREAMCONVERTER_H */
