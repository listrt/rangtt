namespace rttype{
template<typename T>
struct st{
    T val;
    constexpr st() noexcept : val{} {}
    template<typename U>
    constexpr st(U v) noexcept
        : val(static_cast<T>(v))
    {}

    constexpr operator T() const noexcept{
        return val;
    }

    constexpr st& operator++() noexcept{
        ++val;
        return *this;
    }
    constexpr st operator++(int) noexcept{
        st tmp = *this;
        ++val;
        return tmp;
    }

    constexpr st& operator--() noexcept{
        --val;
        return *this;
    }
    constexpr st operator--(int) noexcept{
        st tmp=*this;
        --val;
        return tmp;
    }

    constexpr st& operator+=(T n) noexcept { val += n; return *this; }
    constexpr st& operator-=(T n) noexcept { val -= n; return *this; }
    constexpr st& operator*=(T n) noexcept { val *= n; return *this; }
    constexpr st& operator/=(T n) noexcept { val /= n; return *this; }
    constexpr st& operator%=(T n) noexcept { val %= n; return *this; }

    constexpr st& operator&=(T n) noexcept { val &= n; return *this; }
    constexpr st& operator|=(T n) noexcept { val |= n; return *this; }
    constexpr st& operator^=(T n) noexcept { val ^= n; return *this; }
    constexpr st& operator<<=(T n) noexcept { val <<= n; return *this; }
    constexpr st& operator>>=(T n) noexcept { val >>= n; return *this; }
};

template<typename T>
constexpr st<T> operator+(st<T> a, st<T> b) noexcept { return st<T>(a.val + b.val); }
template<typename T>
constexpr st<T> operator-(st<T> a, st<T> b) noexcept { return st<T>(a.val - b.val); }
template<typename T>
constexpr st<T> operator*(st<T> a, st<T> b) noexcept { return st<T>(a.val * b.val); }
template<typename T>
constexpr st<T> operator/(st<T> a, st<T> b) noexcept { return st<T>(a.val / b.val); }
template<typename T>
constexpr st<T> operator%(st<T> a, st<T> b) noexcept { return st<T>(a.val % b.val); }

template<typename T>
constexpr st<T> operator&(st<T> a, st<T> b) noexcept { return st<T>(a.val & b.val); }
template<typename T>
constexpr st<T> operator|(st<T> a, st<T> b) noexcept { return st<T>(a.val | b.val); }
template<typename T>
constexpr st<T> operator^(st<T> a, st<T> b) noexcept { return st<T>(a.val ^ b.val); }
template<typename T>
constexpr st<T> operator<<(st<T> a, st<T> b) noexcept { return st<T>(a.val << b.val); }
template<typename T>
constexpr st<T> operator>>(st<T> a, st<T> b) noexcept { return st<T>(a.val >> b.val); }

template<typename T>
constexpr int operator==(st<T> a, st<T> b) noexcept { return a.val == b.val; }
template<typename T>
constexpr int operator!=(st<T> a, st<T> b) noexcept { return a.val != b.val; }
template<typename T>
constexpr int operator< (st<T> a, st<T> b) noexcept { return a.val <  b.val; }
template<typename T>
constexpr int operator> (st<T> a, st<T> b) noexcept { return a.val >  b.val; }
template<typename T>
constexpr int operator<=(st<T> a, st<T> b) noexcept { return a.val <= b.val; }
template<typename T>
constexpr int operator>=(st<T> a, st<T> b) noexcept { return a.val >= b.val; }

template<typename T>
struct st<T*>{
    T* val;
    constexpr st() noexcept : val(nullptr) {}
    constexpr st(T* p) noexcept : val(p) {}
    constexpr operator T*() const noexcept { return val; }
    constexpr st& operator++() noexcept { ++val; return *this; }
    constexpr st operator++(int) noexcept{
        st tmp = *this;
        ++val;
        return tmp;
    }
    constexpr st& operator--() noexcept { --val; return *this; }
    constexpr st operator--(int) noexcept{
        st tmp=*this;
        --val;
        return tmp;
    }
    constexpr st& operator+=(ptrdiff_t off) noexcept { val += off; return *this; }
    constexpr st& operator-=(ptrdiff_t off) noexcept { val -= off; return *this; }
};

template<typename T>
constexpr int operator==(st<T*> a, st<T*> b) noexcept { return a.val == b.val; }
template<typename T>
constexpr int operator!=(st<T*> a, st<T*> b) noexcept { return a.val != b.val; }
template<typename T>
constexpr int operator< (st<T*> a, st<T*> b) noexcept { return a.val <  b.val; }
template<typename T>
constexpr int operator> (st<T*> a, st<T*> b) noexcept { return a.val >  b.val; }
template<typename T>
constexpr int operator<=(st<T*> a, st<T*> b) noexcept { return a.val <= b.val; }
template<typename T>
constexpr int operator>=(st<T*> a, st<T*> b) noexcept { return a.val >= b.val; }

using byte_t = st<unsigned char>;
}
