/*
 * Copyright (c) 2010-2016 TIBCO Software Inc.
 * All Rights Reserved. Confidential & Proprietary.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: tibexp.h 90137 2016-12-13 19:04:42Z $
 *
 */

/**
 * @file tibexp.h
 * 
 * @brief Macro definitions that mark public functions for Windows DLL
 * import and export.
 */

#ifndef _INCLUDED_tib_tibexp_h
#define _INCLUDED_tib_tibexp_h

#if defined(_WIN32) && !defined(tib_STATIC)
#if defined(tib_EXPORTS)
#define TIB_API __declspec( dllexport )
#else
#define TIB_API __declspec( dllimport )
#endif
#else
#define TIB_API extern
#endif

#if defined (_WIN32) && !defined(tib_STATIC)
#define TIB_IMPORT __declspec( dllimport )
#else
#define TIB_IMPORT extern
#endif

#endif /* _INCLUDED_tib_tibexp_h */
