#if defined(__cplusplus)
#  if __cplusplus>=201402L
template<typename T>
struct is_ptr_v{
    static constexpr int v=0;
};
template<typename U>
struct is_ptr_v<U*>{
    static constexpr int v=1+is_ptr_v<U>::v;
};
template<typename U> struct is_ptr_v<U**>{				static constexpr int v= 2+is_ptr_v<U>::v;};
template<typename U> struct is_ptr_v<U****>{			static constexpr int v= 4+is_ptr_v<U>::v;};
template<typename U> struct is_ptr_v<U********>{		static constexpr int v= 8+is_ptr_v<U>::v;};
template<typename U> struct is_ptr_v<U****************>{static constexpr int v=16+is_ptr_v<U>::v;};

#if 0
#define is_ptr_batch_rt(r,t) \
template<typename U> struct is_ptr_v<U r>{static constexpr int v=t+is_ptr_v<U>::v;};
#endif

template<typename T>
constexpr int is_ptr=is_ptr_v<T>::v;
#  elif __cplusplus==201103L
#pragma message("C++11?")
#  else
#pragma message("C++98/C++03?")
#  endif

#else
#pragma message("C?")
#endif

int main(void){return 0;}
