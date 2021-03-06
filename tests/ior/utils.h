#ifndef VIFM_TESTS__UTILS_H__
#define VIFM_TESTS__UTILS_H__

void create_non_empty_dir(const char dir[], const char file[]);

void create_empty_nested_dir(const char dir[], const char nested_dir[]);

void create_non_empty_nested_dir(const char root_dir[], const char nested_dir[],
		const char file[]);

void create_empty_dir(const char dir[]);

void create_empty_file(const char file[]);

#endif // VIFM_TESTS__UTILS_H__

/* vim: set tabstop=2 softtabstop=2 shiftwidth=2 noexpandtab cinoptions-=(0 : */
/* vim: set cinoptions+=t0 : */
