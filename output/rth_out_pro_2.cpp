/*rth_out_pro_2*/
#ifndef rop_out2
#define rop_out2

#include<rth_io>

#undef LDBL_MAX_RT
#if defined(__LDBL_MAX__)
    #define LDBL_MAX_RT ((long double)__LDBL_MAX__)
#elif defined(_MSC_VER)
    #define LDBL_MAX_RT 1.79769313486231570815e+308L
#elif defined(__SIZEOF_LONG_DOUBLE__) && (__SIZEOF_LONG_DOUBLE__ == 4)
    #define LDBL_MAX_RT 3.40282346638528859812e+38L
#else
    #define LDBL_MAX_RT 1.79769313486231570815e+308L
#endif
//set up
namespace rttype{
    namespace{
        constexpr unsigned ptr_w_rt=sizeof(void*)<<1;
        constexpr char hexlist[]={
            '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'
        };
        
        struct out_t{};
        constexpr rttype::out_t out={};
    }
}
namespace outrt{
    namespace runtime{
        static inline char* lltoa(long long val, char* buf){
            char* p=buf+20;
            *p=0;
            unsigned long long u;
            if(val<0){
                u = (unsigned long long)(-val);
                do{
                    unsigned long long rem = u % 10;
                    *--p='0' | (char)rem;
                    u /= 10;
                }while(u);
                *--p='-';
            }
            else{
                u=(unsigned long long)val;
                do{
                    unsigned long long rem = u%10;
                    *--p='0' | (char)rem;
                    u/=10;
                }while (u);
            }
            return p;
        }
        static inline char* ulltoa(unsigned long long val, char* buf){
            char* p=buf+20;
            *p=0;
            do{
                unsigned long long rem = val % 10;
                *--p='0' | (char)rem;
                val/=10;
            }while(val);
            return p;
        }
        
        namespace out_fn{
            static inline int si(long long a){
                char c[21];
                char* p=lltoa(a,c);
                unsigned len=(c+20)-p;
                writer(p, len);
                return 0;
            }
            static inline int ui(unsigned long long a){
                char c[21];
                char* p=ulltoa(a,c);
                unsigned len=(c+20)-p;
                writer(p, len);
                return 0;
            }
            static inline int ch(char c){
                writer(&c,1);
                return 0;
            }
            static inline int cp(const char* c) {
                unsigned l=0;
                while (c[l]) l++;
                writer(c,l);
                return 0;
            }
            template<typename T>
            static inline int vp(T* a){
                char c[rttype::ptr_w_rt+2]={};
                c[0]='0';
                c[1]='x';
                unsigned long long ax=(unsigned long long)a;
                for(int i=0;i<rttype::ptr_w_rt;i++){
                    int shift=(rttype::ptr_w_rt-1-i) <<2;
                    c[2+i]=rttype::hexlist[(ax>>shift)&0xF];
                }
                writer(c,rttype::ptr_w_rt+2);
                return 0;
            }
            static inline int fp(long double a) {
                if (a==0) {
                    writer("0", 1);
                    return 0;
                }
                if (a!=a) {
                    writer("NaN", 3);
                    return 0;
                }
                char c[64];
                int idx = 0;
                bool neg = (a < 0);
                if (neg) a = -a;
                if (a > LDBL_MAX_RT) {
                    if (neg) writer("-INF", 4);
                    else     writer("INF", 3);
                    return 0;
                }
                constexpr long double ULL_MAX_P1 = 18446744073709551616.0L; //2^64
                if (a>=ULL_MAX_P1){
                    if (neg) writer("-INF", 4);
                    else     writer("INF", 3);
                    return 0;
                }
                unsigned long long ip = (unsigned long long)a;
                long double frac = a - (long double)ip;

                char buf[32];
                int bi = 0;
                do {
                    buf[bi++] = '0' ^ (int)(ip % 10);
                    ip /= 10;
                } while (ip);
                while (bi--) c[idx++] = buf[bi];

                c[idx++] = '.';

                for (int i = 0; i < 6; i++) {
                    frac *= 10;
                    int digit = (int)frac;
                    c[idx++] = '0' ^ digit;
                    frac -= digit;
                }

                while (idx > 0 && c[idx-1] == '0') idx--;
                if (idx > 0 && c[idx-1] == '.') idx--;

                writer(c, idx);
                return 0;
            }
            //error -> return -1;
            template<typename... A>
            static inline int si(A... a){return -1;}
            template<typename... A>
            static inline int ui(A... a){return -1;}
            template<typename... A>
            static inline int ch(A... a){return -1;}
            template<typename... A>
            static inline int cp(A... a){return -1;}
            template<typename... A>
            static inline int vp(A... a){return -1;}
            template<typename... A>
            static inline int fp(A... a){return -1;}
            static inline int fp(float a){ return fp((long double)a);}
            static inline int fp(double a){return fp((long double)a);}
            namespace inlinefn{
                static inline int si(long long a) {
                    char c[21];
                    char* p=c+20;
                    *p=0;
                    unsigned long long u;
                    if(a<0){
                        u=(unsigned long long)(-a);
                        do{
                            unsigned long long rem=u%10;
                            *--p='0'|(char)rem;
                            u/=10;
                        }while(u);
                        *--p='-';
                    }else{
                        u=(unsigned long long)a;
                        do{
                            unsigned long long rem=u%10;
                            *--p='0'|(char)rem;
                            u/=10;
                        }while (u);
                    }
                    unsigned len=(c+20)-p;
                    writer(p, len);
                    return 0;
                }
                static inline int ui(unsigned long long a) {
                    char c[21];
                    char* p=c+20;
                    *p=0;

                    do{
                        unsigned long long rem=a%10;
                        *--p='0'|(char)rem;
                        a/=10;
                    }while(a);

                    unsigned len=(c+20)-p;
                    writer(p,len);
                    return 0;
                }
                static inline int ch(char c){
                    writer(&c,1);
                    return 0;
                }
                static inline int cp(const char* c) {
                    unsigned l=0;
                    while (c[l]) l++;
                    writer(c,l);
                    return 0;
                }
                template<typename T>
                static inline int vp(T* a){
                    char c[rttype::ptr_w_rt+2]={};
                    c[0]='0';
                    c[1]='x';
                    unsigned long long ax=(unsigned long long)a;
                    for(int i=0;i<rttype::ptr_w_rt;i++){
                        int shift=(rttype::ptr_w_rt-1-i) <<2;
                        c[2+i]=rttype::hexlist[(ax>>shift)&0xF];
                    }
                    writer(c,rttype::ptr_w_rt+2);
                    return 0;
                }
                static inline int fp(long double a) {
                    if(a==0){
                        writer("0", 1);
                        return 0;
                    }
                    if(a!=a){
                        writer("NaN", 3);
                        return 0;
                    }
                    char c[64];
                    int idx=0;
                    bool neg=(a<0);
                    if (neg)a=-a;
                    if (a>LDBL_MAX_RT) {
                        if(neg)writer("-INF",4);
                        else   writer("INF", 3);
                        return 0;
                    }
                    constexpr long double ULL_MAX_P1 = 18446744073709551616.0L; //2^64
                    if (a>=ULL_MAX_P1){
                        if(neg)writer("-INF",4);
                        else   writer("INF", 3);
                        return 0;
                    }
                    unsigned long long ip=(unsigned long long)a;
                    long double frac=a-(long double)ip;

                    char buf[32];
                    int bi=0;
                    do{
                        buf[bi++]='0' ^ (int)(ip % 10);
                        ip/=10;
                    }while(ip);
                    while(bi--) c[idx++]=buf[bi];

                    c[idx++]='.';

                    for (int i=0;i<6;i++) {
                        frac *= 10;
                        int digit = (int)frac;
                        c[idx++] = '0' ^ digit;
                        frac -= digit;
                    }

                    while(idx>0 && c[idx-1]=='0') idx--;
                    if   (idx>0 && c[idx-1]=='.') idx--;

                    writer(c,idx);
                    return 0;
                }
                //error -> return -1;
                template<typename... A>
                static inline int si(A... a){return -1;}
                template<typename... A>
                static inline int ui(A... a){return -1;}
                template<typename... A>
                static inline int ch(A... a){return -1;}
                template<typename... A>
                static inline int cp(A... a){return -1;}
                template<typename... A>
                static inline int vp(A... a){return -1;}
                template<typename... A>
                static inline int fp(A... a){return -1;}
                static inline int fp(float a){ return fp((long double)a);}
                static inline int fp(double a){return fp((long double)a);}
            }
        }
        #define defofn_fp(T) \
        static inline rttype::out_t outf(T a) {\
            out_fn::fp((long double)a);\
            return {};\
        }
        #define defofn_si(T) \
        static inline rttype::out_t outf(T a) {\
            out_fn::si((long long)a);\
            return {};\
        }
        #define defofn_ui(T) \
        static inline rttype::out_t outf(T a) {\
            out_fn::ui((unsigned long long)a);\
            return {};\
        }
        #define defofn_ch(T) \
        static inline rttype::out_t outf(T a) {\
            const char c=static_cast<char>(a);\
            writer(&c,1);\
            return {};\
        }
        defofn_fp(float);
        defofn_fp(double);
        defofn_fp(long double);

        defofn_si(short);
        defofn_si(int);
        defofn_si(long);
        defofn_si(long long);

        defofn_ui(unsigned short);
        defofn_ui(unsigned int);
        defofn_ui(unsigned long);
        defofn_ui(unsigned long long);

        defofn_ch(char);
        defofn_ch(signed char);
        defofn_ch(unsigned char);
        
        #undef defofn_fp
        #undef defofn_si
        #undef defofn_ui
        #undef defofn_ch
        static inline rttype::out_t outf(char* a){
            out_fn::cp(a);
            return {};
        }
        static inline rttype::out_t outf(const char* a){
            out_fn::cp(a);
            return {};
        }
        static inline rttype::out_t outf(bool a) {
            if(a){writer("1",1);}
            else{writer("0",1);}
            return {};
        }
        template<typename T>
        static inline rttype::out_t outf(T* a){
            out_fn::vp(a);
            return {};
        }
        template<typename... A>
        static inline rttype::out_t outf(A... a)
        {return {};}
        namespace inlinefn{
            #define defofn_fp(T) \
            static inline rttype::out_t outf(T a) {\
                long double a_=(long double)a;\
                if(a_==0){\
                    writer("0", 1);\
                    return {};\
                }\
                if(a_!=a_){\
                    writer("NaN", 3);\
                    return {};\
                }\
                char c[64];\
                int idx=0;\
                bool neg=(a_<0);\
                if (neg)a_=-a_;\
                if (a_>LDBL_MAX_RT) {\
                    if(neg)writer("-INF",4);\
                    else   writer("INF", 3);\
                    return {};\
                }\
                constexpr long double ULL_MAX_P1 = 18446744073709551616.0L; /*2^64*/\
                if (a_>=ULL_MAX_P1){\
                    if(neg)writer("-INF",4);\
                    else   writer("INF", 3);\
                    return {};\
                }\
                unsigned long long ip=(unsigned long long)a_;\
                long double frac=a_-(long double)ip;\
                char buf[32];\
                int bi=0;\
                do{\
                    buf[bi++]='0' ^ (int)(ip % 10);\
                    ip/=10;\
                }while(ip);\
                while(bi--) c[idx++]=buf[bi];\
                c[idx++]='.';\
                for (int i=0;i<6;i++) {\
                    frac *= 10;\
                    int digit = (int)frac;\
                    c[idx++] = '0' ^ digit;\
                    frac -= digit;\
                }\
                while(idx>0 && c[idx-1]=='0') idx--;\
                if   (idx>0 && c[idx-1]=='.') idx--;\
                writer(c,idx);\
                return {};\
            }
            #define defofn_si(T) \
            static inline rttype::out_t outf(T a) {\
                long long a_=(long long)a;\
                char c[21];\
                char* p=c+20;\
                *p=0;\
                unsigned long long u;\
                if(a_<0){\
                    u=(unsigned long long)(-a_);\
                    do{\
                        unsigned long long rem=u%10;\
                        *--p='0'|(char)rem;\
                        u/=10;\
                    }while(u);\
                    *--p='-';\
                }else{\
                    u=(unsigned long long)a_;\
                    do{\
                        unsigned long long rem=u%10;\
                        *--p='0'|(char)rem;\
                        u/=10;\
                    }while (u);\
                }\
                unsigned len=(c+20)-p;\
                writer(p, len);\
                return {};\
            }
            #define defofn_ui(T) \
            static inline rttype::out_t outf(T a) {\
                unsigned long long a_=(unsigned long long)a;\
                char c[21];\
                char* p=c+20;\
                *p=0;\
                do{\
                    unsigned long long rem=a_%10;\
                    *--p='0'|(char)rem;\
                    a_/=10;\
                }while(a_);\
                unsigned len=(c+20)-p;\
                writer(p,len);\
                return {};\
            }
            #define defofn_ch(T) \
            static inline rttype::out_t outf(T a) {\
                const char c=static_cast<char>(a);\
                writer(&c,1);\
                return {};\
            }
            defofn_fp(float);
            defofn_fp(double);
            defofn_fp(long double);

            defofn_si(short);
            defofn_si(int);
            defofn_si(long);
            defofn_si(long long);

            defofn_ui(unsigned short);
            defofn_ui(unsigned int);
            defofn_ui(unsigned long);
            defofn_ui(unsigned long long);

            defofn_ch(char);
            defofn_ch(signed char);
            defofn_ch(unsigned char);
            
            #undef defofn_fp
            #undef defofn_si
            #undef defofn_ui
            #undef defofn_ch
        }
    }
}
#define def_op_rtout_si(T) \
static inline rttype::out_t operator<<(rttype::out_t a, T b){\
    long long a_=(long long)b;\
    char c[21];\
    char* p=c+20;\
    *p=0;\
    unsigned long long u;\
    if(a_<0){\
        u=(unsigned long long)(-a_);\
        do{\
            unsigned long long rem=u%10;\
            *--p='0'|(char)rem;\
            u/=10;\
        }while(u);\
        *--p='-';\
    }else{\
        u=(unsigned long long)a_;\
        do{\
            unsigned long long rem=u%10;\
            *--p='0'|(char)rem;\
            u/=10;\
        }while (u);\
    }\
    unsigned len=(c+20)-p;\
    writer(p, len);\
    return a;\
}
#define def_op_rtout_ui(T) \
static inline rttype::out_t operator<<(rttype::out_t a, T b){\
    unsigned long long a_=(unsigned long long)b;\
    char c[21];\
    char* p=c+20;\
    *p=0;\
    do{\
        unsigned long long rem=a_%10;\
        *--p='0'|(char)rem;\
        a_/=10;\
    }while(a_);\
    unsigned len=(c+20)-p;\
    writer(p,len);\
    return a;\
}
#define def_op_rtout_fp(T) \
static inline rttype::out_t operator<<(rttype::out_t a, T b){\
    long double a_=(long double)b;\
    if(a_==0){\
        writer("0", 1);\
        return a;\
    }\
    if(a_!=a_){\
        writer("NaN", 3);\
        return a;\
    }\
    char c[64];\
    int idx=0;\
    bool neg=(a_<0);\
    if (neg)a_=-a_;\
    if (a_>LDBL_MAX_RT) {\
        if(neg)writer("-INF",4);\
        else   writer("INF", 3);\
        return a;\
    }\
    constexpr long double ULL_MAX_P1 = 18446744073709551616.0L; /*2^64*/\
    if (a_>=ULL_MAX_P1){\
        if(neg)writer("-INF",4);\
        else   writer("INF", 3);\
        return a;\
    }\
    unsigned long long ip=(unsigned long long)a_;\
    long double frac=a_-(long double)ip;\
    char buf[32];\
    int bi=0;\
    do{\
        buf[bi++]='0' ^ (int)(ip % 10);\
        ip/=10;\
    }while(ip);\
    while(bi--) c[idx++]=buf[bi];\
    c[idx++]='.';\
    for (int i=0;i<6;i++) {\
        frac *= 10;\
        int digit = (int)frac;\
        c[idx++] = '0' ^ digit;\
        frac -= digit;\
    }\
    while(idx>0 && c[idx-1]=='0') idx--;\
    if   (idx>0 && c[idx-1]=='.') idx--;\
    writer(c,idx);\
    return a;\
}
#define def_op_rtout_ch(T) \
static inline rttype::out_t operator<<(rttype::out_t a, T b){\
    const char c=static_cast<char>(b);\
    writer(&c,1);\
    return a;\
}

def_op_rtout_si(short);
def_op_rtout_si(int);
def_op_rtout_si(long);
def_op_rtout_si(long long);

def_op_rtout_ui(unsigned short);
def_op_rtout_ui(unsigned int);
def_op_rtout_ui(unsigned long);
def_op_rtout_ui(unsigned long long);

def_op_rtout_fp(float);
def_op_rtout_fp(double);
def_op_rtout_fp(long double);

def_op_rtout_ch(char);
def_op_rtout_ch(signed char);
def_op_rtout_ch(unsigned char);

template<typename T>
static inline rttype::out_t operator<<(rttype::out_t a, T b) {
    outrt::runtime::outf(b);
    return a;
}
template<typename T>
static inline rttype::out_t operator<<(rttype::out_t a, T* b) {
    char c[rttype::ptr_w_rt+2]={};
    c[0]='0';
    c[1]='x';
    unsigned long long ax=(unsigned long long)b;
    for(int i=0;i<rttype::ptr_w_rt;i++){
        int shift=(rttype::ptr_w_rt-1-i) <<2;
        c[2+i]=rttype::hexlist[(ax>>shift)&0xF];
    }
    writer(c,rttype::ptr_w_rt+2);
    return a;
}
static inline rttype::out_t operator<<(rttype::out_t a, char* b) {
    const char* c=b;
    unsigned l=0;
    while (c[l]) l++;
    writer(c, l);
    return a;
}
static inline rttype::out_t operator<<(rttype::out_t a, const char* b) {
    unsigned l=0;
    while (b[l]) l++;
    writer(b, l);
    return a;
}
static inline rttype::out_t operator<<(rttype::out_t a, rttype::out_t (*manip)(rttype::out_t)) {
    return manip(a);
}
#endif  /**/
