/*
 * Copyright (c) 2011-2016 TIBCO Software Inc.
 * All Rights Reserved. Confidential & Proprietary.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: tibgrpex.h 90137 2016-12-13 19:04:42Z $
 *
 */

#ifndef _INCLUDED_tibgroup_tibgrpex_h
#define _INCLUDED_tibgroup_tibgrpex_h

#if defined(_WIN32) && !defined(tibgroup_STATIC)
#if defined(tibgroup_EXPORTS)
#define TIBGROUP_API __declspec( dllexport )
#else
#define TIBGROUP_API __declspec( dllimport )
#endif
#else
#define TIBGROUP_API extern
#endif

#endif /* _INCLUDED_tibgroup_tibgrpex_h */
