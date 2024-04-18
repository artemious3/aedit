#ifndef B6E84D0E_D0A9_4B4E_91F7_3853223A1AE6
#define B6E84D0E_D0A9_4B4E_91F7_3853223A1AE6

#include "coretypes.h"
#include <cmath>
#include <complex>
#include <vector>
#include <valarray>

class Utils{

public:
typedef std::valarray<std::complex<float>> Frequencies;
    

private:
    static Frequencies _fft(const Frequencies& freqs){
        auto size = freqs.size();
        auto half_size = size / 2;

        if(size == 1){
            return {freqs[0]};
        } 


        Frequencies odd (half_size), even (half_size);
        for(int i = 0; i < half_size; ++i){
            even[i] = freqs[ i * 2 ];
            odd[i] = freqs [ i * 2 + 1];
        }

        odd = _fft(odd);
        even = _fft(even);

        Frequencies res(size);
        for(int i = 0; i < half_size; ++i){
            auto a = even[i];
            auto b = std::exp(std::complex<float>(0, - 2 * M_PI * i / size)) * odd[i];
            res[i] = a + b;
            res[i + half_size] = a - b;
        }

        return res;
    }


public:


    static Frequencies fft(const Sample* buf, int size){
        Frequencies freqs;
        
        freqs.resize(size);
        for(int i = 0; i < size; ++i){
            freqs[i] = buf[i];
        }

        return _fft(freqs);
    }

    static void ifft_additive(const Frequencies& freqs, Sample* out, size_t max_len){
        Frequencies freqs_conj = freqs.apply(std::conj);
        freqs_conj = _fft(freqs_conj);
        freqs_conj = freqs_conj.apply(std::conj);

        auto size = std::min(freqs_conj.size(), max_len);

        for(int i = 0; i < size; ++i){
            out[i]  +=  freqs_conj[i].real() / size;
        }
    }


};

#endif /* B6E84D0E_D0A9_4B4E_91F7_3853223A1AE6 */
