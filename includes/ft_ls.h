/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtan <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/19 11:42:37 by mtan              #+#    #+#             */
/*   Updated: 2018/06/19 11:42:38 by mtan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

# include <stdlib.h>
# include <errno.h>
# include <limits.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/ioctl.h>
# include <dirent.h>
# include <pwd.h>
# include <grp.h>
# include <time.h>
# include "libft.h"

# define LS_L	1
# define LS_RR	2
# define LS_A	4
# define LS_R	8
# define LS_T	16

enum { ERRNO, USAGE, MALLOC };

typedef struct stat	t_stat;

typedef struct		s_file
{
	blkcnt_t		st_blocks;
	mode_t			mode;
	nlink_t			st_nlink;
	uid_t			st_uid;
	gid_t			st_gid;
	off_t			size;
	dev_t			st_rdev;
	time_t			time;
	long			ntime;
	char			*name;
	char			full_path[PATH_MAX];
	struct s_file	*next;
	t_list			list;
}					t_file;

/*
**	t_point struct: for printing in items in matrix
*/

typedef struct		s_point
{
	int				x;
	int				y;
}					t_point;

/*
**	Parsing functions
*/

void				parse_option(char *s, int *flags);
int					parse_options(int ac, char **av, int *flags);

/*
**	List creation functions
*/

t_file				*create_file(char *path, char *name, t_stat *stat);
int					add_new_file(char *path, char *name, t_file **lst);

/*
**	Sorting functions
*/

int					ft_ascii(t_file *a, t_file *b);
int					ft_time(t_file *a, t_file *b);
t_file				*join(t_file *a, t_file *b, int (*f)(t_file*, t_file*));
void				merge_sort(t_file **head_ref, int (*f)(t_file*, t_file*));
void				sort_list(t_file **list, int flags);

/*
**	Printing functions: Main
*/

void				print_list(t_file **list, int flags, int type);
t_file				*read_folder(char path[PATH_MAX], char *name, int flags);
void				print_folders(t_file *list, int flags, int first, int ac);
int					print_files(t_file *list, int flags);
void				print_args(t_file *list, int flags, int count);

/*
**	Printing functions: Matrix
*/

int					ft_max(int a, int b);
void				get_dimensions(t_file *list, t_point *max, t_point *dims);
void				print_dimensions(t_file *list, t_point *max, t_point *dims);
void				print_matrix(t_file *list);

/*
**	Printing functions: Detailed [-l flag]
*/

void				get_block_size(t_file *list, int size[7], int *total);
void				print_chmod(t_file **file);
void				print_links(t_file **file, int size);
void				print_id(t_file **file, int uid, int gid);
void				print_size(t_file **file, int size[7]);
void				print_time(t_file **file);
void				print_name(t_file **file);
void				print_item(t_file **file, int size[7]);
void				print_details(t_file *list, int type);

/*
**	Utility functions
*/

t_file				*initialize_list(int count, char **names);
int					error(char *s, int err);
void				print_path(char *full_path, int count, int *first);
void				reverse_list(t_file **list);
void				free_list(t_file **list);

#endif
