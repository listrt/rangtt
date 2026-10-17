/* rth_out_pro_3 */
#ifndef rth_out_pro_3
#define rth_out_pro_3

//writer functions
#include<rth_out>

//inline_rt
#if defined(_MSC_VER)
    #define inline_rt __forceinline
#elif defined(__GNUC__) || defined(__clang__)
    #define inline_rt __attribute__((always_inline)) inline
#elif defined(__INTEL_COMPILER) || defined(__ICC)
    #define inline_rt __forceinline
#elif defined(__DMC__) || defined(_DIGITAL_MARS)
    #define inline_rt __inline
#else
    #define inline_rt inline
#endif
namespace{
    namespace rangtt{
        namespace out{
            using len_t =unsigned;
            enum class outway:unsigned char{
                unknown=0,
                stdout=1,
                stderr=2,
                file=3,
                log=4,
                net=5
            };
            
            
            namespace fmt_tools{
                //help_tools
                inline_rt len_t str_len(const char* s){
                    const char* p=s;
                    while(*p)
                        p++;
                    return (unsigned)(p-s);
                }
                inline_rt char* lltoa(long long a, char* b){
                    char* p=b;
                    unsigned long long ua;
                    if(a<0){
                        *p++='-';
                        ua=(unsigned long long)(-(a + 1)) + 1;
                    }else{
                        ua = (unsigned long long)a;
                    }

                    char tmp[24];
                    int i=0;
                    do{
                        tmp[i++] = (char)('0' + ua % 10);
                        ua /= 10;
                    }while (ua);

                    while (i)
                        *p++ = tmp[--i];

                    *p = '\0';
                    return b;
                }
                inline_rt char* ulltoa(unsigned long long a, char* b){
                    char* p=b;
                    char tmp[24];
                    int i=0;
                    do{
                        tmp[i++]=(char)('0' + a % 10);
                        a /= 10;
                    }while (a);
                    while (i)
                        *p++ = tmp[--i];
                    *p='\0';
                    return b;
                }
                inline_rt char* ptrtoa(void* ptr, char* buf){
                    #if defined(_WIN64) || defined(__LP64__) || defined(__x86_64__) \
                        || defined(__aarch64__) || defined(__riscv64)
                        typedef unsigned long long uintptr_t;   // 64
                    #else
                        typedef unsigned int uintptr_t;         // 32
                    #endif
                    static const char hex_digits[] = "0123456789abcdef";
                    uintptr_t value = (uintptr_t)ptr;
                    char* p=buf;int shift;
                    *p++='0';
                    *p++='x';
                    shift = (sizeof(uintptr_t) * 8)-4;
                    while (shift>0 && ((value >> shift) & 0xF)==0) {
                        shift-=4;
                    }
                    for (;shift>=0; shift-=4) {
                        *p++ = hex_digits[(value >> shift) & 0xF];
                    }
                    *p = '\0';
                    return buf;
                }
                inline_rt char* ftoa(long double v, char* s){
                    char tmp[64];
                    char* p=tmp;
                    int neg;
                    int is_nan;
                    int is_inf;
                    unsigned long i;

                    if (!s) return s;

                    is_nan = 0;
                    is_inf = 0;

                    if (sizeof(double) * 8 == 64) {
                        double d = (double)v;
                        unsigned char b[sizeof(double)];
                        unsigned long long u = 0;
                        unsigned int exp_all1;
                        unsigned int one = 1;
                        int little;
                        unsigned char* src;

                        src = (unsigned char*)&d;
                        for (i = 0; i < sizeof(double); i++) b[i] = src[i];

                        little = (*(unsigned char*)&one == 1);
                        if (little) {
                            for (i = 0; i < 8; i++) u |= ((unsigned long long)b[i]) << (8 * i);
                        } else {
                            for (i = 0; i < 8; i++) u = (u << 8) | b[i];
                        }

                        exp_all1 = ((u >> 52) & 0x7FFULL) == 0x7FFULL;
                        if (exp_all1) {
                            if ((u & 0xFFFFFFFFFFFFFULL) != 0) is_nan = 1;
                            else is_inf = 1;
                            if (is_inf && (v - v) == 0.0L) is_inf = 0;
                        }
                    } else {
                        if (v != v) is_nan = 1;
                        else if ((v - v) != 0.0L) is_inf = 1;
                    }

                    if (is_nan) {
                        *p++ = 'n'; *p++ = 'a'; *p++ = 'n'; *p = 0;
                        goto done;
                    }
                    if (is_inf) {
                        if (v < 0) *p++ = '-';
                        *p++ = 'i'; *p++ = 'n'; *p++ = 'f'; *p = 0;
                        goto done;
                    }

                    if (v == 0.0L) {
                        *p++ = '0'; *p = 0;
                        goto done;
                    }

                    neg = 0;
                    if (v < 0) { neg = 1; v = -v; }

                    if (v >= 1e18L) {
                        long double m = v;
                        int e = 0;
                        unsigned long long sig, fo, f;
                        int nd;
                        char t[6];
                        char te[8];
                        int ne;
                        int j;

                        while (m >= 1e10L) { m /= 1e10L; e += 10; }
                        while (m >= 10.0L) { m /= 10.0L; e++; }
                        while (m < 1.0L)   { m *= 10.0L; e--; }

                        sig = (unsigned long long)(m * 1000000.0L + 0.5L);
                        if (sig >= 10000000ULL) { sig = 1000000ULL; e++; }

                        if (neg) *p++ = '-';
                        *p++ = (char)('0' + (int)(sig / 1000000ULL));

                        fo = sig % 1000000ULL;
                        f = fo;
                        nd = 6;
                        while (nd && f % 10 == 0) { f /= 10; nd--; }
                        if (nd) {
                            *p++ = '.';
                            for (j = 5; j >= 0; j--) { t[j] = (char)('0' + fo % 10); fo /= 10; }
                            for (j = 0; j < nd; j++) *p++ = t[j];
                        }

                        *p++ = 'e';
                        if (e < 0) { *p++ = '-'; e = -e; } else *p++ = '+';

                        ne = 0;
                        if (!e) te[ne++] = '0';
                        while (e) { te[ne++] = (char)('0' + e % 10); e /= 10; }
                        while (ne) *p++ = te[--ne];
                        *p = 0;
                        goto done;
                    }

                    {
                        unsigned long long ip = (unsigned long long)v;
                        long double frac = v - (long double)ip;
                        unsigned long long frac6 = (unsigned long long)(frac * 1000000.0L + 0.5L);

                        if (frac6 >= 1000000ULL) {
                            frac6 = 0;
                            ip++;
                        }

                        if (neg) *p++ = '-';

                        {
                            char t[24]; int n2 = 0;
                            if (!ip) t[n2++] = '0';
                            while (ip) { t[n2++] = (char)('0' + (int)(ip % 10)); ip /= 10; }
                            while (n2) *p++ = t[--n2];
                        }

                        if (frac6) {
                            char fbuf[6];
                            unsigned nf = 6;
                            for (i = 5; i >= 0; i--) { fbuf[i] = (char)('0' + (int)(frac6 % 10)); frac6 /= 10; }
                            while (nf && fbuf[nf-1] == '0') nf--;
                            *p++ = '.';
                            for (i = 0; i < nf; i++) *p++ = fbuf[i];
                        }

                        if (neg && p == tmp + 2 && tmp[0] == '-' && tmp[1] == '0') {
                            tmp[0] = '0'; p = tmp + 1;
                        }
                        *p = 0;
                    }

                done:
                    {
                        unsigned long len = (unsigned long)(p - tmp);
                        for (i = 0; i < len; i++) s[i] = tmp[i];
                        s[len] = 0;
                    }
                    return s;
                }
            }
            namespace stdout_ns{
                inline_rt len_t outf(long long a){
                    char b[30]={};
                    fmt_tools::lltoa(a,b);
                    len_t l=fmt_tools::str_len(b);
                    stdoutwriter(b,l);
                    return l;
                }
                inline_rt len_t outf(long  a){return outf((long long)(a));}
                inline_rt len_t outf(int   a){return outf((long long)(a));}
                inline_rt len_t outf(short a){return outf((long long)(a));}
                inline_rt len_t outf(unsigned long long a){
                    char b[30]={};
                    fmt_tools::ulltoa(a,b);
                    len_t l=fmt_tools::str_len(b);
                    stdoutwriter(b,l);
                    return l;
                }
                inline_rt len_t outf(unsigned long  a){return outf((unsigned long long)(a));}
                inline_rt len_t outf(unsigned       a){return outf((unsigned long long)(a));}
                inline_rt len_t outf(unsigned short a){return outf((unsigned long long)(a));}
                inline_rt len_t outf(char          a){
                    return stdoutwriter(&a,1);
                }
                inline_rt len_t outf(signed   char a){return outf((char)(a));}
                inline_rt len_t outf(unsigned char a){return outf((char)(a));}
                inline_rt len_t outf(long double a){
                    char b[42]={};
                    fmt_tools::ftoa(a,b);
                    len_t l=fmt_tools::str_len(b);
                    stdoutwriter(b,l);
                    return l;
                }
                inline_rt len_t outf(double      a){return outf((long double)(a));}
                inline_rt len_t outf(float       a){return outf((long double)(a));}
                inline_rt len_t outf(void* a){
                    char b[42]={};
                    fmt_tools::ptrtoa(a,b);
                    len_t l=fmt_tools::str_len(b);
                    stdoutwriter(b,l);
                    return l;
                }
                template<typename T>
                inline_rt len_t outf(T* a){return outf((void*)(a));}
                inline_rt len_t outf(char* a){
                    len_t l=fmt_tools::str_len(a);
                    stdoutwriter(a,l);
                    return l;
                }
                inline_rt len_t outf(const char* a){
                    len_t l=fmt_tools::str_len(a);
                    stdoutwriter(a,l);
                    return l;
                }
            }
            namespace stderr_ns{
                inline_rt len_t outf(long long a){
                    char b[30]={};
                    fmt_tools::lltoa(a,b);
                    len_t l=fmt_tools::str_len(b);
                    stderrwriter(b,l);
                    return l;
                }
                inline_rt len_t outf(long  a){return outf((long long)(a));}
                inline_rt len_t outf(int   a){return outf((long long)(a));}
                inline_rt len_t outf(short a){return outf((long long)(a));}
                inline_rt len_t outf(unsigned long long a){
                    char b[30]={};
                    fmt_tools::ulltoa(a,b);
                    len_t l=fmt_tools::str_len(b);
                    stderrwriter(b,l);
                    return l;
                }
                inline_rt len_t outf(unsigned long  a){return outf((unsigned long long)(a));}
                inline_rt len_t outf(unsigned       a){return outf((unsigned long long)(a));}
                inline_rt len_t outf(unsigned short a){return outf((unsigned long long)(a));}
                inline_rt len_t outf(char          a){
                    return stderrwriter(&a,1);
                }
                inline_rt len_t outf(signed   char a){return outf((char)(a));}
                inline_rt len_t outf(unsigned char a){return outf((char)(a));}
                inline_rt len_t outf(long double a){
                    char b[42]={};
                    fmt_tools::ftoa(a,b);
                    len_t l=fmt_tools::str_len(b);
                    stderrwriter(b,l);
                    return l;
                }
                inline_rt len_t outf(double      a){return outf((long double)(a));}
                inline_rt len_t outf(float       a){return outf((long double)(a));}
                inline_rt len_t outf(void* a){
                    char b[42]={};
                    fmt_tools::ptrtoa(a,b);
                    len_t l=fmt_tools::str_len(b);
                    stderrwriter(b,l);
                    return l;
                }
                template<typename T>
                inline_rt len_t outf(T* a){return outf((void*)(a));}
                inline_rt len_t outf(char* a){
                    len_t l=fmt_tools::str_len(a);
                    stderrwriter(a,l);
                    return l;
                }
                inline_rt len_t outf(const char* a){
                    len_t l=fmt_tools::str_len(a);
                    stderrwriter(a,l);
                    return l;
                }
            }
            namespace fileout_ns{
                inline_rt len_t outf(long long a, const char* path){
                    char b[30]={};
                    fmt_tools::lltoa(a,b);
                    len_t l=fmt_tools::str_len(b);
                    filewriter(path,b,l);
                    return l;
                }
                inline_rt len_t outf(long  a, const char* path){return outf((long long)(a),path);}
                inline_rt len_t outf(int   a, const char* path){return outf((long long)(a),path);}
                inline_rt len_t outf(short a, const char* path){return outf((long long)(a),path);}
                inline_rt len_t outf(unsigned long long a, const char* path){
                    char b[30]={};
                    fmt_tools::ulltoa(a,b);
                    len_t l=fmt_tools::str_len(b);
                    filewriter(path,b,l);
                    return l;
                }
                inline_rt len_t outf(unsigned long  a, const char* path){return outf((unsigned long long)(a),path);}
                inline_rt len_t outf(unsigned       a, const char* path){return outf((unsigned long long)(a),path);}
                inline_rt len_t outf(unsigned short a, const char* path){return outf((unsigned long long)(a),path);}
                inline_rt len_t outf(char          a, const char* path){
                    return filewriter(path,&a,1);
                }
                inline_rt len_t outf(signed   char a, const char* path){return outf((char)(a),path);}
                inline_rt len_t outf(unsigned char a, const char* path){return outf((char)(a),path);}
                inline_rt len_t outf(long double a, const char* path){
                    char b[42]={};
                    fmt_tools::ftoa(a,b);
                    len_t l=fmt_tools::str_len(b);
                    filewriter(path,b,l);
                    return l;
                }
                inline_rt len_t outf(double      a, const char* path){return outf((long double)(a),path);}
                inline_rt len_t outf(float       a, const char* path){return outf((long double)(a),path);}
                inline_rt len_t outf(void* a, const char* path){
                    char b[42]={};
                    fmt_tools::ptrtoa(a,b);
                    len_t l=fmt_tools::str_len(b);
                    filewriter(path,b,l);
                    return l;
                }
                template<typename T>
                inline_rt len_t outf(T* a, const char* path){return outf((void*)(a),path);}
                inline_rt len_t outf(char* a, const char* path){
                    len_t l=fmt_tools::str_len(a);
                    filewriter(path,a,l);
                    return l;
                }
                inline_rt len_t outf(const char* a, const char* path){
                    len_t l=fmt_tools::str_len(a);
                    filewriter(path,a,l);
                    return l;
                }
            }
            namespace logout_ns{
                inline_rt len_t outf(long long a, const char* path){
                    char b[30]={};
                    fmt_tools::lltoa(a,b);
                    len_t l=fmt_tools::str_len(b);
                    logwriter(path,b,l);
                    return l;
                }
                inline_rt len_t outf(long  a, const char* path){return outf((long long)(a),path);}
                inline_rt len_t outf(int   a, const char* path){return outf((long long)(a),path);}
                inline_rt len_t outf(short a, const char* path){return outf((long long)(a),path);}
                inline_rt len_t outf(unsigned long long a, const char* path){
                    char b[30]={};
                    fmt_tools::ulltoa(a,b);
                    len_t l=fmt_tools::str_len(b);
                    logwriter(path,b,l);
                    return l;
                }
                inline_rt len_t outf(unsigned long  a, const char* path){return outf((unsigned long long)(a),path);}
                inline_rt len_t outf(unsigned       a, const char* path){return outf((unsigned long long)(a),path);}
                inline_rt len_t outf(unsigned short a, const char* path){return outf((unsigned long long)(a),path);}
                inline_rt len_t outf(char          a, const char* path){
                    return logwriter(path,&a,1);
                }
                inline_rt len_t outf(signed   char a, const char* path){return outf((char)(a),path);}
                inline_rt len_t outf(unsigned char a, const char* path){return outf((char)(a),path);}
                inline_rt len_t outf(long double a, const char* path){
                    char b[42]={};
                    fmt_tools::ftoa(a,b);
                    len_t l=fmt_tools::str_len(b);
                    logwriter(path,b,l);
                    return l;
                }
                inline_rt len_t outf(double      a, const char* path){return outf((long double)(a),path);}
                inline_rt len_t outf(float       a, const char* path){return outf((long double)(a),path);}
                inline_rt len_t outf(void* a, const char* path){
                    char b[42]={};
                    fmt_tools::ptrtoa(a,b);
                    len_t l=fmt_tools::str_len(b);
                    logwriter(path,b,l);
                    return l;
                }
                template<typename T>
                inline_rt len_t outf(T* a, const char* path){return outf((void*)(a),path);}
                inline_rt len_t outf(char* a, const char* path){
                    len_t l=fmt_tools::str_len(a);
                    logwriter(path,a,l);
                    return l;
                }
                inline_rt len_t outf(const char* a, const char* path){
                    len_t l=fmt_tools::str_len(a);
                    logwriter(path,a,l);
                    return l;
                }
            }
            struct out_t{
                outway w=outway::unknown;
                const char* path=0;
                inline_rt out_t& operator<<(outway b)noexcept{
                    w=b;
                    return *this;
                }
                inline_rt out_t& operator=(const out_t&) =default;
                inline_rt out_t& setpath(const char* p) noexcept{
                    path=p;
                    return *this;
                }
                #define RTH_DISP(expr)                                            \
                    do{                                                          \
                        switch (w) {                                              \
                        case outway::stdout: stdout_ns::outf(expr);          break; \
                        case outway::stderr: stderr_ns::outf(expr);          break; \
                        case outway::file:   fileout_ns::outf(expr, path);   break; \
                        case outway::log:    logout_ns::outf(expr, path);    break; \
                        default: break;                                           \
                        }                                                         \
                    }while(0)

                inline_rt out_t& operator<<(long long a)            { RTH_DISP(a); return *this; }
                inline_rt out_t& operator<<(long a)                 { return *this << (long long)a; }
                inline_rt out_t& operator<<(int a)                  { return *this << (long long)a; }
                inline_rt out_t& operator<<(short a)                { return *this << (long long)a; }

                inline_rt out_t& operator<<(unsigned long long a)   { RTH_DISP(a); return *this; }
                inline_rt out_t& operator<<(unsigned long a)        { return *this << (unsigned long long)a; }
                inline_rt out_t& operator<<(unsigned a)             { return *this << (unsigned long long)a; }
                inline_rt out_t& operator<<(unsigned short a)       { return *this << (unsigned long long)a; }
                
                inline_rt out_t& operator<<(char a)                 { RTH_DISP(a); return *this; }
                inline_rt out_t& operator<<(signed char a)          { return *this << (char)a; }
                inline_rt out_t& operator<<(unsigned char a)        { return *this << (char)a; }

                inline_rt out_t& operator<<(long double a)          { RTH_DISP(a); return *this; }
                inline_rt out_t& operator<<(double a)               { return *this << (long double)a; }
                inline_rt out_t& operator<<(float a)                { return *this << (long double)a; }

                inline_rt out_t& operator<<(const char* a)          { RTH_DISP(a); return *this; }
                inline_rt out_t& operator<<(char* a)                { return *this << (const char*)a; }

                inline_rt out_t& operator<<(void* a)                { RTH_DISP(a); return *this; }
                template<typename T>
                inline_rt out_t& operator<<(T* a)                   { return *this << (void*)a; }

                #undef RTH_DISP
            };
        }
    }
}

#endif
