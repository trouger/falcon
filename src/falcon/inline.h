#ifndef FALCON_INLINE_H
#define FALCON_INLINE_H

/* inlining attributes */

#if defined(SWIG)

#define f_inline
#define n_inline

#elif defined(FALCON_DEBUG)

#if defined(_MSC_VER)

#define f_inline __declspec(noinline)
#define n_inline __declspec(noinline)

#else

#define f_inline __attribute__((noinline))
#define n_inline __attribute__((noinline))

#endif

#else

#if defined(_MSC_VER)

#define f_inline __forceinline
#define n_inline __declspec(noinline)

#else

//#define f_inline inline
//#define f_inline __attribute__((noinline))
#define f_inline __attribute__((always_inline))
#define n_inline __attribute__((noinline))

#endif

#endif

#endif
