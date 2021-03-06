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

#include "filetypes_menu.h"

#include <stdio.h> /* snprintf() */
#include <stdlib.h> /* free() */
#include <string.h> /* strdup() strlen() */

#include "../modes/menu.h"
#include "../ui/ui.h"
#include "../utils/fs_limits.h"
#include "../utils/macros.h"
#include "../utils/str.h"
#include "../utils/string_array.h"
#include "../file_magic.h"
#include "../filelist.h"
#include "../filetype.h"
#include "../running.h"
#include "../types.h"
#include "menus.h"

static const char * form_filetype_menu_entry(assoc_record_t prog,
		int descr_width);
static const char * form_filetype_data_entry(assoc_record_t prog);
static int execute_filetype_cb(FileView *view, menu_info *m);
static void fill_menu_from_records(menu_info *m,
		const assoc_records_t *records);
static int max_desc_len(const assoc_records_t *records);

int
show_file_menu(FileView *view, int background)
{
	static menu_info m;

	int i;
	int max_len;

	char *const typed_name = get_typed_current_fname(view);
	assoc_records_t ft = ft_get_all_programs(typed_name);
	assoc_records_t magic = get_magic_handlers(typed_name);
	free(typed_name);

	init_menu_info(&m, FILE_MENU, strdup("No programs set for this filetype"));

	m.title = strdup(" Filetype associated commands ");
	m.execute_handler = &execute_filetype_cb;
	m.extra_data = (background ? 1 : 0);

	max_len = MAX(max_desc_len(&ft), max_desc_len(&magic));

	for(i = 0; i < ft.count; i++)
	{
		(void)add_to_string_array(&m.data, m.len, 1,
				form_filetype_data_entry(ft.list[i]));
		m.len = add_to_string_array(&m.items, m.len, 1,
				form_filetype_menu_entry(ft.list[i], max_len));
	}

	ft_assoc_records_free(&ft);

#ifdef ENABLE_DESKTOP_FILES
	(void)add_to_string_array(&m.data, m.len, 1,
			form_filetype_data_entry(NONE_PSEUDO_PROG));
	m.len = add_to_string_array(&m.items, m.len, 1, "");
#endif

	for(i = 0; i < magic.count; i++)
	{
		(void)add_to_string_array(&m.data, m.len, 1,
				form_filetype_data_entry(magic.list[i]));
		m.len = add_to_string_array(&m.items, m.len, 1,
				form_filetype_menu_entry(magic.list[i], max_len));
	}

	return display_menu(&m, view);
}

/* Returns pointer to a statically allocated buffer */
static const char *
form_filetype_menu_entry(assoc_record_t prog, int descr_width)
{
	static char result[PATH_MAX];
	if(descr_width > 0)
	{
		char format[16];
		if(prog.description[0] == '\0')
		{
			snprintf(format, sizeof(format), " %%-%ds  %%s", descr_width);
		}
		else
		{
			snprintf(format, sizeof(format), "[%%-%ds] %%s", descr_width);
		}
		snprintf(result, sizeof(result), format, prog.description, prog.command);
	}
	else
	{
		snprintf(result, sizeof(result), "%s", prog.command);
	}
	return result;
}

/* Returns pointer to a statically allocated buffer */
static const char *
form_filetype_data_entry(assoc_record_t prog)
{
	static char result[PATH_MAX];
	snprintf(result, sizeof(result), "%s|%s", prog.description, prog.command);
	return result;
}

/* Callback that is called when menu item is selected.  Should return non-zero
 * to stay in menu mode. */
int
execute_filetype_cb(FileView *view, menu_info *m)
{
	if(view->dir_entry[view->list_pos].type == DIRECTORY && m->pos == 0)
	{
		handle_dir(view);
	}
	else
	{
		const char *prog_str = strchr(m->data[m->pos], '|') + 1;
		if(prog_str[0] != '\0')
		{
			int background = m->extra_data & 1;
			run_using_prog(view, prog_str, 0, background);
		}
	}

	clean_selected_files(view);
	redraw_view(view);
	return 0;
}

int
show_fileprograms_menu(FileView *view, const char fname[])
{
	static menu_info m;

	assoc_records_t file_programs;

	init_menu_info(&m, FILEPROGRAMS_MENU,
			format_str("No programs match %s", fname));

	m.title = format_str(" Programs that match %s ", fname);

	file_programs = ft_get_all_programs(fname);
	fill_menu_from_records(&m, &file_programs);
	ft_assoc_records_free(&file_programs);

	return display_menu(&m, view);
}

int
show_fileviewers_menu(FileView *view, const char fname[])
{
	static menu_info m;

	assoc_records_t file_viewers;

	init_menu_info(&m, FILEVIEWERS_MENU,
			format_str("No viewers match %s", fname));

	m.title = format_str(" Viewers that match %s ", fname);

	file_viewers = ft_get_all_viewers(fname);
	fill_menu_from_records(&m, &file_viewers);
	ft_assoc_records_free(&file_viewers);

	return display_menu(&m, view);
}

/* Fills the menu with commands from association records. */
static void
fill_menu_from_records(menu_info *m, const assoc_records_t *records)
{
	int i;
	const int max_len = max_desc_len(records);

	for(i = 0; i < records->count; ++i)
	{
		m->len = add_to_string_array(&m->items, m->len, 1,
				form_filetype_menu_entry(records->list[i], max_len));
	}
}

/* Calculates the maximum length of the description among the records.  Returns
 * the length. */
static int
max_desc_len(const assoc_records_t *records)
{
	int i;
	int max_len = 0;
	for(i = 0; i < records->count; ++i)
	{
		max_len = MAX(max_len, strlen(records->list[i].description));
	}
	return max_len;
}

/* vim: set tabstop=2 softtabstop=2 shiftwidth=2 noexpandtab cinoptions-=(0 : */
/* vim: set cinoptions+=t0 : */
