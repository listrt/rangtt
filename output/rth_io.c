/* ====================================================================== */
/* writer - Ultra-fast output function for 40 compilers (C99)             */
/* Returns: number of bytes written, or -1 if not supported               */
/* ====================================================================== */
#undef _start_rt
#define _start_rt int main(int argc,char* argv[]){

#undef _end_rt
#   ifdef __cplusplus
#       define _end_rt return {};}
#   else
#       define _end_rt return 0;}
#   endif


#ifndef rth_io_h
#define rth_io_h

/* ---------- 1. GCC ---------- */
#if defined(__GNUC__) && !defined(__clang__) && !defined(__INTEL_COMPILER) && !defined(__INTEL_LLVM_COMPILER) && !defined(__MINGW32__) && !defined(__MINGW64__) && !defined(__CYGWIN__)
    #if defined(_WIN32) || defined(_WIN64)
        #ifdef __cplusplus
        extern "C" {
        #endif
        __declspec(dllimport) void* __stdcall GetStdHandle(unsigned long);
        __declspec(dllimport) int __stdcall WriteFile(void*, const void*, unsigned long, unsigned long*, void*);
        #ifdef __cplusplus
        }
        #endif
        static inline long writer(const char* b, unsigned long l) {
            static void* h = (void*)0;
            if (!h) h = GetStdHandle((unsigned long)-11);
            if (h) {
                unsigned long w;
                if (WriteFile(h, b, l, &w, (void*)0)) return (long)w;
            }
            return -1;
        }
    #elif defined(__linux__) || defined(__unix__)
        #if defined(__x86_64__)
        static inline long writer(const char* b, unsigned long l) {
            long result;
            __asm__ volatile (
                "mov $1, %%rax\n"
                "mov $1, %%rdi\n"
                "mov %1, %%rsi\n"
                "mov %2, %%rdx\n"
                "syscall\n"
                "mov %%rax, %0"
                : "=r"(result)
                : "r"(b), "r"(l)
                : "rax", "rdi", "rsi", "rdx", "memory"
            );
            return result;
        }
        #elif defined(__aarch64__)
        static inline long writer(const char* b, unsigned long l) {
            long result;
            __asm__ volatile (
                "mov x8, #64\n"
                "mov x0, #1\n"
                "mov x1, %1\n"
                "mov x2, %2\n"
                "svc #0\n"
                "mov %0, x0"
                : "=r"(result)
                : "r"(b), "r"(l)
                : "x0", "x1", "x2", "x8", "memory"
            );
            return result;
        }
        #else
        static inline long writer(const char* b, unsigned long l) { (void)b; (void)l; return -1; }
        #endif
    #else
        static inline long writer(const char* b, unsigned long l) { (void)b; (void)l; return -1; }
    #endif

/* ---------- 2. Clang ---------- */
#elif defined(__clang__) && !defined(__INTEL_LLVM_COMPILER) && !defined(__AMD_COMPILER) && !defined(__ARMCC_VERSION) && !defined(__BORLANDC__)
    #if defined(_WIN32) || defined(_WIN64)
        #ifdef __cplusplus
        extern "C" {
        #endif
        __declspec(dllimport) void* __stdcall GetStdHandle(unsigned long);
        __declspec(dllimport) int __stdcall WriteFile(void*, const void*, unsigned long, unsigned long*, void*);
        #ifdef __cplusplus
        }
        #endif
        static inline long writer(const char* b, unsigned long l) {
            static void* h = (void*)0;
            if (!h) h = GetStdHandle((unsigned long)-11);
            if (h) {
                unsigned long w;
                if (WriteFile(h, b, l, &w, (void*)0)) return (long)w;
            }
            return -1;
        }
    #elif defined(__linux__) || defined(__unix__)
        #if defined(__x86_64__)
        static inline long writer(const char* b, unsigned long l) {
            long result;
            __asm__ volatile (
                "mov $1, %%rax\n"
                "mov $1, %%rdi\n"
                "mov %1, %%rsi\n"
                "mov %2, %%rdx\n"
                "syscall\n"
                "mov %%rax, %0"
                : "=r"(result)
                : "r"(b), "r"(l)
                : "rax", "rdi", "rsi", "rdx", "memory"
            );
            return result;
        }
        #elif defined(__aarch64__)
        static inline long writer(const char* b, unsigned long l) {
            long result;
            __asm__ volatile (
                "mov x8, #64\n"
                "mov x0, #1\n"
                "mov x1, %1\n"
                "mov x2, %2\n"
                "svc #0\n"
                "mov %0, x0"
                : "=r"(result)
                : "r"(b), "r"(l)
                : "x0", "x1", "x2", "x8", "memory"
            );
            return result;
        }
        #else
        static inline long writer(const char* b, unsigned long l) { (void)b; (void)l; return -1; }
        #endif
    #else
        static inline long writer(const char* b, unsigned long l) { (void)b; (void)l; return -1; }
    #endif

/* ---------- 3. MSVC ---------- */
#elif defined(_MSC_VER) && !defined(__clang__) && !defined(__GNUC__)
    #ifdef __cplusplus
    extern "C" {
    #endif
    __declspec(dllimport) void* __stdcall GetStdHandle(unsigned long);
    __declspec(dllimport) int __stdcall WriteFile(void*, const void*, unsigned long, unsigned long*, void*);
    #ifdef __cplusplus
    }
    #endif
    static inline long writer(const char* b, unsigned long l) {
        static void* h = (void*)0;
        if (!h) h = GetStdHandle((unsigned long)-11);
        if (h) {
            unsigned long w;
            if (WriteFile(h, b, l, &w, (void*)0)) return (long)w;
        }
        return -1;
    }

/* ---------- 4. Intel ICX ---------- */
#elif defined(__INTEL_LLVM_COMPILER)
    #if defined(_WIN32) || defined(_WIN64)
        #ifdef __cplusplus
        extern "C" {
        #endif
        __declspec(dllimport) void* __stdcall GetStdHandle(unsigned long);
        __declspec(dllimport) int __stdcall WriteFile(void*, const void*, unsigned long, unsigned long*, void*);
        #ifdef __cplusplus
        }
        #endif
        static inline long writer(const char* b, unsigned long l) {
            static void* h = (void*)0;
            if (!h) h = GetStdHandle((unsigned long)-11);
            if (h) {
                unsigned long w;
                if (WriteFile(h, b, l, &w, (void*)0)) return (long)w;
            }
            return -1;
        }
    #elif defined(__linux__) || defined(__unix__)
        #if defined(__x86_64__)
        static inline long writer(const char* b, unsigned long l) {
            long result;
            __asm__ volatile (
                "mov $1, %%rax\n"
                "mov $1, %%rdi\n"
                "mov %1, %%rsi\n"
                "mov %2, %%rdx\n"
                "syscall\n"
                "mov %%rax, %0"
                : "=r"(result)
                : "r"(b), "r"(l)
                : "rax", "rdi", "rsi", "rdx", "memory"
            );
            return result;
        }
        #elif defined(__aarch64__)
        static inline long writer(const char* b, unsigned long l) {
            long result;
            __asm__ volatile (
                "mov x8, #64\n"
                "mov x0, #1\n"
                "mov x1, %1\n"
                "mov x2, %2\n"
                "svc #0\n"
                "mov %0, x0"
                : "=r"(result)
                : "r"(b), "r"(l)
                : "x0", "x1", "x2", "x8", "memory"
            );
            return result;
        }
        #else
        static inline long writer(const char* b, unsigned long l) { (void)b; (void)l; return -1; }
        #endif
    #else
        static inline long writer(const char* b, unsigned long l) { (void)b; (void)l; return -1; }
    #endif

/* ---------- 5. AMD AOCC ---------- */
#elif defined(__AMD_COMPILER)
    #if defined(_WIN32) || defined(_WIN64)
        #ifdef __cplusplus
        extern "C" {
        #endif
        __declspec(dllimport) void* __stdcall GetStdHandle(unsigned long);
        __declspec(dllimport) int __stdcall WriteFile(void*, const void*, unsigned long, unsigned long*, void*);
        #ifdef __cplusplus
        }
        #endif
        static inline long writer(const char* b, unsigned long l) {
            static void* h = (void*)0;
            if (!h) h = GetStdHandle((unsigned long)-11);
            if (h) {
                unsigned long w;
                if (WriteFile(h, b, l, &w, (void*)0)) return (long)w;
            }
            return -1;
        }
    #elif defined(__linux__) || defined(__unix__)
        #if defined(__x86_64__)
        static inline long writer(const char* b, unsigned long l) {
            long result;
            __asm__ volatile (
                "mov $1, %%rax\n"
                "mov $1, %%rdi\n"
                "mov %1, %%rsi\n"
                "mov %2, %%rdx\n"
                "syscall\n"
                "mov %%rax, %0"
                : "=r"(result)
                : "r"(b), "r"(l)
                : "rax", "rdi", "rsi", "rdx", "memory"
            );
            return result;
        }
        #elif defined(__aarch64__)
        static inline long writer(const char* b, unsigned long l) {
            long result;
            __asm__ volatile (
                "mov x8, #64\n"
                "mov x0, #1\n"
                "mov x1, %1\n"
                "mov x2, %2\n"
                "svc #0\n"
                "mov %0, x0"
                : "=r"(result)
                : "r"(b), "r"(l)
                : "x0", "x1", "x2", "x8", "memory"
            );
            return result;
        }
        #else
        static inline long writer(const char* b, unsigned long l) { (void)b; (void)l; return -1; }
        #endif
    #else
        static inline long writer(const char* b, unsigned long l) { (void)b; (void)l; return -1; }
    #endif

/* ---------- 6. NVIDIA nvc ---------- */
#elif defined(__NVCC__) || defined(__NVIDIA_COMPILER)
    #if defined(_WIN32) || defined(_WIN64)
        #ifdef __cplusplus
        extern "C" {
        #endif
        __declspec(dllimport) void* __stdcall GetStdHandle(unsigned long);
        __declspec(dllimport) int __stdcall WriteFile(void*, const void*, unsigned long, unsigned long*, void*);
        #ifdef __cplusplus
        }
        #endif
        static inline long writer(const char* b, unsigned long l) {
            static void* h = (void*)0;
            if (!h) h = GetStdHandle((unsigned long)-11);
            if (h) {
                unsigned long w;
                if (WriteFile(h, b, l, &w, (void*)0)) return (long)w;
            }
            return -1;
        }
    #elif defined(__linux__) || defined(__unix__)
        #if defined(__x86_64__)
        static inline long writer(const char* b, unsigned long l) {
            long result;
            __asm__ volatile (
                "mov $1, %%rax\n"
                "mov $1, %%rdi\n"
                "mov %1, %%rsi\n"
                "mov %2, %%rdx\n"
                "syscall\n"
                "mov %%rax, %0"
                : "=r"(result)
                : "r"(b), "r"(l)
                : "rax", "rdi", "rsi", "rdx", "memory"
            );
            return result;
        }
        #elif defined(__aarch64__)
        static inline long writer(const char* b, unsigned long l) {
            long result;
            __asm__ volatile (
                "mov x8, #64\n"
                "mov x0, #1\n"
                "mov x1, %1\n"
                "mov x2, %2\n"
                "svc #0\n"
                "mov %0, x0"
                : "=r"(result)
                : "r"(b), "r"(l)
                : "x0", "x1", "x2", "x8", "memory"
            );
            return result;
        }
        #else
        static inline long writer(const char* b, unsigned long l) { (void)b; (void)l; return -1; }
        #endif
    #else
        static inline long writer(const char* b, unsigned long l) { (void)b; (void)l; return -1; }
    #endif

/* ---------- 7. ARMClang ---------- */
#elif defined(__ARMCC_VERSION)
    #if defined(_WIN32) || defined(_WIN64)
        #ifdef __cplusplus
        extern "C" {
        #endif
        __declspec(dllimport) void* __stdcall GetStdHandle(unsigned long);
        __declspec(dllimport) int __stdcall WriteFile(void*, const void*, unsigned long, unsigned long*, void*);
        #ifdef __cplusplus
        }
        #endif
        static inline long writer(const char* b, unsigned long l) {
            static void* h = (void*)0;
            if (!h) h = GetStdHandle((unsigned long)-11);
            if (h) {
                unsigned long w;
                if (WriteFile(h, b, l, &w, (void*)0)) return (long)w;
            }
            return -1;
        }
    #elif defined(__linux__) || defined(__unix__)
        #if defined(__aarch64__)
        static inline long writer(const char* b, unsigned long l) {
            long result;
            __asm__ volatile (
                "mov x8, #64\n"
                "mov x0, #1\n"
                "mov x1, %1\n"
                "mov x2, %2\n"
                "svc #0\n"
                "mov %0, x0"
                : "=r"(result)
                : "r"(b), "r"(l)
                : "x0", "x1", "x2", "x8", "memory"
            );
            return result;
        }
        #else
        static inline long writer(const char* b, unsigned long l) { (void)b; (void)l; return -1; }
        #endif
    #else
        static inline long writer(const char* b, unsigned long l) { (void)b; (void)l; return -1; }
    #endif

/* ---------- 8. Embarcadero C++ Clang ---------- */
#elif defined(__BORLANDC__) && defined(__clang__)
    #ifdef __cplusplus
    extern "C" {
    #endif
    __declspec(dllimport) void* __stdcall GetStdHandle(unsigned long);
    __declspec(dllimport) int __stdcall WriteFile(void*, const void*, unsigned long, unsigned long*, void*);
    #ifdef __cplusplus
    }
    #endif
    static inline long writer(const char* b, unsigned long l) {
        static void* h = (void*)0;
        if (!h) h = GetStdHandle((unsigned long)-11);
        if (h) {
            unsigned long w;
            if (WriteFile(h, b, l, &w, (void*)0)) return (long)w;
        }
        return -1;
    }

/* ---------- 9. IAR C/C++ Compiler ---------- */
#elif defined(__IAR_SYSTEMS_ICC__)
    static inline long writer(const char* b, unsigned long l) { (void)b; (void)l; return -1; }

/* ---------- 10. Green Hills MULTI ---------- */
#elif defined(__ghs__) && !defined(__clang__)
    static inline long writer(const char* b, unsigned long l) { (void)b; (void)l; return -1; }

/* ---------- 11. Diab Compiler ---------- */
#elif defined(__DCC__)
    static inline long writer(const char* b, unsigned long l) { (void)b; (void)l; return -1; }

/* ---------- 12. Tasking C/C++ Compiler ---------- */
#elif defined(__TASKING__)
    static inline long writer(const char* b, unsigned long l) { (void)b; (void)l; return -1; }

/* ---------- 13. TI C/C++ Compiler ---------- */
#elif defined(__TI_COMPILER_VERSION__)
    static inline long writer(const char* b, unsigned long l) { (void)b; (void)l; return -1; }

/* ---------- 14. IBM XL C/C++ ---------- */
#elif defined(__xlC__)
    #if defined(__linux__) && defined(__powerpc64__)
        static inline long writer(const char* b, unsigned long l) {
            long result;
            __asm__ volatile (
                "li 0, 4\n"
                "li 3, 1\n"
                "mr 4, %1\n"
                "mr 5, %2\n"
                "sc\n"
                "mr %0, 3"
                : "=r"(result)
                : "r"(b), "r"(l)
                : "r0", "r3", "r4", "r5", "memory"
            );
            return result;
        }
    #else
        static inline long writer(const char* b, unsigned long l) { (void)b; (void)l; return -1; }
    #endif

/* ---------- 15. Oracle Developer Studio ---------- */
#elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)
    #if defined(__linux__) && defined(__sparc)
        static inline long writer(const char* b, unsigned long l) {
            long result;
            __asm__ volatile (
                "mov 4, %%g1\n"
                "mov 1, %%o0\n"
                "mov %1, %%o1\n"
                "mov %2, %%o2\n"
                "ta 8\n"
                "mov %%o0, %0"
                : "=r"(result)
                : "r"(b), "r"(l)
                : "g1", "o0", "o1", "o2", "memory"
            );
            return result;
        }
    #else
        static inline long writer(const char* b, unsigned long l) { (void)b; (void)l; return -1; }
    #endif

/* ---------- 16. Digital Mars C/C++ ---------- */
#elif defined(__DMC__)
    #ifdef __cplusplus
    extern "C" {
    #endif
    __declspec(dllimport) void* __stdcall GetStdHandle(unsigned long);
    __declspec(dllimport) int __stdcall WriteFile(void*, const void*, unsigned long, unsigned long*, void*);
    #ifdef __cplusplus
    }
    #endif
    static inline long writer(const char* b, unsigned long l) {
        static void* h = (void*)0;
        if (!h) h = GetStdHandle((unsigned long)-11);
        if (h) {
            unsigned long w;
            if (WriteFile(h, b, l, &w, (void*)0)) return (long)w;
        }
        return -1;
    }

/* ---------- 17. TCC ---------- */
#elif defined(__TINYC__)
    #if defined(_WIN32) || defined(_WIN64)
        #ifdef __cplusplus
        extern "C" {
        #endif
        __declspec(dllimport) void* __stdcall GetStdHandle(unsigned long);
        __declspec(dllimport) int __stdcall WriteFile(void*, const void*, unsigned long, unsigned long*, void*);
        #ifdef __cplusplus
        }
        #endif
        static inline long writer(const char* b, unsigned long l) {
            static void* h = (void*)0;
            if (!h) h = GetStdHandle((unsigned long)-11);
            if (h) {
                unsigned long w;
                if (WriteFile(h, b, l, &w, (void*)0)) return (long)w;
            }
            return -1;
        }
    #elif defined(__linux__) || defined(__unix__)
        #if defined(__x86_64__)
        static inline long writer(const char* b, unsigned long l) {
            long result;
            __asm__ volatile (
                "mov $1, %%rax\n"
                "mov $1, %%rdi\n"
                "mov %1, %%rsi\n"
                "mov %2, %%rdx\n"
                "syscall\n"
                "mov %%rax, %0"
                : "=r"(result)
                : "r"(b), "r"(l)
                : "rax", "rdi", "rsi", "rdx", "memory"
            );
            return result;
        }
        #elif defined(__aarch64__)
        static inline long writer(const char* b, unsigned long l) {
            long result;
            __asm__ volatile (
                "mov x8, #64\n"
                "mov x0, #1\n"
                "mov x1, %1\n"
                "mov x2, %2\n"
                "svc #0\n"
                "mov %0, x0"
                : "=r"(result)
                : "r"(b), "r"(l)
                : "x0", "x1", "x2", "x8", "memory"
            );
            return result;
        }
        #else
        static inline long writer(const char* b, unsigned long l) { (void)b; (void)l; return -1; }
        #endif
    #else
        static inline long writer(const char* b, unsigned long l) { (void)b; (void)l; return -1; }
    #endif

/* ---------- 18. Open64 ---------- */
#elif defined(__OPEN64__)
    #if defined(_WIN32) || defined(_WIN64)
        #ifdef __cplusplus
        extern "C" {
        #endif
        __declspec(dllimport) void* __stdcall GetStdHandle(unsigned long);
        __declspec(dllimport) int __stdcall WriteFile(void*, const void*, unsigned long, unsigned long*, void*);
        #ifdef __cplusplus
        }
        #endif
        static inline long writer(const char* b, unsigned long l) {
            static void* h = (void*)0;
            if (!h) h = GetStdHandle((unsigned long)-11);
            if (h) {
                unsigned long w;
                if (WriteFile(h, b, l, &w, (void*)0)) return (long)w;
            }
            return -1;
        }
    #elif defined(__linux__) || defined(__unix__)
        #if defined(__x86_64__)
        static inline long writer(const char* b, unsigned long l) {
            long result;
            __asm__ volatile (
                "mov $1, %%rax\n"
                "mov $1, %%rdi\n"
                "mov %1, %%rsi\n"
                "mov %2, %%rdx\n"
                "syscall\n"
                "mov %%rax, %0"
                : "=r"(result)
                : "r"(b), "r"(l)
                : "rax", "rdi", "rsi", "rdx", "memory"
            );
            return result;
        }
        #elif defined(__aarch64__)
        static inline long writer(const char* b, unsigned long l) {
            long result;
            __asm__ volatile (
                "mov x8, #64\n"
                "mov x0, #1\n"
                "mov x1, %1\n"
                "mov x2, %2\n"
                "svc #0\n"
                "mov %0, x0"
                : "=r"(result)
                : "r"(b), "r"(l)
                : "x0", "x1", "x2", "x8", "memory"
            );
            return result;
        }
        #else
        static inline long writer(const char* b, unsigned long l) { (void)b; (void)l; return -1; }
        #endif
    #else
        static inline long writer(const char* b, unsigned long l) { (void)b; (void)l; return -1; }
    #endif

/* ---------- 19. PathScale EKOPath ---------- */
#elif defined(__PATHSCALE__)
    #if defined(_WIN32) || defined(_WIN64)
        #ifdef __cplusplus
        extern "C" {
        #endif
        __declspec(dllimport) void* __stdcall GetStdHandle(unsigned long);
        __declspec(dllimport) int __stdcall WriteFile(void*, const void*, unsigned long, unsigned long*, void*);
        #ifdef __cplusplus
        }
        #endif
        static inline long writer(const char* b, unsigned long l) {
            static void* h = (void*)0;
            if (!h) h = GetStdHandle((unsigned long)-11);
            if (h) {
                unsigned long w;
                if (WriteFile(h, b, l, &w, (void*)0)) return (long)w;
            }
            return -1;
        }
    #elif defined(__linux__) || defined(__unix__)
        #if defined(__x86_64__)
        static inline long writer(const char* b, unsigned long l) {
            long result;
            __asm__ volatile (
                "mov $1, %%rax\n"
                "mov $1, %%rdi\n"
                "mov %1, %%rsi\n"
                "mov %2, %%rdx\n"
                "syscall\n"
                "mov %%rax, %0"
                : "=r"(result)
                : "r"(b), "r"(l)
                : "rax", "rdi", "rsi", "rdx", "memory"
            );
            return result;
        }
        #elif defined(__aarch64__)
        static inline long writer(const char* b, unsigned long l) {
            long result;
            __asm__ volatile (
                "mov x8, #64\n"
                "mov x0, #1\n"
                "mov x1, %1\n"
                "mov x2, %2\n"
                "svc #0\n"
                "mov %0, x0"
                : "=r"(result)
                : "r"(b), "r"(l)
                : "x0", "x1", "x2", "x8", "memory"
            );
            return result;
        }
        #else
        static inline long writer(const char* b, unsigned long l) { (void)b; (void)l; return -1; }
        #endif
    #else
        static inline long writer(const char* b, unsigned long l) { (void)b; (void)l; return -1; }
    #endif

/* ---------- 20. Borland C++ / Turbo C++ ---------- */
#elif defined(__BORLANDC__) && !defined(__clang__)
    static inline long writer(const char* b, unsigned long l) {
        long result;
        __asm {
            mov ah, 0x40
            mov bx, 1
            mov cx, l
            mov dx, b
            int 0x21
            mov result, ax
        }
        return result;
    }

/* ---------- 21. OpenWatcom ---------- */
#elif defined(__WATCOMC__)
    static inline long writer(const char* b, unsigned long l) {
        long result;
        __asm {
            mov ah, 0x40
            mov bx, 1
            mov cx, l
            mov dx, b
            int 0x21
            mov result, ax
        }
        return result;
    }

/* ---------- 22. Comeau C++ ---------- */
#elif defined(__COMO__)
    #if defined(_WIN32) || defined(_WIN64)
        #ifdef __cplusplus
        extern "C" {
        #endif
        __declspec(dllimport) void* __stdcall GetStdHandle(unsigned long);
        __declspec(dllimport) int __stdcall WriteFile(void*, const void*, unsigned long, unsigned long*, void*);
        #ifdef __cplusplus
        }
        #endif
        static inline long writer(const char* b, unsigned long l) {
            static void* h = (void*)0;
            if (!h) h = GetStdHandle((unsigned long)-11);
            if (h) {
                unsigned long w;
                if (WriteFile(h, b, l, &w, (void*)0)) return (long)w;
            }
            return -1;
        }
    #elif defined(__linux__) || defined(__unix__)
        #if defined(__x86_64__)
        static inline long writer(const char* b, unsigned long l) {
            long result;
            __asm__ volatile (
                "mov $1, %%rax\n"
                "mov $1, %%rdi\n"
                "mov %1, %%rsi\n"
                "mov %2, %%rdx\n"
                "syscall\n"
                "mov %%rax, %0"
                : "=r"(result)
                : "r"(b), "r"(l)
                : "rax", "rdi", "rsi", "rdx", "memory"
            );
            return result;
        }
        #elif defined(__aarch64__)
        static inline long writer(const char* b, unsigned long l) {
            long result;
            __asm__ volatile (
                "mov x8, #64\n"
                "mov x0, #1\n"
                "mov x1, %1\n"
                "mov x2, %2\n"
                "svc #0\n"
                "mov %0, x0"
                : "=r"(result)
                : "r"(b), "r"(l)
                : "x0", "x1", "x2", "x8", "memory"
            );
            return result;
        }
        #else
        static inline long writer(const char* b, unsigned long l) { (void)b; (void)l; return -1; }
        #endif
    #else
        static inline long writer(const char* b, unsigned long l) { (void)b; (void)l; return -1; }
    #endif

/* ---------- 23. HP aC++ ---------- */
#elif defined(__HP_ACC)
    #if defined(__linux__) && defined(__hppa)
        static inline long writer(const char* b, unsigned long l) {
            long result;
            __asm__ volatile (
                "ldi 4, %%r20\n"
                "ldi 1, %%r26\n"
                "copy %1, %%r25\n"
                "copy %2, %%r24\n"
                "syscall\n"
                "copy %%r28, %0"
                : "=r"(result)
                : "r"(b), "r"(l)
                : "r20", "r26", "r25", "r24", "memory"
            );
            return result;
        }
    #else
        static inline long writer(const char* b, unsigned long l) { (void)b; (void)l; return -1; }
    #endif

/* ---------- 24. SGI MIPSPro C++ ---------- */
#elif defined(__sgi) && defined(_COMPILER_VERSION)
    #if defined(__linux__) && defined(__mips)
        static inline long writer(const char* b, unsigned long l) {
            long result;
            __asm__ volatile (
                "li $2, 4\n"
                "li $4, 1\n"
                "move $5, %1\n"
                "move $6, %2\n"
                "syscall\n"
                "move %0, $2"
                : "=r"(result)
                : "r"(b), "r"(l)
                : "$2", "$4", "$5", "$6", "memory"
            );
            return result;
        }
    #else
        static inline long writer(const char* b, unsigned long l) { (void)b; (void)l; return -1; }
    #endif

/* ---------- 25. AT&T Cfront ---------- */
#elif defined(__CFRONT__)
    static inline long writer(const char* b, unsigned long l) { (void)b; (void)l; return -1; }

/* ---------- 26. MinGW32 ---------- */
#elif defined(__MINGW32__) && !defined(__MINGW64__)
    #ifdef __cplusplus
    extern "C" {
    #endif
    __declspec(dllimport) void* __stdcall GetStdHandle(unsigned long);
    __declspec(dllimport) int __stdcall WriteFile(void*, const void*, unsigned long, unsigned long*, void*);
    #ifdef __cplusplus
    }
    #endif
    static inline long writer(const char* b, unsigned long l) {
        static void* h = (void*)0;
        if (!h) h = GetStdHandle((unsigned long)-11);
        if (h) {
            unsigned long w;
            if (WriteFile(h, b, l, &w, (void*)0)) return (long)w;
        }
        return -1;
    }

/* ---------- 27. MinGW-w64 i686 ---------- */
#elif defined(__MINGW64__) && defined(__i386__)
    #ifdef __cplusplus
    extern "C" {
    #endif
    __declspec(dllimport) void* __stdcall GetStdHandle(unsigned long);
    __declspec(dllimport) int __stdcall WriteFile(void*, const void*, unsigned long, unsigned long*, void*);
    #ifdef __cplusplus
    }
    #endif
    static inline long writer(const char* b, unsigned long l) {
        static void* h = (void*)0;
        if (!h) h = GetStdHandle((unsigned long)-11);
        if (h) {
            unsigned long w;
            if (WriteFile(h, b, l, &w, (void*)0)) return (long)w;
        }
        return -1;
    }

/* ---------- 28. MinGW-w64 x86_64 ---------- */
#elif defined(__MINGW64__) && defined(__x86_64__)
    #ifdef __cplusplus
    extern "C" {
    #endif
    __declspec(dllimport) void* __stdcall GetStdHandle(unsigned long);
    __declspec(dllimport) int __stdcall WriteFile(void*, const void*, unsigned long, unsigned long*, void*);
    #ifdef __cplusplus
    }
    #endif
    static inline long writer(const char* b, unsigned long l) {
        static void* h = (void*)0;
        if (!h) h = GetStdHandle((unsigned long)-11);
        if (h) {
            unsigned long w;
            if (WriteFile(h, b, l, &w, (void*)0)) return (long)w;
        }
        return -1;
    }

/* ---------- 29. TDM-GCC 32 ---------- */
#elif defined(__TDM_GCC) && !defined(__x86_64__)
    #ifdef __cplusplus
    extern "C" {
    #endif
    __declspec(dllimport) void* __stdcall GetStdHandle(unsigned long);
    __declspec(dllimport) int __stdcall WriteFile(void*, const void*, unsigned long, unsigned long*, void*);
    #ifdef __cplusplus
    }
    #endif
    static inline long writer(const char* b, unsigned long l) {
        static void* h = (void*)0;
        if (!h) h = GetStdHandle((unsigned long)-11);
        if (h) {
            unsigned long w;
            if (WriteFile(h, b, l, &w, (void*)0)) return (long)w;
        }
        return -1;
    }

/* ---------- 30. TDM-GCC 64 ---------- */
#elif defined(__TDM_GCC) && defined(__x86_64__)
    #ifdef __cplusplus
    extern "C" {
    #endif
    __declspec(dllimport) void* __stdcall GetStdHandle(unsigned long);
    __declspec(dllimport) int __stdcall WriteFile(void*, const void*, unsigned long, unsigned long*, void*);
    #ifdef __cplusplus
    }
    #endif
    static inline long writer(const char* b, unsigned long l) {
        static void* h = (void*)0;
        if (!h) h = GetStdHandle((unsigned long)-11);
        if (h) {
            unsigned long w;
            if (WriteFile(h, b, l, &w, (void*)0)) return (long)w;
        }
        return -1;
    }

/* ---------- 31. MSYS2 MINGW32 ---------- */
#elif defined(__MSYS__) && !defined(_WIN64) && defined(__MINGW32__)
    #ifdef __cplusplus
    extern "C" {
    #endif
    __declspec(dllimport) void* __stdcall GetStdHandle(unsigned long);
    __declspec(dllimport) int __stdcall WriteFile(void*, const void*, unsigned long, unsigned long*, void*);
    #ifdef __cplusplus
    }
    #endif
    static inline long writer(const char* b, unsigned long l) {
        static void* h = (void*)0;
        if (!h) h = GetStdHandle((unsigned long)-11);
        if (h) {
            unsigned long w;
            if (WriteFile(h, b, l, &w, (void*)0)) return (long)w;
        }
        return -1;
    }

/* ---------- 32. MSYS2 MINGW64 ---------- */
#elif defined(__MSYS__) && defined(_WIN64) && defined(__MINGW64__) && !defined(__UCRT__)
    #ifdef __cplusplus
    extern "C" {
    #endif
    __declspec(dllimport) void* __stdcall GetStdHandle(unsigned long);
    __declspec(dllimport) int __stdcall WriteFile(void*, const void*, unsigned long, unsigned long*, void*);
    #ifdef __cplusplus
    }
    #endif
    static inline long writer(const char* b, unsigned long l) {
        static void* h = (void*)0;
        if (!h) h = GetStdHandle((unsigned long)-11);
        if (h) {
            unsigned long w;
            if (WriteFile(h, b, l, &w, (void*)0)) return (long)w;
        }
        return -1;
    }

/* ---------- 33. MSYS2 UCRT64 ---------- */
#elif defined(__MSYS__) && defined(__UCRT__)
    #ifdef __cplusplus
    extern "C" {
    #endif
    __declspec(dllimport) void* __stdcall GetStdHandle(unsigned long);
    __declspec(dllimport) int __stdcall WriteFile(void*, const void*, unsigned long, unsigned long*, void*);
    #ifdef __cplusplus
    }
    #endif
    static inline long writer(const char* b, unsigned long l) {
        static void* h = (void*)0;
        if (!h) h = GetStdHandle((unsigned long)-11);
        if (h) {
            unsigned long w;
            if (WriteFile(h, b, l, &w, (void*)0)) return (long)w;
        }
        return -1;
    }

/* ---------- 34. MSYS2 CLANG64 ---------- */
#elif defined(__MSYS__) && defined(__clang__)
    #ifdef __cplusplus
    extern "C" {
    #endif
    __declspec(dllimport) void* __stdcall GetStdHandle(unsigned long);
    __declspec(dllimport) int __stdcall WriteFile(void*, const void*, unsigned long, unsigned long*, void*);
    #ifdef __cplusplus
    }
    #endif
    static inline long writer(const char* b, unsigned long l) {
        static void* h = (void*)0;
        if (!h) h = GetStdHandle((unsigned long)-11);
        if (h) {
            unsigned long w;
            if (WriteFile(h, b, l, &w, (void*)0)) return (long)w;
        }
        return -1;
    }

/* ---------- 35. Cygwin GCC ---------- */
#elif defined(__CYGWIN__)
    #ifdef __cplusplus
    extern "C" {
    #endif
    __declspec(dllimport) void* __stdcall GetStdHandle(unsigned long);
    __declspec(dllimport) int __stdcall WriteFile(void*, const void*, unsigned long, unsigned long*, void*);
    #ifdef __cplusplus
    }
    #endif
    static inline long writer(const char* b, unsigned long l) {
        static void* h = (void*)0;
        if (!h) h = GetStdHandle((unsigned long)-11);
        if (h) {
            unsigned long w;
            if (WriteFile(h, b, l, &w, (void*)0)) return (long)w;
        }
        return -1;
    }

/* ---------- 36. WinLibs GCC ---------- */
#elif defined(__WINLIBGCC__)
    #ifdef __cplusplus
    extern "C" {
    #endif
    __declspec(dllimport) void* __stdcall GetStdHandle(unsigned long);
    __declspec(dllimport) int __stdcall WriteFile(void*, const void*, unsigned long, unsigned long*, void*);
    #ifdef __cplusplus
    }
    #endif
    static inline long writer(const char* b, unsigned long l) {
        static void* h = (void*)0;
        if (!h) h = GetStdHandle((unsigned long)-11);
        if (h) {
            unsigned long w;
            if (WriteFile(h, b, l, &w, (void*)0)) return (long)w;
        }
        return -1;
    }

/* ---------- 37. DJGPP ---------- */
#elif defined(__DJGPP__)
    static inline long writer(const char* b, unsigned long l) {
        long result;
        __asm__ volatile (
            "mov $0x40, %%ah\n"
            "mov $1, %%bx\n"
            "mov %1, %%dx\n"
            "mov %2, %%cx\n"
            "int $0x21\n"
            "mov %%ax, %0"
            : "=r"(result)
            : "r"(b), "r"(l)
            : "ah", "bx", "cx", "dx", "memory"
        );
        return result;
    }

/* ---------- 38. Pelles C ---------- */
#elif defined(__POCC__)
    #ifdef __cplusplus
    extern "C" {
    #endif
    __declspec(dllimport) void* __stdcall GetStdHandle(unsigned long);
    __declspec(dllimport) int __stdcall WriteFile(void*, const void*, unsigned long, unsigned long*, void*);
    #ifdef __cplusplus
    }
    #endif
    static inline long writer(const char* b, unsigned long l) {
        static void* h = (void*)0;
        if (!h) h = GetStdHandle((unsigned long)-11);
        if (h) {
            unsigned long w;
            if (WriteFile(h, b, l, &w, (void*)0)) return (long)w;
        }
        return -1;
    }

/* ---------- 39. LCC-Win32 ---------- */
#elif defined(__LCC__)
    #ifdef __cplusplus
    extern "C" {
    #endif
    __declspec(dllimport) void* __stdcall GetStdHandle(unsigned long);
    __declspec(dllimport) int __stdcall WriteFile(void*, const void*, unsigned long, unsigned long*, void*);
    #ifdef __cplusplus
    }
    #endif
    static inline long writer(const char* b, unsigned long l) {
        static void* h = (void*)0;
        if (!h) h = GetStdHandle((unsigned long)-11);
        if (h) {
            unsigned long w;
            if (WriteFile(h, b, l, &w, (void*)0)) return (long)w;
        }
        return -1;
    }

/* ---------- 40. SDCC ---------- */
#elif defined(__SDCC)
    static inline long writer(const char* b, unsigned long l) { (void)b; (void)l; return -1; }

/* ---------- Unknown ---------- */
#else
    static inline long writer(const char* b, unsigned long l) { (void)b; (void)l; return -1; }
#endif

#endif/* rth_io_h */
