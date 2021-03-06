/*
 *  Copyright (C) 2004-2009, Eric Lund
 *  http://www.mvpmc.org/
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 * debug.c - functions to produce and control debug output from
 *           libcmyth routines.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <refmem_local.h>

#include "mvp_debug.h"

static cmyth_debug_ctx_t refmem_debug_ctx = CMYTH_DEBUG_CTX_INIT("refmem",
								 REF_DBG_COUNTERS,
								 NULL);
/*
 * refmem_dbg_level(int l)
 * 
 * Scope: PUBLIC
 *
 * Description
 *
 * Set the current debug level to the absolute setting 'l'
 * permitting all debug messages with a debug level less
 * than or equal to 'l' to be displayed.
 *
 * Return Value:
 *
 * None.
 */
void
refmem_dbg_level(int l)
{
	__cmyth_dbg_setlevel(&refmem_debug_ctx, l);
}

/*
 * refmem_dbg_all()
 * 
 * Scope: PUBLIC
 * 
 * Description
 *
 * Set the current debug level so that all debug messages are displayed.
 *
 * Return Value:
 *
 * None.
 */
void
refmem_dbg_all()
{
	__cmyth_dbg_setlevel(&refmem_debug_ctx, REF_DBG_ALL);
}

/*
 * refmem_dbg_none()
 * 
 * Scope: PUBLIC
 * 
 * Description
 *
 * Set the current debug level so that no debug messages are displayed.
 *
 * Return Value:
 *
 * None.
 */
void
refmem_dbg_none()
{
	__cmyth_dbg_setlevel(&refmem_debug_ctx, REF_DBG_NONE);
}

/*
 * refmem_dbg()
 * 
 * Scope: PRIVATE (mapped to __refmem_dbg)
 * 
 * Description
 *
 * Print a debug message of level 'level' on 'stderr' provided that
 * the current debug level allows messages of level 'level' to be
 * printed.
 *
 * Return Value:
 *
 * None.
 */
void
refmem_dbg(int level, char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	__cmyth_dbg(&refmem_debug_ctx, level, fmt, ap);
	va_end(ap);
}
