#ifndef rth_out_c
#define rth_out_c

/* ====================================================================== */
/* rth_out_c - no-include, no-libc, multi-platform output layer           */
/*                                                                        */
/* Supported:                                                             */
/*   Windows  (MSVC / MinGW / Cygwin / MSYS2)   -> Win32 API (dllimport)  */
/*   Linux x86_64                               -> raw syscall            */
/*   Linux i386                                 -> raw syscall (int 0x80)  */
/*   Linux aarch64                              -> raw syscall            */
/*   Linux armv7                                -> raw syscall            */
/*   Linux riscv64                              -> raw syscall (ecall)    */
/*   macOS x86_64 / arm64                       -> raw syscall            */
/*   FreeBSD x86_64                             -> raw syscall            */
/*   Others                                     -> compile, all return -1 */
/*                                                                        */
/* No #include. No typedef. No libc.                                      */
/* ====================================================================== */

/* ====================================================================== */
/* Feature switches                                                       */
/* ====================================================================== */
#ifndef roc_stdout
#define roc_stdout
#endif
#ifndef roc_stderror
#define roc_stderror
#endif
#ifndef roc_fileout
#define roc_fileout
#endif
#ifndef roc_logout
#define roc_logout
#endif
#ifndef roc_netout
#define roc_netout
#endif

/* ====================================================================== */
/* Program entry                                                          */
/* ====================================================================== */
#undef _start_rt
#define _start_rt int main(int argc,char* argv[]){

#undef _end_rt
#   ifdef __cplusplus
#       define _end_rt return {};}
#   else
#       define _end_rt return 0;}
#   endif

/* ====================================================================== */
/* Platform detection                                                     */
/* ====================================================================== */
#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__) || \
    defined(__MINGW32__) || defined(__MINGW64__) || defined(__MSYS__)
#   define ROC_PLAT_WIN 1
#else
#   define ROC_PLAT_WIN 0
#endif

#if defined(__APPLE__) && defined(__MACH__)
#   define ROC_PLAT_MAC 1
#else
#   define ROC_PLAT_MAC 0
#endif

#if defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
#   define ROC_PLAT_FBSD 1
#else
#   define ROC_PLAT_FBSD 0
#endif

#if defined(__linux__)
#   define ROC_PLAT_LINUX 1
#else
#   define ROC_PLAT_LINUX 0
#endif

/* Arch on Linux */
#if ROC_PLAT_LINUX && defined(__x86_64__)
#   define ROC_ARCH_X64 1
#else
#   define ROC_ARCH_X64 0
#endif
#if ROC_PLAT_LINUX && (defined(__i386__) || defined(__i686__))
#   define ROC_ARCH_I386 1
#else
#   define ROC_ARCH_I386 0
#endif
#if ROC_PLAT_LINUX && defined(__aarch64__)
#   define ROC_ARCH_ARM64 1
#else
#   define ROC_ARCH_ARM64 0
#endif
#if ROC_PLAT_LINUX && (defined(__arm__) || defined(__thumb__))
#   define ROC_ARCH_ARMV7 1
#else
#   define ROC_ARCH_ARMV7 0
#endif
#if ROC_PLAT_LINUX && defined(__riscv) && (__riscv_xlen == 64)
#   define ROC_ARCH_RV64 1
#else
#   define ROC_ARCH_RV64 0
#endif

/* Arch on macOS */
#if ROC_PLAT_MAC && defined(__x86_64__)
#   define ROC_MAC_X64 1
#else
#   define ROC_MAC_X64 0
#endif
#if ROC_PLAT_MAC && defined(__arm64__)
#   define ROC_MAC_ARM64 1
#else
#   define ROC_MAC_ARM64 0
#endif

/* ====================================================================== */
/* Socket type / invalid value                                            */
/* ====================================================================== */
#if ROC_PLAT_WIN
#   if defined(_WIN64)
#       define ROC_SOCK_T unsigned long long
#   else
#       define ROC_SOCK_T unsigned int
#   endif
#   define ROC_SOCK_INVALID ((ROC_SOCK_T)-1)
#   define ROC_SOCK_IS_VALID(fd) ((fd) != (ROC_SOCK_T)-1)
#else
#   define ROC_SOCK_T long
#   define ROC_SOCK_INVALID (-1L)
#   define ROC_SOCK_IS_VALID(fd) ((fd) >= 0)
#endif

/* ====================================================================== */
/* Windows API declarations                                               */
/* ====================================================================== */
#if ROC_PLAT_WIN
#   ifdef __cplusplus
extern "C" {
#   endif

__declspec(dllimport) void* __stdcall GetStdHandle(unsigned long);
__declspec(dllimport) int   __stdcall WriteFile(void*, const void*, unsigned long,
                                                unsigned long*, void*);
__declspec(dllimport) void* __stdcall CreateFileA(const char*, unsigned long,
                                                  unsigned long, void*,
                                                  unsigned long, unsigned long,
                                                  void*);
__declspec(dllimport) int   __stdcall CloseHandle(void*);
__declspec(dllimport) int   __stdcall WSAStartup(unsigned short, void*);
__declspec(dllimport) int   __stdcall WSACleanup(void);
__declspec(dllimport) ROC_SOCK_T __stdcall socket(int, int, int);
__declspec(dllimport) int   __stdcall connect(ROC_SOCK_T, const void*, int);
__declspec(dllimport) int   __stdcall send(ROC_SOCK_T, const char*, int, int);
__declspec(dllimport) int   __stdcall closesocket(ROC_SOCK_T);

#   ifdef __cplusplus
}
#   endif
#endif

/* ====================================================================== */
/* Channel / log level constants                                          */
/* ====================================================================== */
#define ROC_STDOUT 1
#define ROC_STDERR 2
#define ROC_FILE   3
#define ROC_LOG    4
#define ROC_NET    5

#define ROC_LOG_DEBUG 0
#define ROC_LOG_INFO  1
#define ROC_LOG_WARN  2
#define ROC_LOG_ERROR 3
#define ROC_LOG_FATAL 4

/* ====================================================================== */
/* Raw write primitive                                                    */
/*   fd: 1 = stdout, 2 = stderr                                           */
/* ====================================================================== */
#if defined(roc_stdout) || defined(roc_stderror) || defined(roc_fileout)

/* ---- Windows ---- */
#if ROC_PLAT_WIN
static inline long roc_raw_write(long fd, const char* b, unsigned long l) {
    void* h = GetStdHandle(fd == 2 ? (unsigned long)-12 : (unsigned long)-11);
    unsigned long done = 0;
    if (!h || h == (void*)-1) return -1;
    while (done < l) {
        unsigned long chunk = (l - done > 0x7FFFFFFFUL) ? 0x7FFFFFFFUL : (l - done);
        unsigned long w = 0;
        if (!WriteFile(h, b + done, chunk, &w, 0)) return -1;
        if (w == 0) break;
        done += w;
    }
    return (long)done;
}

/* ---- Linux x86_64: syscall 1 = write ---- */
#elif ROC_ARCH_X64
static inline long roc_raw_write(long fd, const char* b, unsigned long l) {
    long total = 0, n;
    while (l > 0) {
        __asm__ volatile("syscall"
            : "=a"(n)
            : "a"(1L), "D"(fd), "S"((long)b), "d"((long)l)
            : "rcx", "r11", "memory");
        if (n < 0) return total ? total : -1;
        if (n == 0) break;
        total += n; b += n; l -= (unsigned long)n;
    }
    return total;
}

/* ---- Linux i386: int 0x80, syscall 4 = write ---- */
#elif ROC_ARCH_I386
static inline long roc_raw_write(long fd, const char* b, unsigned long l) {
    long total = 0, n;
    while (l > 0) {
        __asm__ volatile("int $0x80"
            : "=a"(n)
            : "a"(4L), "b"((long)fd), "c"((long)b), "d"((long)l)
            : "memory", "cc");
        if (n < 0) return total ? total : -1;
        if (n == 0) break;
        total += n; b += n; l -= (unsigned long)n;
    }
    return total;
}

/* ---- Linux aarch64: svc #0, syscall 64 = write ---- */
#elif ROC_ARCH_ARM64
static inline long roc_raw_write(long fd, const char* b, unsigned long l) {
    long total = 0, n;
    register long x8 __asm__("x8");
    register long x0 __asm__("x0");
    register long x1 __asm__("x1");
    register long x2 __asm__("x2");
    while (l > 0) {
        x8 = 64; x0 = fd; x1 = (long)b; x2 = (long)l;
        __asm__ volatile("svc #0"
            : "+r"(x0) : "r"(x8), "r"(x1), "r"(x2) : "memory");
        n = x0;
        if (n < 0) return total ? total : -1;
        if (n == 0) break;
        total += n; b += n; l -= (unsigned long)n;
    }
    return total;
}

/* ---- Linux armv7 (EABI): svc #0, syscall 4 = write ---- */
#elif ROC_ARCH_ARMV7
static inline long roc_raw_write(long fd, const char* b, unsigned long l) {
    long total = 0, n;
    register long r7 __asm__("r7");
    register long r0 __asm__("r0");
    register long r1 __asm__("r1");
    register long r2 __asm__("r2");
    while (l > 0) {
        r7 = 4; r0 = fd; r1 = (long)b; r2 = (long)l;
        __asm__ volatile("svc #0"
            : "+r"(r0) : "r"(r7), "r"(r1), "r"(r2) : "memory");
        n = r0;
        if (n < 0) return total ? total : -1;
        if (n == 0) break;
        total += n; b += n; l -= (unsigned long)n;
    }
    return total;
}

/* ---- Linux riscv64: ecall, syscall 64 = write ---- */
#elif ROC_ARCH_RV64
static inline long roc_raw_write(long fd, const char* b, unsigned long l) {
    long total = 0, n;
    register long a0 __asm__("a0");
    register long a1 __asm__("a1");
    register long a2 __asm__("a2");
    register long a7 __asm__("a7");
    while (l > 0) {
        a0 = fd; a1 = (long)b; a2 = (long)l; a7 = 64;
        __asm__ volatile("ecall"
            : "+r"(a0) : "r"(a1), "r"(a2), "r"(a7) : "memory");
        n = a0;
        if (n < 0) return total ? total : -1;
        if (n == 0) break;
        total += n; b += n; l -= (unsigned long)n;
    }
    return total;
}

/* ---- macOS x86_64: syscall with 0x2000000 prefix, write = 4 ---- */
#elif ROC_MAC_X64
static inline long roc_raw_write(long fd, const char* b, unsigned long l) {
    long total = 0, n;
    while (l > 0) {
        __asm__ volatile("syscall"
            : "=a"(n)
            : "a"(0x2000004L), "D"(fd), "S"((long)b), "d"((long)l)
            : "rcx", "r11", "memory");
        if (n < 0) return total ? total : -1;
        if (n == 0) break;
        total += n; b += n; l -= (unsigned long)n;
    }
    return total;
}

/* ---- macOS arm64: svc #0x80, write = 4 ---- */
#elif ROC_MAC_ARM64
static inline long roc_raw_write(long fd, const char* b, unsigned long l) {
    long total = 0, n;
    register long x16 __asm__("x16");
    register long x0 __asm__("x0");
    register long x1 __asm__("x1");
    register long x2 __asm__("x2");
    while (l > 0) {
        x16 = 4; x0 = fd; x1 = (long)b; x2 = (long)l;
        __asm__ volatile("svc #0x80"
            : "+r"(x0) : "r"(x16), "r"(x1), "r"(x2) : "memory");
        n = x0;
        if (n < 0) return total ? total : -1;
        if (n == 0) break;
        total += n; b += n; l -= (unsigned long)n;
    }
    return total;
}

/* ---- FreeBSD x86_64: syscall 4 = write ---- */
#elif ROC_PLAT_FBSD && defined(__x86_64__)
static inline long roc_raw_write(long fd, const char* b, unsigned long l) {
    long total = 0, n;
    while (l > 0) {
        __asm__ volatile("syscall"
            : "=a"(n)
            : "a"(4L), "D"(fd), "S"((long)b), "d"((long)l)
            : "rcx", "r11", "memory");
        if (n < 0) return total ? total : -1;
        if (n == 0) break;
        total += n; b += n; l -= (unsigned long)n;
    }
    return total;
}

#else
static inline long roc_raw_write(long fd, const char* b, unsigned long l) {
    (void)fd; (void)b; (void)l; return -1;
}
#endif

#endif /* roc_stdout || roc_stderror || roc_fileout */

/* ====================================================================== */
/* stdout                                                                 */
/* ====================================================================== */
#ifdef roc_stdout
static inline long roc_stdout_write(const char* b, unsigned long l) {
    return roc_raw_write(1, b, l);
}
#endif

/* ====================================================================== */
/* stderr                                                                 */
/* ====================================================================== */
#ifdef roc_stderror
static inline long roc_stderr_write(const char* b, unsigned long l) {
    return roc_raw_write(2, b, l);
}
#endif

/* ====================================================================== */
/* File                                                                   */
/* ====================================================================== */
#ifdef roc_fileout

#if ROC_PLAT_WIN

static inline long roc_file(const char* path, const char* buf, unsigned long len) {
    void* h; unsigned long done = 0;
    h = CreateFileA(path, 0x40000000UL, 3, 0, 2, 0, 0);
    if (h == (void*)-1) return -1;
    while (done < len) {
        unsigned long chunk = (len - done > 0x7FFFFFFFUL) ? 0x7FFFFFFFUL : (len - done);
        unsigned long w = 0;
        if (!WriteFile(h, buf + done, chunk, &w, 0)) { CloseHandle(h); return -1; }
        if (w == 0) break;
        done += w;
    }
    CloseHandle(h);
    return (long)done;
}

static inline long roc_file_append(const char* path, const char* buf, unsigned long len) {
    void* h; unsigned long done = 0;
    h = CreateFileA(path, 4, 3, 0, 4, 0, 0);
    if (h == (void*)-1) return -1;
    while (done < len) {
        unsigned long chunk = (len - done > 0x7FFFFFFFUL) ? 0x7FFFFFFFUL : (len - done);
        unsigned long w = 0;
        if (!WriteFile(h, buf + done, chunk, &w, 0)) { CloseHandle(h); return -1; }
        if (w == 0) break;
        done += w;
    }
    CloseHandle(h);
    return (long)done;
}

#elif ROC_ARCH_X64 || (ROC_PLAT_FBSD && defined(__x86_64__))

/* Linux x64: open = 2, close = 3, write = 1 */
/* FreeBSD x64: open = 5, close = 6, write = 4 */
static inline long roc_file(const char* path, const char* buf, unsigned long len) {
    long fd, total = 0, n;
#if ROC_PLAT_FBSD
    __asm__ volatile("syscall" : "=a"(fd)
        : "a"(5L), "D"((long)path), "S"(0x0601L), "d"(0644L)
        : "rcx", "r11", "memory");
#else
    __asm__ volatile("syscall" : "=a"(fd)
        : "a"(2L), "D"((long)path), "S"(577L), "d"(0644L)
        : "rcx", "r11", "memory");
#endif
    if (fd < 0) return -1;
    while (len > 0) {
#if ROC_PLAT_FBSD
        __asm__ volatile("syscall" : "=a"(n)
            : "a"(4L), "D"(fd), "S"((long)buf), "d"((long)len)
            : "rcx", "r11", "memory");
#else
        __asm__ volatile("syscall" : "=a"(n)
            : "a"(1L), "D"(fd), "S"((long)buf), "d"((long)len)
            : "rcx", "r11", "memory");
#endif
        if (n < 0) { total = total ? total : -1; break; }
        if (n == 0) break;
        total += n; buf += n; len -= (unsigned long)n;
    }
#if ROC_PLAT_FBSD
    __asm__ volatile("syscall" : : "a"(6L), "D"(fd) : "rcx","r11","memory");
#else
    __asm__ volatile("syscall" : : "a"(3L), "D"(fd) : "rcx","r11","memory");
#endif
    return total;
}

static inline long roc_file_append(const char* path, const char* buf, unsigned long len) {
    long fd, total = 0, n;
#if ROC_PLAT_FBSD
    __asm__ volatile("syscall" : "=a"(fd)
        : "a"(5L), "D"((long)path), "S"(0x0609L), "d"(0644L)
        : "rcx", "r11", "memory");
#else
    __asm__ volatile("syscall" : "=a"(fd)
        : "a"(2L), "D"((long)path), "S"(1089L), "d"(0644L)
        : "rcx", "r11", "memory");
#endif
    if (fd < 0) return -1;
    while (len > 0) {
#if ROC_PLAT_FBSD
        __asm__ volatile("syscall" : "=a"(n)
            : "a"(4L), "D"(fd), "S"((long)buf), "d"((long)len)
            : "rcx", "r11", "memory");
#else
        __asm__ volatile("syscall" : "=a"(n)
            : "a"(1L), "D"(fd), "S"((long)buf), "d"((long)len)
            : "rcx", "r11", "memory");
#endif
        if (n < 0) { total = total ? total : -1; break; }
        if (n == 0) break;
        total += n; buf += n; len -= (unsigned long)n;
    }
#if ROC_PLAT_FBSD
    __asm__ volatile("syscall" : : "a"(6L), "D"(fd) : "rcx","r11","memory");
#else
    __asm__ volatile("syscall" : : "a"(3L), "D"(fd) : "rcx","r11","memory");
#endif
    return total;
}

#elif ROC_ARCH_I386

/* open = 5, close = 6, write = 4 */
static inline long roc_file(const char* path, const char* buf, unsigned long len) {
    long fd, total = 0, n;
    __asm__ volatile("int $0x80" : "=a"(fd)
        : "a"(5L), "b"((long)path), "c"(577L), "d"(0644L)
        : "memory", "cc");
    if (fd < 0) return -1;
    while (len > 0) {
        __asm__ volatile("int $0x80" : "=a"(n)
            : "a"(4L), "b"(fd), "c"((long)buf), "d"((long)len)
            : "memory", "cc");
        if (n < 0) { total = total ? total : -1; break; }
        if (n == 0) break;
        total += n; buf += n; len -= (unsigned long)n;
    }
    __asm__ volatile("int $0x80" : : "a"(6L), "b"(fd) : "memory", "cc");
    return total;
}

static inline long roc_file_append(const char* path, const char* buf, unsigned long len) {
    long fd, total = 0, n;
    __asm__ volatile("int $0x80" : "=a"(fd)
        : "a"(5L), "b"((long)path), "c"(1089L), "d"(0644L)
        : "memory", "cc");
    if (fd < 0) return -1;
    while (len > 0) {
        __asm__ volatile("int $0x80" : "=a"(n)
            : "a"(4L), "b"(fd), "c"((long)buf), "d"((long)len)
            : "memory", "cc");
        if (n < 0) { total = total ? total : -1; break; }
        if (n == 0) break;
        total += n; buf += n; len -= (unsigned long)n;
    }
    __asm__ volatile("int $0x80" : : "a"(6L), "b"(fd) : "memory", "cc");
    return total;
}

#elif ROC_ARCH_ARM64

/* openat = 56, close = 57, write = 64 */
static inline long roc_file(const char* path, const char* buf, unsigned long len) {
    long fd, total = 0, n;
    register long x8 __asm__("x8");
    register long x0 __asm__("x0");
    register long x1 __asm__("x1");
    register long x2 __asm__("x2");
    x8 = 56; x0 = -100; x1 = (long)path; x2 = 577;
    __asm__ volatile("svc #0" : "+r"(x0) : "r"(x8), "r"(x1), "r"(x2) : "memory");
    fd = x0;
    if (fd < 0) return -1;
    while (len > 0) {
        x8 = 64; x0 = fd; x1 = (long)buf; x2 = (long)len;
        __asm__ volatile("svc #0" : "+r"(x0) : "r"(x8), "r"(x1), "r"(x2) : "memory");
        n = x0;
        if (n < 0) { total = total ? total : -1; break; }
        if (n == 0) break;
        total += n; buf += n; len -= (unsigned long)n;
    }
    x8 = 57; x0 = fd;
    __asm__ volatile("svc #0" : "+r"(x0) : "r"(x8) : "memory");
    return total;
}

static inline long roc_file_append(const char* path, const char* buf, unsigned long len) {
    long fd, total = 0, n;
    register long x8 __asm__("x8");
    register long x0 __asm__("x0");
    register long x1 __asm__("x1");
    register long x2 __asm__("x2");
    x8 = 56; x0 = -100; x1 = (long)path; x2 = 1089;
    __asm__ volatile("svc #0" : "+r"(x0) : "r"(x8), "r"(x1), "r"(x2) : "memory");
    fd = x0;
    if (fd < 0) return -1;
    while (len > 0) {
        x8 = 64; x0 = fd; x1 = (long)buf; x2 = (long)len;
        __asm__ volatile("svc #0" : "+r"(x0) : "r"(x8), "r"(x1), "r"(x2) : "memory");
        n = x0;
        if (n < 0) { total = total ? total : -1; break; }
        if (n == 0) break;
        total += n; buf += n; len -= (unsigned long)n;
    }
    x8 = 57; x0 = fd;
    __asm__ volatile("svc #0" : "+r"(x0) : "r"(x8) : "memory");
    return total;
}

#elif ROC_ARCH_ARMV7

/* open = 5, close = 6, write = 4 */
static inline long roc_file(const char* path, const char* buf, unsigned long len) {
    long fd, total = 0, n;
    register long r7 __asm__("r7");
    register long r0 __asm__("r0");
    register long r1 __asm__("r1");
    register long r2 __asm__("r2");
    r7 = 5; r0 = (long)path; r1 = 577; r2 = 0644;
    __asm__ volatile("svc #0" : "+r"(r0) : "r"(r7), "r"(r1), "r"(r2) : "memory");
    fd = r0;
    if (fd < 0) return -1;
    while (len > 0) {
        r7 = 4; r0 = fd; r1 = (long)buf; r2 = (long)len;
        __asm__ volatile("svc #0" : "+r"(r0) : "r"(r7), "r"(r1), "r"(r2) : "memory");
        n = r0;
        if (n < 0) { total = total ? total : -1; break; }
        if (n == 0) break;
        total += n; buf += n; len -= (unsigned long)n;
    }
    r7 = 6; r0 = fd;
    __asm__ volatile("svc #0" : "+r"(r0) : "r"(r7) : "memory");
    return total;
}

static inline long roc_file_append(const char* path, const char* buf, unsigned long len) {
    long fd, total = 0, n;
    register long r7 __asm__("r7");
    register long r0 __asm__("r0");
    register long r1 __asm__("r1");
    register long r2 __asm__("r2");
    r7 = 5; r0 = (long)path; r1 = 1089; r2 = 0644;
    __asm__ volatile("svc #0" : "+r"(r0) : "r"(r7), "r"(r1), "r"(r2) : "memory");
    fd = r0;
    if (fd < 0) return -1;
    while (len > 0) {
        r7 = 4; r0 = fd; r1 = (long)buf; r2 = (long)len;
        __asm__ volatile("svc #0" : "+r"(r0) : "r"(r7), "r"(r1), "r"(r2) : "memory");
        n = r0;
        if (n < 0) { total = total ? total : -1; break; }
        if (n == 0) break;
        total += n; buf += n; len -= (unsigned long)n;
    }
    r7 = 6; r0 = fd;
    __asm__ volatile("svc #0" : "+r"(r0) : "r"(r7) : "memory");
    return total;
}

#elif ROC_ARCH_RV64

/* openat = 56, close = 57, write = 64 */
static inline long roc_file(const char* path, const char* buf, unsigned long len) {
    long fd, total = 0, n;
    register long a0 __asm__("a0");
    register long a1 __asm__("a1");
    register long a2 __asm__("a2");
    register long a7 __asm__("a7");
    a0 = -100; a1 = (long)path; a2 = 577; a7 = 56;
    __asm__ volatile("ecall" : "+r"(a0) : "r"(a1), "r"(a2), "r"(a7) : "memory");
    fd = a0;
    if (fd < 0) return -1;
    while (len > 0) {
        a0 = fd; a1 = (long)buf; a2 = (long)len; a7 = 64;
        __asm__ volatile("ecall" : "+r"(a0) : "r"(a1), "r"(a2), "r"(a7) : "memory");
        n = a0;
        if (n < 0) { total = total ? total : -1; break; }
        if (n == 0) break;
        total += n; buf += n; len -= (unsigned long)n;
    }
    a0 = fd; a7 = 57;
    __asm__ volatile("ecall" : "+r"(a0) : "r"(a7) : "memory");
    return total;
}

static inline long roc_file_append(const char* path, const char* buf, unsigned long len) {
    long fd, total = 0, n;
    register long a0 __asm__("a0");
    register long a1 __asm__("a1");
    register long a2 __asm__("a2");
    register long a7 __asm__("a7");
    a0 = -100; a1 = (long)path; a2 = 1089; a7 = 56;
    __asm__ volatile("ecall" : "+r"(a0) : "r"(a1), "r"(a2), "r"(a7) : "memory");
    fd = a0;
    if (fd < 0) return -1;
    while (len > 0) {
        a0 = fd; a1 = (long)buf; a2 = (long)len; a7 = 64;
        __asm__ volatile("ecall" : "+r"(a0) : "r"(a1), "r"(a2), "r"(a7) : "memory");
        n = a0;
        if (n < 0) { total = total ? total : -1; break; }
        if (n == 0) break;
        total += n; buf += n; len -= (unsigned long)n;
    }
    a0 = fd; a7 = 57;
    __asm__ volatile("ecall" : "+r"(a0) : "r"(a7) : "memory");
    return total;
}

#elif ROC_MAC_X64

/* macOS: open = 5, close = 6, write = 4, all with 0x2000000 prefix */
static inline long roc_file(const char* path, const char* buf, unsigned long len) {
    long fd, total = 0, n;
    __asm__ volatile("syscall" : "=a"(fd)
        : "a"(0x2000005L), "D"((long)path), "S"(0x0601L), "d"(0644L)
        : "rcx", "r11", "memory");
    if (fd < 0) return -1;
    while (len > 0) {
        __asm__ volatile("syscall" : "=a"(n)
            : "a"(0x2000004L), "D"(fd), "S"((long)buf), "d"((long)len)
            : "rcx", "r11", "memory");
        if (n < 0) { total = total ? total : -1; break; }
        if (n == 0) break;
        total += n; buf += n; len -= (unsigned long)n;
    }
    __asm__ volatile("syscall" : : "a"(0x2000006L), "D"(fd) : "rcx","r11","memory");
    return total;
}

static inline long roc_file_append(const char* path, const char* buf, unsigned long len) {
    long fd, total = 0, n;
    __asm__ volatile("syscall" : "=a"(fd)
        : "a"(0x2000005L), "D"((long)path), "S"(0x0609L), "d"(0644L)
        : "rcx", "r11", "memory");
    if (fd < 0) return -1;
    while (len > 0) {
        __asm__ volatile("syscall" : "=a"(n)
            : "a"(0x2000004L), "D"(fd), "S"((long)buf), "d"((long)len)
            : "rcx", "r11", "memory");
        if (n < 0) { total = total ? total : -1; break; }
        if (n == 0) break;
        total += n; buf += n; len -= (unsigned long)n;
    }
    __asm__ volatile("syscall" : : "a"(0x2000006L), "D"(fd) : "rcx","r11","memory");
    return total;
}

#elif ROC_MAC_ARM64

/* macOS arm64: open = 5, close = 6, write = 4, svc #0x80 */
static inline long roc_file(const char* path, const char* buf, unsigned long len) {
    long fd, total = 0, n;
    register long x16 __asm__("x16");
    register long x0 __asm__("x0");
    register long x1 __asm__("x1");
    register long x2 __asm__("x2");
    register long x3 __asm__("x3");
    x16 = 5; x0 = (long)path; x1 = 0x0601; x2 = 0644;
    __asm__ volatile("svc #0x80" : "+r"(x0) : "r"(x16), "r"(x1), "r"(x2), "r"(x3) : "memory");
    fd = x0;
    if (fd < 0) return -1;
    while (len > 0) {
        x16 = 4; x0 = fd; x1 = (long)buf; x2 = (long)len;
        __asm__ volatile("svc #0x80" : "+r"(x0) : "r"(x16), "r"(x1), "r"(x2) : "memory");
        n = x0;
        if (n < 0) { total = total ? total : -1; break; }
        if (n == 0) break;
        total += n; buf += n; len -= (unsigned long)n;
    }
    x16 = 6; x0 = fd;
    __asm__ volatile("svc #0x80" : "+r"(x0) : "r"(x16) : "memory");
    return total;
}

static inline long roc_file_append(const char* path, const char* buf, unsigned long len) {
    long fd, total = 0, n;
    register long x16 __asm__("x16");
    register long x0 __asm__("x0");
    register long x1 __asm__("x1");
    register long x2 __asm__("x2");
    register long x3 __asm__("x3");
    x16 = 5; x0 = (long)path; x1 = 0x0609; x2 = 0644;
    __asm__ volatile("svc #0x80" : "+r"(x0) : "r"(x16), "r"(x1), "r"(x2), "r"(x3) : "memory");
    fd = x0;
    if (fd < 0) return -1;
    while (len > 0) {
        x16 = 4; x0 = fd; x1 = (long)buf; x2 = (long)len;
        __asm__ volatile("svc #0x80" : "+r"(x0) : "r"(x16), "r"(x1), "r"(x2) : "memory");
        n = x0;
        if (n < 0) { total = total ? total : -1; break; }
        if (n == 0) break;
        total += n; buf += n; len -= (unsigned long)n;
    }
    x16 = 6; x0 = fd;
    __asm__ volatile("svc #0x80" : "+r"(x0) : "r"(x16) : "memory");
    return total;
}

#else
static inline long roc_file(const char* path, const char* buf, unsigned long len) {
    (void)path; (void)buf; (void)len; return -1;
}
static inline long roc_file_append(const char* path, const char* buf, unsigned long len) {
    (void)path; (void)buf; (void)len; return -1;
}
#endif

#endif /* roc_fileout */

/* ====================================================================== */
/* Log                                                                    */
/* ====================================================================== */
#ifdef roc_logout
#ifdef roc_fileout

static inline unsigned long roc_utoa(unsigned long v, char* out) {
    char tmp[24]; int i = 0, j;
    if (v == 0) { out[0] = '0'; return 1; }
    while (v) { tmp[i++] = (char)('0' + (v % 10)); v /= 10; }
    for (j = 0; j < i; j++) out[j] = tmp[i - 1 - j];
    return (unsigned long)i;
}

#ifdef ROC_LOG_CONFIG_EXTERN
extern int roc_log_level;
extern unsigned long roc_clock_value;
extern int roc_clock_valid;
extern long (*roc_time_impl)(char* buf, unsigned long cap);
#else
static int roc_log_level = ROC_LOG_INFO;
static unsigned long roc_clock_value = 0;
static int roc_clock_valid = 0;
static long (*roc_time_impl)(char* buf, unsigned long cap) = 0;
#endif

static inline long roc_log_write(const char* path, int level,
                                 const char* msg, unsigned long msg_len) {
    char line[512];
    unsigned long p = 0;
    if (!path || !msg) return -1;
    if (level < 0 || level > ROC_LOG_FATAL) level = ROC_LOG_INFO;
    if (level < roc_log_level) return 0;

    line[p++] = '[';
    if (roc_time_impl) {
        unsigned long remain = (unsigned long)sizeof(line) - p;
        long t = roc_time_impl(line + p, remain);
        if (t > 0 && (unsigned long)t <= remain) p += (unsigned long)t;
        else line[p++] = '-';
    } else if (roc_clock_valid) {
        p += roc_utoa(roc_clock_value, line + p);
    } else {
        line[p++] = '-';
    }
    line[p++] = ']'; line[p++] = ' ';

    line[p++] = '[';
    {
        const char* lv =
            (level == ROC_LOG_ERROR) ? "ERROR" :
            (level == ROC_LOG_WARN ) ? "WARN " :
            (level == ROC_LOG_DEBUG) ? "DEBUG" :
            (level == ROC_LOG_FATAL) ? "FATAL" : "INFO ";
        while (*lv && p < sizeof(line) - 2) line[p++] = *lv++;
    }
    line[p++] = ']'; line[p++] = ' ';

    {
        unsigned long room = (unsigned long)sizeof(line) - p - 1;
        unsigned long n = msg_len < room ? msg_len : room;
        unsigned long i;
        for (i = 0; i < n; i++) line[p++] = msg[i];
    }
    line[p++] = '\n';

    return roc_file_append(path, line, p);
}

static inline long roc_log_debug(const char* path, const char* m, unsigned long n) {
    return roc_log_write(path, ROC_LOG_DEBUG, m, n);
}
static inline long roc_log_info(const char* path, const char* m, unsigned long n) {
    return roc_log_write(path, ROC_LOG_INFO, m, n);
}
static inline long roc_log_warn(const char* path, const char* m, unsigned long n) {
    return roc_log_write(path, ROC_LOG_WARN, m, n);
}
static inline long roc_log_error(const char* path, const char* m, unsigned long n) {
    return roc_log_write(path, ROC_LOG_ERROR, m, n);
}
static inline long roc_log_fatal(const char* path, const char* m, unsigned long n) {
    return roc_log_write(path, ROC_LOG_FATAL, m, n);
}

#endif
#endif

/* ====================================================================== */
/* Network (TCP client, send only)                                        */
/* ====================================================================== */
#ifdef roc_netout

struct roc_sock {
    ROC_SOCK_T fd;
    int is_udp;
    unsigned short port;
};

#if ROC_PLAT_WIN

#ifdef ROC_LOG_CONFIG_EXTERN
extern int roc_wsa_ready;
#else
static int roc_wsa_ready = 0;
#endif

static inline long roc_net_connect(struct roc_sock* s, unsigned long ip, unsigned short port) {
    ROC_SOCK_T fd;
    unsigned char sa[16];
    int i, r;
    if (!s) return -1;
    s->fd = ROC_SOCK_INVALID;

    if (!roc_wsa_ready) {
        union { unsigned char raw[408]; long long align; } wsadata;
        if (WSAStartup(0x0202, wsadata.raw) != 0) return -1;
        roc_wsa_ready = 1;
    }

    fd = socket(2, 1, 0);
    if (fd == ROC_SOCK_INVALID) return -1;

    sa[0] = 2; sa[1] = 0;
    sa[2] = (unsigned char)(port >> 8);
    sa[3] = (unsigned char)(port & 0xFF);
    sa[4] = (unsigned char)(ip >> 24);
    sa[5] = (unsigned char)(ip >> 16);
    sa[6] = (unsigned char)(ip >> 8);
    sa[7] = (unsigned char)(ip);
    for (i = 8; i < 16; i++) sa[i] = 0;

    r = connect(fd, sa, 16);
    if (r != 0) { closesocket(fd); return -1; }
    s->fd = fd; s->is_udp = 0; s->port = port;
    return 0;
}

static inline long roc_net_send(struct roc_sock* s, const char* b, unsigned long l) {
    if (!s || !ROC_SOCK_IS_VALID(s->fd)) return -1;
    {
        unsigned long done = 0;
        while (done < l) {
            int chunk = (l - done > 0x7FFFFFFFUL) ? 0x7FFFFFFF : (int)(l - done);
            int n = send(s->fd, b + done, chunk, 0);
            if (n < 0) return done ? (long)done : -1;
            if (n == 0) break;
            done += (unsigned long)n;
        }
        return (long)done;
    }
}

static inline void roc_net_close(struct roc_sock* s) {
    if (!s || !ROC_SOCK_IS_VALID(s->fd)) return;
    closesocket(s->fd);
    s->fd = ROC_SOCK_INVALID;
}

static inline void roc_net_cleanup(void) {
    if (roc_wsa_ready) { WSACleanup(); roc_wsa_ready = 0; }
}

#elif ROC_ARCH_X64

/* socket = 41, connect = 42, sendto = 44, close = 3 */
static inline long roc_net_connect(struct roc_sock* s, unsigned long ip, unsigned short port) {
    long fd, r;
    unsigned char sa[16];
    int i;
    if (!s) return -1;
    s->fd = ROC_SOCK_INVALID;

    __asm__ volatile("syscall" : "=a"(fd)
        : "a"(41L), "D"(2L), "S"(1L), "d"(0L)
        : "rcx", "r11", "memory");
    if (fd < 0) return -1;

    sa[0] = 2; sa[1] = 0;
    sa[2] = (unsigned char)(port >> 8);
    sa[3] = (unsigned char)(port & 0xFF);
    sa[4] = (unsigned char)(ip >> 24);
    sa[5] = (unsigned char)(ip >> 16);
    sa[6] = (unsigned char)(ip >> 8);
    sa[7] = (unsigned char)(ip);
    for (i = 8; i < 16; i++) sa[i] = 0;

    __asm__ volatile("syscall" : "=a"(r)
        : "a"(42L), "D"(fd), "S"((long)sa), "d"(16L)
        : "rcx", "r11", "memory");
    if (r < 0) {
        __asm__ volatile("syscall" : : "a"(3L), "D"(fd) : "rcx","r11","memory");
        return -1;
    }
    s->fd = fd; s->is_udp = 0; s->port = port;
    return 0;
}

static inline long roc_net_send(struct roc_sock* s, const char* b, unsigned long l) {
    if (!s || !ROC_SOCK_IS_VALID(s->fd)) return -1;
    {
        unsigned long done = 0;
        while (done < l) {
            long n;
            __asm__ volatile("syscall" : "=a"(n)
                : "a"(44L), "D"(s->fd), "S"((long)(b + done)),
                  "d"((long)(l - done)), "r10"(0L), "r8"(0L), "r9"(0L)
                : "rcx", "r11", "memory");
            if (n < 0) return done ? (long)done : -1;
            if (n == 0) break;
            done += (unsigned long)n;
        }
        return (long)done;
    }
}

static inline void roc_net_close(struct roc_sock* s) {
    if (!s || !ROC_SOCK_IS_VALID(s->fd)) return;
    __asm__ volatile("syscall" : : "a"(3L), "D"(s->fd) : "rcx","r11","memory");
    s->fd = ROC_SOCK_INVALID;
}

static inline void roc_net_cleanup(void) { }

#elif ROC_ARCH_I386

/* socketcall-based: socketcall = 102, sub 1=socket, 3=connect, 9=send */
static inline long roc_net_connect(struct roc_sock* s, unsigned long ip, unsigned short port) {
    long fd, r;
    unsigned char sa[16];
    unsigned long args[3];
    int i;
    if (!s) return -1;
    s->fd = ROC_SOCK_INVALID;

    args[0] = 2; args[1] = 1; args[2] = 0;
    __asm__ volatile("int $0x80" : "=a"(fd)
        : "a"(102L), "b"(1L), "c"((long)args) : "memory", "cc");
    if (fd < 0) return -1;

    sa[0] = 2; sa[1] = 0;
    sa[2] = (unsigned char)(port >> 8);
    sa[3] = (unsigned char)(port & 0xFF);
    sa[4] = (unsigned char)(ip >> 24);
    sa[5] = (unsigned char)(ip >> 16);
    sa[6] = (unsigned char)(ip >> 8);
    sa[7] = (unsigned char)(ip);
    for (i = 8; i < 16; i++) sa[i] = 0;

    args[0] = (unsigned long)fd;
    args[1] = (unsigned long)sa;
    args[2] = 16;
    __asm__ volatile("int $0x80" : "=a"(r)
        : "a"(102L), "b"(3L), "c"((long)args) : "memory", "cc");
    if (r < 0) {
        __asm__ volatile("int $0x80" : : "a"(6L), "b"(fd) : "memory", "cc");
        return -1;
    }
    s->fd = fd; s->is_udp = 0; s->port = port;
    return 0;
}

static inline long roc_net_send(struct roc_sock* s, const char* b, unsigned long l) {
    if (!s || !ROC_SOCK_IS_VALID(s->fd)) return -1;
    {
        unsigned long done = 0;
        while (done < l) {
            long n;
            unsigned long args[4];
            args[0] = (unsigned long)s->fd;
            args[1] = (unsigned long)(b + done);
            args[2] = l - done;
            args[3] = 0;
            __asm__ volatile("int $0x80" : "=a"(n)
                : "a"(102L), "b"(9L), "c"((long)args) : "memory", "cc");
            if (n < 0) return done ? (long)done : -1;
            if (n == 0) break;
            done += (unsigned long)n;
        }
        return (long)done;
    }
}

static inline void roc_net_close(struct roc_sock* s) {
    if (!s || !ROC_SOCK_IS_VALID(s->fd)) return;
    __asm__ volatile("int $0x80" : : "a"(6L), "b"(s->fd) : "memory", "cc");
    s->fd = ROC_SOCK_INVALID;
}

static inline void roc_net_cleanup(void) { }

#elif ROC_ARCH_ARM64

/* socket = 198, connect = 203, sendto = 206, close = 57 */
static inline long roc_net_connect(struct roc_sock* s, unsigned long ip, unsigned short port) {
    long fd, r;
    unsigned char sa[16];
    int i;
    register long x8 __asm__("x8");
    register long x0 __asm__("x0");
    register long x1 __asm__("x1");
    register long x2 __asm__("x2");
    if (!s) return -1;
    s->fd = ROC_SOCK_INVALID;

    x8 = 198; x0 = 2; x1 = 1; x2 = 0;
    __asm__ volatile("svc #0" : "+r"(x0) : "r"(x8), "r"(x1), "r"(x2) : "memory");
    fd = x0;
    if (fd < 0) return -1;

    sa[0] = 2; sa[1] = 0;
    sa[2] = (unsigned char)(port >> 8);
    sa[3] = (unsigned char)(port & 0xFF);
    sa[4] = (unsigned char)(ip >> 24);
    sa[5] = (unsigned char)(ip >> 16);
    sa[6] = (unsigned char)(ip >> 8);
    sa[7] = (unsigned char)(ip);
    for (i = 8; i < 16; i++) sa[i] = 0;

    x8 = 203; x0 = fd; x1 = (long)sa; x2 = 16;
    __asm__ volatile("svc #0" : "+r"(x0) : "r"(x8), "r"(x1), "r"(x2) : "memory");
    r = x0;
    if (r < 0) {
        x8 = 57; x0 = fd;
        __asm__ volatile("svc #0" : "+r"(x0) : "r"(x8) : "memory");
        return -1;
    }
    s->fd = fd; s->is_udp = 0; s->port = port;
    return 0;
}

static inline long roc_net_send(struct roc_sock* s, const char* b, unsigned long l) {
    if (!s || !ROC_SOCK_IS_VALID(s->fd)) return -1;
    {
        unsigned long done = 0;
        register long x8 __asm__("x8");
        register long x0 __asm__("x0");
        register long x1 __asm__("x1");
        register long x2 __asm__("x2");
        register long x3 __asm__("x3");
        register long x4 __asm__("x4");
        register long x5 __asm__("x5");
        while (done < l) {
            x8 = 206; x0 = s->fd; x1 = (long)(b + done);
            x2 = (long)(l - done); x3 = 0; x4 = 0; x5 = 0;
            __asm__ volatile("svc #0" : "+r"(x0)
                : "r"(x8), "r"(x1), "r"(x2), "r"(x3), "r"(x4), "r"(x5)
                : "memory");
            if (x0 < 0) return done ? (long)done : -1;
            if (x0 == 0) break;
            done += (unsigned long)x0;
        }
        return (long)done;
    }
}

static inline void roc_net_close(struct roc_sock* s) {
    if (!s || !ROC_SOCK_IS_VALID(s->fd)) return;
    {
        register long x8 __asm__("x8");
        register long x0 __asm__("x0");
        x8 = 57; x0 = s->fd;
        __asm__ volatile("svc #0" : "+r"(x0) : "r"(x8) : "memory");
    }
    s->fd = ROC_SOCK_INVALID;
}

static inline void roc_net_cleanup(void) { }

#elif ROC_ARCH_ARMV7

/* socket = 281, connect = 283, sendto = 290, close = 6 (EABI) */
static inline long roc_net_connect(struct roc_sock* s, unsigned long ip, unsigned short port) {
    long fd, r;
    unsigned char sa[16];
    int i;
    register long r7 __asm__("r7");
    register long r0 __asm__("r0");
    register long r1 __asm__("r1");
    register long r2 __asm__("r2");
    if (!s) return -1;
    s->fd = ROC_SOCK_INVALID;

    r7 = 281; r0 = 2; r1 = 1; r2 = 0;
    __asm__ volatile("svc #0" : "+r"(r0) : "r"(r7), "r"(r1), "r"(r2) : "memory");
    fd = r0;
    if (fd < 0) return -1;

    sa[0] = 2; sa[1] = 0;
    sa[2] = (unsigned char)(port >> 8);
    sa[3] = (unsigned char)(port & 0xFF);
    sa[4] = (unsigned char)(ip >> 24);
    sa[5] = (unsigned char)(ip >> 16);
    sa[6] = (unsigned char)(ip >> 8);
    sa[7] = (unsigned char)(ip);
    for (i = 8; i < 16; i++) sa[i] = 0;

    r7 = 283; r0 = fd; r1 = (long)sa; r2 = 16;
    __asm__ volatile("svc #0" : "+r"(r0) : "r"(r7), "r"(r1), "r"(r2) : "memory");
    r = r0;
    if (r < 0) {
        r7 = 6; r0 = fd;
        __asm__ volatile("svc #0" : "+r"(r0) : "r"(r7) : "memory");
        return -1;
    }
    s->fd = fd; s->is_udp = 0; s->port = port;
    return 0;
}

static inline long roc_net_send(struct roc_sock* s, const char* b, unsigned long l) {
    if (!s || !ROC_SOCK_IS_VALID(s->fd)) return -1;
    {
        unsigned long done = 0;
        register long r7 __asm__("r7");
        register long r0 __asm__("r0");
        register long r1 __asm__("r1");
        register long r2 __asm__("r2");
        register long r3 __asm__("r3");
        register long r4 __asm__("r4");
        register long r5 __asm__("r5");
        while (done < l) {
            r7 = 290; r0 = s->fd; r1 = (long)(b + done);
            r2 = (long)(l - done); r3 = 0; r4 = 0; r5 = 0;
            __asm__ volatile("svc #0" : "+r"(r0)
                : "r"(r7), "r"(r1), "r"(r2), "r"(r3), "r"(r4), "r"(r5)
                : "memory");
            if (r0 < 0) return done ? (long)done : -1;
            if (r0 == 0) break;
            done += (unsigned long)r0;
        }
        return (long)done;
    }
}

static inline void roc_net_close(struct roc_sock* s) {
    if (!s || !ROC_SOCK_IS_VALID(s->fd)) return;
    {
        register long r7 __asm__("r7");
        register long r0 __asm__("r0");
        r7 = 6; r0 = s->fd;
        __asm__ volatile("svc #0" : "+r"(r0) : "r"(r7) : "memory");
    }
    s->fd = ROC_SOCK_INVALID;
}

static inline void roc_net_cleanup(void) { }

#elif ROC_ARCH_RV64

/* socket = 198, connect = 203, sendto = 206, close = 57 */
static inline long roc_net_connect(struct roc_sock* s, unsigned long ip, unsigned short port) {
    long fd, r;
    unsigned char sa[16];
    int i;
    register long a0 __asm__("a0");
    register long a1 __asm__("a1");
    register long a2 __asm__("a2");
    register long a7 __asm__("a7");
    if (!s) return -1;
    s->fd = ROC_SOCK_INVALID;

    a0 = 2; a1 = 1; a2 = 0; a7 = 198;
    __asm__ volatile("ecall" : "+r"(a0) : "r"(a1), "r"(a2), "r"(a7) : "memory");
    fd = a0;
    if (fd < 0) return -1;

    sa[0] = 2; sa[1] = 0;
    sa[2] = (unsigned char)(port >> 8);
    sa[3] = (unsigned char)(port & 0xFF);
    sa[4] = (unsigned char)(ip >> 24);
    sa[5] = (unsigned char)(ip >> 16);
    sa[6] = (unsigned char)(ip >> 8);
    sa[7] = (unsigned char)(ip);
    for (i = 8; i < 16; i++) sa[i] = 0;

    a0 = fd; a1 = (long)sa; a2 = 16; a7 = 203;
    __asm__ volatile("ecall" : "+r"(a0) : "r"(a1), "r"(a2), "r"(a7) : "memory");
    r = a0;
    if (r < 0) {
        a0 = fd; a7 = 57;
        __asm__ volatile("ecall" : "+r"(a0) : "r"(a7) : "memory");
        return -1;
    }
    s->fd = fd; s->is_udp = 0; s->port = port;
    return 0;
}

static inline long roc_net_send(struct roc_sock* s, const char* b, unsigned long l) {
    if (!s || !ROC_SOCK_IS_VALID(s->fd)) return -1;
    {
        unsigned long done = 0;
        register long a0 __asm__("a0");
        register long a1 __asm__("a1");
        register long a2 __asm__("a2");
        register long a3 __asm__("a3");
        register long a4 __asm__("a4");
        register long a5 __asm__("a5");
        register long a7 __asm__("a7");
        while (done < l) {
            a0 = s->fd; a1 = (long)(b + done); a2 = (long)(l - done);
            a3 = 0; a4 = 0; a5 = 0; a7 = 206;
            __asm__ volatile("ecall" : "+r"(a0)
                : "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5), "r"(a7)
                : "memory");
            if (a0 < 0) return done ? (long)done : -1;
            if (a0 == 0) break;
            done += (unsigned long)a0;
        }
        return (long)done;
    }
}

static inline void roc_net_close(struct roc_sock* s) {
    if (!s || !ROC_SOCK_IS_VALID(s->fd)) return;
    {
        register long a0 __asm__("a0");
        register long a7 __asm__("a7");
        a0 = s->fd; a7 = 57;
        __asm__ volatile("ecall" : "+r"(a0) : "r"(a7) : "memory");
    }
    s->fd = ROC_SOCK_INVALID;
}

static inline void roc_net_cleanup(void) { }

#elif ROC_PLAT_FBSD && defined(__x86_64__)

/* FreeBSD x64: socket = 97, connect = 98, sendto = 133, close = 6 */
static inline long roc_net_connect(struct roc_sock* s, unsigned long ip, unsigned short port) {
    long fd, r;
    unsigned char sa[16];
    int i;
    if (!s) return -1;
    s->fd = ROC_SOCK_INVALID;

    __asm__ volatile("syscall" : "=a"(fd)
        : "a"(97L), "D"(2L), "S"(1L), "d"(0L)
        : "rcx", "r11", "memory");
    if (fd < 0) return -1;

    sa[0] = 2; sa[1] = 0;
    sa[2] = (unsigned char)(port >> 8);
    sa[3] = (unsigned char)(port & 0xFF);
    sa[4] = (unsigned char)(ip >> 24);
    sa[5] = (unsigned char)(ip >> 16);
    sa[6] = (unsigned char)(ip >> 8);
    sa[7] = (unsigned char)(ip);
    for (i = 8; i < 16; i++) sa[i] = 0;

    __asm__ volatile("syscall" : "=a"(r)
        : "a"(98L), "D"(fd), "S"((long)sa), "d"(16L)
        : "rcx", "r11", "memory");
    if (r < 0) {
        __asm__ volatile("syscall" : : "a"(6L), "D"(fd) : "rcx","r11","memory");
        return -1;
    }
    s->fd = fd; s->is_udp = 0; s->port = port;
    return 0;
}

static inline long roc_net_send(struct roc_sock* s, const char* b, unsigned long l) {
    if (!s || !ROC_SOCK_IS_VALID(s->fd)) return -1;
    {
        unsigned long done = 0;
        while (done < l) {
            long n;
            __asm__ volatile("syscall" : "=a"(n)
                : "a"(133L), "D"(s->fd), "S"((long)(b + done)),
                  "d"((long)(l - done)), "r10"(0L), "r8"(0L), "r9"(0L)
                : "rcx", "r11", "memory");
            if (n < 0) return done ? (long)done : -1;
            if (n == 0) break;
            done += (unsigned long)n;
        }
        return (long)done;
    }
}

static inline void roc_net_close(struct roc_sock* s) {
    if (!s || !ROC_SOCK_IS_VALID(s->fd)) return;
    __asm__ volatile("syscall" : : "a"(6L), "D"(s->fd) : "rcx","r11","memory");
    s->fd = ROC_SOCK_INVALID;
}

static inline void roc_net_cleanup(void) { }

#elif ROC_MAC_X64

/* macOS: socket = 97, connect = 98, sendto = 133, close = 6 (0x2000000 prefix) */
static inline long roc_net_connect(struct roc_sock* s, unsigned long ip, unsigned short port) {
    long fd, r;
    unsigned char sa[16];
    int i;
    if (!s) return -1;
    s->fd = ROC_SOCK_INVALID;

    __asm__ volatile("syscall" : "=a"(fd)
        : "a"(0x2000061L), "D"(2L), "S"(1L), "d"(0L)
        : "rcx", "r11", "memory");
    if (fd < 0) return -1;

    sa[0] = 2; sa[1] = 0;
    sa[2] = (unsigned char)(port >> 8);
    sa[3] = (unsigned char)(port & 0xFF);
    sa[4] = (unsigned char)(ip >> 24);
    sa[5] = (unsigned char)(ip >> 16);
    sa[6] = (unsigned char)(ip >> 8);
    sa[7] = (unsigned char)(ip);
    for (i = 8; i < 16; i++) sa[i] = 0;

    __asm__ volatile("syscall" : "=a"(r)
        : "a"(0x2000062L), "D"(fd), "S"((long)sa), "d"(16L)
        : "rcx", "r11", "memory");
    if (r < 0) {
        __asm__ volatile("syscall" : : "a"(0x2000006L), "D"(fd) : "rcx","r11","memory");
        return -1;
    }
    s->fd = fd; s->is_udp = 0; s->port = port;
    return 0;
}

static inline long roc_net_send(struct roc_sock* s, const char* b, unsigned long l) {
    if (!s || !ROC_SOCK_IS_VALID(s->fd)) return -1;
    {
        unsigned long done = 0;
        while (done < l) {
            long n;
            __asm__ volatile("syscall" : "=a"(n)
                : "a"(0x2000085L), "D"(s->fd), "S"((long)(b + done)),
                  "d"((long)(l - done)), "r10"(0L), "r8"(0L), "r9"(0L)
                : "rcx", "r11", "memory");
            if (n < 0) return done ? (long)done : -1;
            if (n == 0) break;
            done += (unsigned long)n;
        }
        return (long)done;
    }
}

static inline void roc_net_close(struct roc_sock* s) {
    if (!s || !ROC_SOCK_IS_VALID(s->fd)) return;
    __asm__ volatile("syscall" : : "a"(0x2000006L), "D"(s->fd) : "rcx","r11","memory");
    s->fd = ROC_SOCK_INVALID;
}

static inline void roc_net_cleanup(void) { }

#elif ROC_MAC_ARM64

/* macOS arm64: socket = 97, connect = 98, sendto = 133, close = 6, svc #0x80 */
static inline long roc_net_connect(struct roc_sock* s, unsigned long ip, unsigned short port) {
    long fd, r;
    unsigned char sa[16];
    int i;
    register long x16 __asm__("x16");
    register long x0 __asm__("x0");
    register long x1 __asm__("x1");
    register long x2 __asm__("x2");
    if (!s) return -1;
    s->fd = ROC_SOCK_INVALID;

    x16 = 97; x0 = 2; x1 = 1; x2 = 0;
    __asm__ volatile("svc #0x80" : "+r"(x0) : "r"(x16), "r"(x1), "r"(x2) : "memory");
    fd = x0;
    if (fd < 0) return -1;

    sa[0] = 2; sa[1] = 0;
    sa[2] = (unsigned char)(port >> 8);
    sa[3] = (unsigned char)(port & 0xFF);
    sa[4] = (unsigned char)(ip >> 24);
    sa[5] = (unsigned char)(ip >> 16);
    sa[6] = (unsigned char)(ip >> 8);
    sa[7] = (unsigned char)(ip);
    for (i = 8; i < 16; i++) sa[i] = 0;

    x16 = 98; x0 = fd; x1 = (long)sa; x2 = 16;
    __asm__ volatile("svc #0x80" : "+r"(x0) : "r"(x16), "r"(x1), "r"(x2) : "memory");
    r = x0;
    if (r < 0) {
        x16 = 6; x0 = fd;
        __asm__ volatile("svc #0x80" : "+r"(x0) : "r"(x16) : "memory");
        return -1;
    }
    s->fd = fd; s->is_udp = 0; s->port = port;
    return 0;
}

static inline long roc_net_send(struct roc_sock* s, const char* b, unsigned long l) {
    if (!s || !ROC_SOCK_IS_VALID(s->fd)) return -1;
    {
        unsigned long done = 0;
        register long x16 __asm__("x16");
        register long x0 __asm__("x0");
        register long x1 __asm__("x1");
        register long x2 __asm__("x2");
        register long x3 __asm__("x3");
        register long x4 __asm__("x4");
        register long x5 __asm__("x5");
        while (done < l) {
            x16 = 133; x0 = s->fd; x1 = (long)(b + done);
            x2 = (long)(l - done); x3 = 0; x4 = 0; x5 = 0;
            __asm__ volatile("svc #0x80" : "+r"(x0)
                : "r"(x16), "r"(x1), "r"(x2), "r"(x3), "r"(x4), "r"(x5)
                : "memory");
            if (x0 < 0) return done ? (long)done : -1;
            if (x0 == 0) break;
            done += (unsigned long)x0;
        }
        return (long)done;
    }
}

static inline void roc_net_close(struct roc_sock* s) {
    if (!s || !ROC_SOCK_IS_VALID(s->fd)) return;
    {
        register long x16 __asm__("x16");
        register long x0 __asm__("x0");
        x16 = 6; x0 = s->fd;
        __asm__ volatile("svc #0x80" : "+r"(x0) : "r"(x16) : "memory");
    }
    s->fd = ROC_SOCK_INVALID;
}

static inline void roc_net_cleanup(void) { }

#else
static inline long roc_net_connect(struct roc_sock* s, unsigned long ip, unsigned short port) {
    (void)s; (void)ip; (void)port; return -1;
}
static inline long roc_net_send(struct roc_sock* s, const char* b, unsigned long l) {
    (void)s; (void)b; (void)l; return -1;
}
static inline void roc_net_close(struct roc_sock* s) { (void)s; }
static inline void roc_net_cleanup(void) { }
#endif

#endif /* roc_netout */

/* ====================================================================== */
/* Unified dispatch                                                       */
/* ====================================================================== */
static inline long roc_write(int channel, const void* ctx,
                             const char* buf, unsigned long len) {
    (void)ctx;
    switch (channel) {
    case ROC_STDOUT:
#ifdef roc_stdout
        return roc_stdout_write(buf, len);
#else
        return -1;
#endif
    case ROC_STDERR:
#ifdef roc_stderror
        return roc_stderr_write(buf, len);
#else
        return -1;
#endif
    case ROC_FILE:
#ifdef roc_fileout
        return roc_file((const char*)ctx, buf, len);
#else
        return -1;
#endif
    case ROC_LOG:
#ifdef roc_logout
        return roc_log_write((const char*)ctx, ROC_LOG_INFO, buf, len);
#else
        return -1;
#endif
    case ROC_NET:
#ifdef roc_netout
        return roc_net_send((struct roc_sock*)ctx, buf, len);
#else
        return -1;
#endif
    default:
        return -1;
    }
}

#endif /* rth_out_c */
