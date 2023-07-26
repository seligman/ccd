#if defined(DEBUG) || defined(_DEBUG)

#ifdef __cplusplus
extern "C"
{
#endif

    void XTrack_Warn(const char* line);
    void XTrack_Startup();
    void XTrack_Cleanup();
    void* XTrack_Track(void * value, unsigned int line, const char * file);
    void* XTrack_Untrack(void * value, unsigned int line, const char * file);

#ifdef __cplusplus
}
#endif

/* Track a pointer or pointer like thing, returns the pointer being tracked */
#define XTRACK(x)       XTrack_Track(x, __LINE__, __FILE__)
/* Remove the track fo a pointer or pointer like thing, returns the pointer being untracked */
#define XUNTRACK(x)     XTrack_Untrack(x, __LINE__, __FILE__)
/* Startup the XTRACK layer */
#define XTRACK_START()  XTrack_Startup()
/* Stop the XTRACK layer, reporting on anything still being tracked */
#define XTRACK_STOP()   XTrack_Cleanup()

#else

/* In release builds, these macros do nothing */
#define XTRACK(x)       (x)
#define XUNTRACK(x)     (x)
#define XTRACK_START()
#define XTRACK_STOP()

#endif
