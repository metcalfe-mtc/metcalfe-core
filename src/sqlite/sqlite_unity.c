//------------------------------------------------------------------------------
/*
  This file is part of mtchaind: https://github.com/MTChain/MTChain-core
  Copyright (c) 2012-2015 MTChain Alliance.
  
  Permission to use, copy, modify, and/or distribute this software for any
  
*/
//==============================================================================

// Prevents sqlite.h from being included, since it screws up the .c
#define MTCHAIN_SQLITE_CPP_INCLUDED

#include <sqlite/sqlite.h>

#ifdef _MSC_VER // MSVC
#pragma warning (push)
#pragma warning (disable: 4100) /* unreferenced formal parameter */
#pragma warning (disable: 4127) /* conditional expression is constant */
#pragma warning (disable: 4232) /* nonstandard extension used: dllimport address */
#pragma warning (disable: 4244) /* conversion from 'int': possible loss of data */
#pragma warning (disable: 4701) /* potentially uninitialized variable */
#pragma warning (disable: 4706) /* assignment within conditional expression */
#pragma warning (disable: 4996) /* 'GetVersionExA' was declared deprecated */
#endif

/* When compiled with SQLITE_THREADSAFE=1, SQLite operates in serialized mode.
   In this mode, SQLite can be safely used by multiple threads with no restriction.

   VFALCO NOTE This implies a global mutex!
*/
#define SQLITE_THREADSAFE 1

/* When compiled with SQLITE_THREADSAFE=2, SQLite can be used in a
   multithreaded program so long as no two threads attempt to use the
   same database connection at the same time.

   VFALCO NOTE This is the preferred threading model.
*/
//#define SQLITE_THREADSAFE 2

#if defined MTCHAIN_SQLITE_USE_NDEBUG && !defined (NDEBUG)
#define NDEBUG
#endif

#include <sqlite/sqlite/sqlite3.c>

#ifdef _MSC_VER
#pragma warning (pop)
#endif

