/* COMPILER(C) */

/* ---------- 1. GCC ---------- */
#if defined(__GNUC__) && !defined(__clang__) && !defined(__INTEL_COMPILER) && !defined(__INTEL_LLVM_COMPILER) && !defined(__MINGW32__) && !defined(__MINGW64__) && !defined(__CYGWIN__)
    

/* ---------- 2. Clang ---------- */
#elif defined(__clang__) && !defined(__INTEL_LLVM_COMPILER) && !defined(__AMD_COMPILER) && !defined(__ARMCC_VERSION) && !defined(__BORLANDC__)
    

/* ---------- 3. MSVC ---------- */
#elif defined(_MSC_VER) && !defined(__clang__) && !defined(__GNUC__)
    

/* ---------- 4. Intel ICX ---------- */
#elif defined(__INTEL_LLVM_COMPILER)
    

/* ---------- 5. AMD AOCC ---------- */
#elif defined(__AMD_COMPILER)
    

/* ---------- 6. NVIDIA nvc ---------- */
#elif defined(__NVCC__) || defined(__NVIDIA_COMPILER)
    

/* ---------- 7. ARMClang ---------- */
#elif defined(__ARMCC_VERSION)
    

/* ---------- 8. Embarcadero C++ Clang ---------- */
#elif defined(__BORLANDC__) && defined(__clang__)
    

/* ---------- 9. IAR C/C++ Compiler ---------- */
#elif defined(__IAR_SYSTEMS_ICC__)
    

/* ---------- 10. Green Hills MULTI ---------- */
#elif defined(__ghs__) && !defined(__clang__)
    

/* ---------- 11. Diab Compiler ---------- */
#elif defined(__DCC__)
    

/* ---------- 12. Tasking C/C++ Compiler ---------- */
#elif defined(__TASKING__)
    

/* ---------- 13. TI C/C++ Compiler ---------- */
#elif defined(__TI_COMPILER_VERSION__)
    

/* ---------- 14. IBM XL C/C++ ---------- */
#elif defined(__xlC__)
    

/* ---------- 15. Oracle Developer Studio ---------- */
#elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)
    

/* ---------- 16. Digital Mars C/C++ ---------- */
#elif defined(__DMC__)
    

/* ---------- 17. TCC ---------- */
#elif defined(__TINYC__)
    

/* ---------- 18. Open64 ---------- */
#elif defined(__OPEN64__)
    

/* ---------- 19. PathScale EKOPath ---------- */
#elif defined(__PATHSCALE__)
    

/* ---------- 20. Borland C++ / Turbo C++ ---------- */
#elif defined(__BORLANDC__) && !defined(__clang__)
    

/* ---------- 21. OpenWatcom ---------- */
#elif defined(__WATCOMC__)
    

/* ---------- 22. Comeau C++ ---------- */
#elif defined(__COMO__)
    

/* ---------- 23. HP aC++ ---------- */
#elif defined(__HP_ACC)
    

/* ---------- 24. SGI MIPSPro C++ ---------- */
#elif defined(__sgi) && defined(_COMPILER_VERSION)
    

/* ---------- 25. AT&T Cfront ---------- */
#elif defined(__CFRONT__)
    

/* ---------- 26. MinGW32 ---------- */
#elif defined(__MINGW32__) && !defined(__MINGW64__)
    

/* ---------- 27. MinGW-w64 i686 ---------- */
#elif defined(__MINGW64__) && defined(__i386__)
    

/* ---------- 28. MinGW-w64 x86_64 ---------- */
#elif defined(__MINGW64__) && defined(__x86_64__)
    

/* ---------- 29. TDM-GCC 32 ---------- */
#elif defined(__TDM_GCC) && !defined(__x86_64__)
    

/* ---------- 30. TDM-GCC 64 ---------- */
#elif defined(__TDM_GCC) && defined(__x86_64__)
    

/* ---------- 31. MSYS2 MINGW32 ---------- */
#elif defined(__MSYS__) && !defined(_WIN64) && defined(__MINGW32__)
    

/* ---------- 32. MSYS2 MINGW64 ---------- */
#elif defined(__MSYS__) && defined(_WIN64) && defined(__MINGW64__) && !defined(__UCRT__)
    

/* ---------- 33. MSYS2 UCRT64 ---------- */
#elif defined(__MSYS__) && defined(__UCRT__)
    

/* ---------- 34. MSYS2 CLANG64 ---------- */
#elif defined(__MSYS__) && defined(__clang__)
    

/* ---------- 35. Cygwin GCC ---------- */
#elif defined(__CYGWIN__)
    

/* ---------- 36. WinLibs GCC ---------- */
#elif defined(__WINLIBGCC__)
    

/* ---------- 37. DJGPP ---------- */
#elif defined(__DJGPP__)
    

/* ---------- 38. Pelles C ---------- */
#elif defined(__POCC__)
    

/* ---------- 39. LCC-Win32 ---------- */
#elif defined(__LCC__)
    

/* ---------- 40. SDCC ---------- */
#elif defined(__SDCC)
    

/* ---------- Unknown ---------- */
#else
    
#endif

int main(void){return 0;}
