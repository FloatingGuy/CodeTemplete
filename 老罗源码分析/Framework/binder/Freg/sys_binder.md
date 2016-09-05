
/*
* @Author: LK_mac
* @Date:   2016-08-17 19:37:40
* @Last Modified by:   LK_mac
* @Last Modified time: 2016-08-17 20:20:59
* @descript: Binder 系统接口源代码
*/


using namespace std;

frameworks/base{native}/include/binder/  头文件
	|-IInterface.h
	|-Binder.h
	|-BpBinder.h
	|-IPCThreadState.h
	|-ProcessState.h

frameworks/base{native}/libs/binder/  函数实现
	|-IInterface.cpp
	|-Binder.cpp
	|-BpBinder.cpp
	|-IPCThreadState.cpp
	|-ProcessState.cpp

#IInterface.h
	服务端
        * BnInterface
		* DECLARE_META_INTERFACE
		* IMPLEMENT_META_INTERFACE
	客户端
		* BpInterface

```c++
template<typename INTERFACE>
class BnInterface : public INTERFACE, public BBinder
{
public:
    virtual sp<IInterface>      queryLocalInterface(const String16& _descriptor);
    virtual const String16&     getInterfaceDescriptor() const;

protected:
    virtual IBinder*            onAsBinder();
};

// ----------------------------------------------------------------------

template<typename INTERFACE>
class BpInterface : public INTERFACE, public BpRefBase
{
public:
                                BpInterface(const sp<IBinder>& remote);

protected:
    virtual IBinder*            onAsBinder();
};

#define DECLARE_META_INTERFACE(INTERFACE)                               \
    static const android::String16 descriptor;                          \
    static android::sp<I##INTERFACE> asInterface(                       \
            const android::sp<android::IBinder>& obj);                  \
    virtual const android::String16& getInterfaceDescriptor() const;    \
    I##INTERFACE();                                                     \
    virtual ~I##INTERFACE();                                            \


#define IMPLEMENT_META_INTERFACE(INTERFACE, NAME)                       \
    const android::String16 I##INTERFACE::descriptor(NAME);             \
    const android::String16&                                            \
            I##INTERFACE::getInterfaceDescriptor() const {              \
        return I##INTERFACE::descriptor;                                \
    }                                                                   \
    android::sp<I##INTERFACE> I##INTERFACE::asInterface(                \
            const android::sp<android::IBinder>& obj)                   \
    {                                                                   \
        android::sp<I##INTERFACE> intr;                                 \
        if (obj != NULL) {                                              \
            intr = static_cast<I##INTERFACE*>(                          \
                obj->queryLocalInterface(                               \
                        I##INTERFACE::descriptor).get());               \
            if (intr == NULL) {                                         \
                intr = new Bp##INTERFACE(obj);                          \
            }                                                           \
        }                                                               \
        return intr;                                                    \
    }                                                                   \
    I##INTERFACE::I##INTERFACE() { }                                    \
    I##INTERFACE::~I##INTERFACE() { }                                   \
```


/*
#Binder.h
	服务端
	BBinder
	客户端
	BpRefBase
*/
```c++
class BBinder : public IBinder
{
public:
	...
	virtual status_t    transact(   uint32_t code,
                                    const Parcel& data,
                                    Parcel* reply,
                                    uint32_t flags = 0);
protected:
	virtual status_t    onTransact( uint32_t code,
                                    const Parcel& data,
                                    Parcel* reply,
                                    uint32_t flags = 0);
};

class BpRefBase : public virtual RefBase
{
protected:
	...
	inline  IBinder*        remote()                { return mRemote; }
    inline  IBinder*        remote() const          { return mRemote; }
private:
	IBinder* const          mRemote;
    RefBase::weakref_type*  mRefs;
    volatile int32_t        mState;
};
```


/*
#BpBinder.h
	客户端
*/
```c++
class BpBinder : public IBinder
{
public:
	....
						BpBinder(int32_t handle);

    inline  int32_t     handle() const { return mHandle; }
    virtual status_t    transact(   uint32_t code,
                                    const Parcel& data,
                                    Parcel* reply,
                                    uint32_t flags = 0);
private:
    const   int32_t             mHandle;
};
```



##IPCThreadState.h
	服务端
		* IPCThreadState--use-->BBinder
	客户端
		* BpBinder--use--> IPCThreadState


```c++
class IPCThreadState
{
public:
	....
    static  IPCThreadState*     self();
    status_t            transact(int32_t handle,
                                         uint32_t code, const Parcel& data,
                                         Parcel* reply, uint32_t flags);
private:
	const   sp<ProcessState>    mProcess;
    const   pid_t               mMyThreadId;
    Parcel              mIn;
    Parcel              mOut;
};
```



/*
#ProcessState.h
	服务端
	客户端
*/
```c++
class ProcessState : public virtual RefBase
{
public:
    static  sp<ProcessState>    self();
private:
    int                 mDriverFD;
    void*               mVMStart;
};
```