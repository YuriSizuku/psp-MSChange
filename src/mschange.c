/*
 * emulate psp memory stick attach or detach
 *   v0.1, developed by devseed 
 */
#include <pspkernel.h>
#include <pspctrl.h>
#include <pspmscm.h>
#include <stdio.h>
#include <string.h>

PSP_MODULE_INFO("mschange", PSP_MODULE_KERNEL, 0, 1);

// util functions
#ifdef ENABLE_FILELOG
#include <pspiofilemgr.h>
char g_logbuf[256];
SceUID g_logsema = 0;

// logpath must use from ms0:/
#define LOGFILE "ms0:/mschange.log.txt"

#define LOG_INIT()\
    {\
        SceUID fd = sceIoOpen(LOGFILE, PSP_O_CREAT | PSP_O_WRONLY, 0777);\
        if(fd>=0) sceIoClose(fd);\
        g_logsema = sceKernelCreateSema("LOGFILE", 0, 1, 1, NULL);\
    }
#define LOG_DEINIT()\
    {\
        if(g_logsema>=0) sceKernelDeleteSema(g_logsema);\
    }

#define LOG(format, ...) \
    {\
        if(g_logsema>=0) sceKernelWaitSema(g_logsema, 1, NULL);\
        sprintf(g_logbuf, format, ##__VA_ARGS__);\
        SceUID fd = sceIoOpen(LOGFILE, PSP_O_APPEND | PSP_O_CREAT | PSP_O_WRONLY, 0777);\
        if(fd>=0)\
        {\
           sceIoWrite(fd, g_logbuf, strlen(g_logbuf));\
           sceIoClose(fd);\
        }\
        if(g_logsema>=0) sceKernelSignalSema(g_logsema, 1);\
    }
#else 
#define LOG_INIT()
#define LOG_DINIT()
#define LOG(format, ...) printf(format, ##__VA_ARGS__)
#endif

// memory stick hook function
SceUID g_mscb = -1;
int g_msarg = MS_CB_EVENT_INSERTED;
void ms_change(int arg)
{
    LOG("in ms_change g_mscb=%d, arg=%d\n", g_mscb, arg);
    if(g_mscb>=0)
    {
        // arg: 1 inserted, 2 removed
        g_msarg = arg;
        sceKernelNotifyCallback(g_mscb, arg);
        sceKernelDelayThread(1000000);
        // SceKernelCallbackInfo cbinfo; 
        // sceKernelReferCallbackStatus(ms_cb, &cbinfo);
    }
}

#if 1 // hook methods
extern u32 sctrlHENFindFunction(char* szMod, char* szLib, u32 nid);
void sctrlHENPatchSyscall(void *addr, void *newaddr)
{
	void *ptr;
	u32 *syscalls;
	int i;
	u32 _addr = (u32)addr;

	// get syscall struct from cop0
	asm("cfc0 %0, $12\n" : "=r"(ptr));
	if (ptr == NULL) return;
	syscalls = (u32*)(ptr+0x10);

	for(i=0; i<0xFF4; ++i) 
    {
		if ((syscalls[i] & 0x0FFFFFFF) == (_addr & 0x0FFFFFFF)) 
        {
			syscalls[i] = (u32)newaddr;
		}
	}
}

int sceIoDevctl_hook(const char *dev, unsigned int cmd, 
    void *indata, int inlen, void *outdata, int outlen)
{
    // actually this function works, but don't know why log doesn't work
    LOG("in sceIoDevctl_hook %s, %08x\n", dev, cmd);
    if(strcmp(dev, "fatms0:")==0)
    {
        if(cmd==0x2415821) // MScmRegisterMSInsertEjectCallback
        {
            g_mscb = *(SceUID*)indata;
        }
        else if(cmd==0x2425823) // Check if FAT enabled
        {
            if(g_msarg == MS_CB_EVENT_EJECTED)
            {
                *((u32*)outdata) = 0;
                return 0;
            }
        }
    }
    return sceIoDevctl(dev, cmd, indata, inlen, outdata, outlen);
}

void install_hook()
{
    // nid from ppsspp_niddb.xml
    LOG("in install_hook\n");
    void *addr = (void*)sctrlHENFindFunction("sceIOFileManager", "IoFileMgrForUser", 0x54F5FB11);
    LOG("find sceIoDevctl at %p\n", addr);

    if(addr) sctrlHENPatchSyscall(addr, sceIoDevctl_hook);
	sceKernelDcacheWritebackAll();
	sceKernelIcacheClearAll();
}

void uninstall_hook()
{
    LOG("in uninstall_hook\n");
    void *addr = (void*)sctrlHENFindFunction("sceIOFileManager", "IoFileMgrForUser", 0x54F5FB11);
    LOG("find sceIoDevctl at %p\n", addr);

    if(addr) sctrlHENPatchSyscall(sceIoDevctl_hook, addr);
	sceKernelDcacheWritebackAll();
	sceKernelIcacheClearAll();
}
#endif

// event function
enum STATUS{
    STATUS_EXIT = 0,
    STATUS_RUNNING
} g_status = STATUS_RUNNING;

int thread_main(SceSize args, void *argp)
{
    LOG("in thread_main tid=%d\n", sceKernelGetThreadId());
    sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
    SceCtrlData pad;
    while(g_status!=STATUS_EXIT)
    {
        sceCtrlReadBufferPositive(&pad, 1); 
        if(pad.Buttons!=0)
        {
            if(pad.Buttons & PSP_CTRL_SELECT)
            {
                if(pad.Buttons & PSP_CTRL_UP) ms_change(MS_CB_EVENT_INSERTED);
                else if (pad.Buttons & PSP_CTRL_DOWN) ms_change(MS_CB_EVENT_EJECTED);
            }
        }
        sceKernelDelayThread(50000); // 50ms, need to add delay, or exit crash
    }
    return 0;
}

int module_start(SceSize args, void *argp)
{
    LOG_INIT();
    LOG("mschange module_start\n");
    install_hook();

    SceUID thid = sceKernelCreateThread("ThreadMain", thread_main, 16, 0x1000, 0, NULL);
    LOG("sceKernelCreateThread  thread_main %d\n", thid);
    if(thid>=0) sceKernelStartThread(thid, args, argp);
	return 0;
}

int module_stop(SceSize args, void *argp)
{
    LOG("mschange module_stop\n");
    uninstall_hook();
    g_status = STATUS_EXIT;
    LOG_DEINIT();
	return 0;
}