#ifndef LS_H
# define LS_H

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

// # define LS_D	32
// # define LS_G	64
// # define LS_ONE 128
// # define LS_SS	256
// # define LS_S	512

#endif
