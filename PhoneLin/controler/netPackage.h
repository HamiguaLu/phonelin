
#ifndef __PHONE_LIN_PACKAGE_HEADER__
#define __PHONE_LIN_PACKAGE_HEADER__

#include "phonelin.h"

#ifdef Q_OS_WIN32
#pragma pack(1)
#else
__attribute__((packed))
#endif

struct __attribute__((packed, aligned(1))) NetPackageHdr
{
    uint32_t  TAG;
    qint64  data_len;
};



#endif

