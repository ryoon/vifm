/* vifm
 * Copyright (C) 2001 Ken Steen.
 * Copyright (C) 2011 xaizek.
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

#ifndef VIFM__RUNNING_H__
#define VIFM__RUNNING_H__

#include "ui/ui.h"

/* Kinds of executable file treatment on file handling. */
typedef enum
{
	FHE_NO_RUN,          /* Don't run. */
	FHE_RUN,             /* Run with rights of current user. */
	FHE_ELEVATE_AND_RUN, /* Run with rights elevation (if available). */
}
FileHandleExec;

/* Kinds of symbolic link file treatment on file handling. */
typedef enum
{
	FHL_NO_FOLLOW, /* Don't follow (navigate to instead of navigation inside). */
	FHL_FOLLOW,    /* Follow (end up on the link target, not inside it). */
}
FileHandleLink;

/* Handles opening of current file/selection of the view. */
void handle_file(FileView *view, FileHandleExec exec, FileHandleLink follow);

void run_using_prog(FileView *view, const char program[], int dont_execute,
		int force_background);

void handle_dir(FileView *view);

void cd_updir(FileView *view);

/* Values of pause:
 *  > 0 - pause always
 *  = 0 - do not pause
 *  < 0 - pause on error
 * Returns zero on success, otherwise non-zero is returned. */
int shellout(const char command[], int pause, int use_term_multiplexer);

void output_to_nowhere(const char cmd[]);

/* Returns zero on successful running. */
int run_with_filetype(FileView *view, const char beginning[], int background);

#endif /* VIFM__RUNNING_H__ */

/* vim: set tabstop=2 softtabstop=2 shiftwidth=2 noexpandtab cinoptions-=(0 : */
/* vim: set cinoptions+=t0 : */
