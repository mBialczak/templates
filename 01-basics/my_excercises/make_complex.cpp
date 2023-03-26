#include <complex>
#include <iostream>
#include <type_traits>

template <typename RealType,
          typename ImgType,
          typename = std::enable_if_t<!std::is_same_v<RealType, ImgType>>>
std::complex<RealType> make_complex(RealType re, ImgType im)
{
    return std::complex<RealType>(std::forward<RealType>(re), std::forward<RealType>(im));
}

int main(int argc, const char** argv)
{
    std::complex number = make_complex(1, 2.3);
    // std::complex number2 = make_complex(1.1, 2.3); // should not compile

    std::cout << number << std::endl;
    // std::cout << number2 << std::endl;

    return 0;
}