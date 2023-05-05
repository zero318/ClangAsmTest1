#pragma once
#if __INTELLISENSE__
#if __clang_major__
#undef __clang_major__
#define __clang_major__ 15
#endif
#ifndef IS_X64
#ifdef _WIN64
#undef _WIN64
#endif
#ifdef _AMD64_
#undef _AMD64_
#endif
#ifdef __x86_64__
#undef __x86_64__
#endif
#endif
#endif
