 ACPC FFT/NTT Quick Decision Guide
 
1) Multiply / Convolve
- Small / moderate integer coefficients
    -> FFT::convolution_ll(a, b)
    Safe as long as the final values stay roughly < 1e15 (fast).
 
- Huge coefficients (e.g., around 1e9) or you need guaranteed exactness
    -> convolution_mod_any(a, b, MOD)
    Exact via CRT (slower ~3 NTTs, but safe).
 
- Mod = 998244353
    -> NTT998::convolution(a, b)
    Always exact + fastest modular option.
 
- Mod = 1e9+7 (or any other MOD)
    -> convolution_mod_any(a, b, MOD)
    Exact for any MOD.
 
- Small MOD (like 2000)
    -> FFT::convolution_ll(a, b) then take % MOD
    Works if values don’t overflow long long and rounding remains safe.
 
2) Polynomial advanced ops (only under mod 998244353)
    -> Poly998::{inverse, ln, exp, pow_poly, sqrt_poly}
    Used for combinatorics/partitions/generating functions.
 
3) String matching
- Fast (usually safe)
    -> StringFFT::exactMatch(T, P)
    Uses the squared-error trick (very fast, tiny float-risk).
 
- Safest (robust)
    -> StringFFT::atMostKMismatch(T, P, 0)
    Exact Hamming with k=0 (slower, but extremely reliable).
 
4) Pairs with i - j = k
    -> FFT::cross_correlation(a, b)
    Then S(k) is at index: corr[k + (int)b.size() - 1]
 
i−j=k	FFT::cross_correlation	Result index: res[k + B.size() - 1].
========================================================
          FFT / NTT "SOLVE ANY CONVOLUTION" TEMPLATE
========================================================
 
FFT (double):
  - convolution_ll (rounded)            [good for moderate coefficients]
  - convolution_double
  - cross_correlation / autocorrelation
  - string match exact / wildcard '?' / <=k mismatches (Hamming)
 
NTT (mod 998244353):
  - exact convolution
  - Poly: inv, log, exp, pow, sqrt  (O(n log n))
 
Arbitrary mod convolution:
  - convolution_mod_any(a,b,MOD) using CRT (3 NTT primes)
    -> use for mod 1e9+7, etc.
 
Big Integer:
  - multiply(a,b)  (decimal strings, supports sign)
  - power(a, e)
 
RULE OF THUMB:
  - exact modular polys: NTT
  - correlations / strings / bigint: FFT
  - mod != 998244353: CRT convolution_mod_any
========================================================