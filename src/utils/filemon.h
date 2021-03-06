/* vifm
 * Copyright (C) 2015 xaizek.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
 */

#ifndef VIFM__UTILS__FILEMON_H__
#define VIFM__UTILS__FILEMON_H__

#include <sys/types.h> /* dev_t ino_t */

#include <time.h> /* time_t timespec */

/* Various time stamp service functions. */

/* Storage for file monitoring information. */
typedef struct
{
#ifdef HAVE_STRUCT_STAT_ST_MTIM
	struct timespec ts;
#else
	time_t ts;
#endif
	dev_t dev;
	ino_t inode;
}
filemon_t;

/* Sets file monitor from a file.  Returns zero on success, otherwise non-zero
 * is returned. */
int filemon_from_file(const char path[], filemon_t *timestamp);

/* Checks whether two timestamps are equal.  Returns non-zero if so, otherwise
 * zero is returned. */
int filemon_equal(const filemon_t *a, const filemon_t *b);

/* Assigns value of the *rhs to *lhs. */
void filemon_assign(filemon_t *lhs, const filemon_t *rhs);

#endif /* VIFM__UTILS__FILEMON_H__ */

/* vim: set tabstop=2 softtabstop=2 shiftwidth=2 noexpandtab cinoptions-=(0 : */
/* vim: set cinoptions+=t0 : */
