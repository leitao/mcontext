/* fd-based mount test.
 *
 * Copyright (C) 2017 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/prctl.h>
#include <sys/wait.h>
#include <linux/fs.h>
#include <linux/unistd.h>

#define __NR_open_tree         389
#define __NR_move_mount                390
#define __NR_fsopen            391
#define __NR_fsmount           392
#define __NR_fspick            393
#define __NR_fsinfo            394


#define MOVE_MOUNT_F_EMPTY_PATH		0x00000004
#define E(x) do { if ((x) == -1) { perror(#x); exit(1); } } while(0)

static void check_messages(int fd)
{
	char buf[4096];
	int err, n;

	err = errno;

	for (;;) {
		n = read(fd, buf, sizeof(buf));
		if (n < 0)
			break;
		n -= 2;

		switch (buf[0]) {
		case 'e':
			fprintf(stderr, "Error: %*.*s\n", n, n, buf + 2);
			break;
		case 'w':
			fprintf(stderr, "Warning: %*.*s\n", n, n, buf + 2);
			break;
		case 'i':
			fprintf(stderr, "Info: %*.*s\n", n, n, buf + 2);
			break;
		}
	}

	errno = err;
}

static __attribute__((noreturn))
void mount_error(int fd, const char *s)
{
	check_messages(fd);
	fprintf(stderr, "%s: %m\n", s);
	exit(1);
}

static inline int fsopen(const char *fs_name, unsigned int flags)
{
	return syscall(__NR_fsopen, fs_name, flags);
}

static inline int fsmount(int fsfd, unsigned int flags, unsigned int ms_flags)
{
	return syscall(__NR_fsmount, fsfd, flags, ms_flags);
}

static inline int fsconfig(int fsfd, unsigned int cmd,
			   const char *key, const void *val, int aux)
{
	return syscall(__NR_fsconfig, fsfd, cmd, key, val, aux);
}

static inline int move_mount(int from_dfd, const char *from_pathname,
			     int to_dfd, const char *to_pathname,
			     unsigned int flags)
{
	return syscall(__NR_move_mount,
		       from_dfd, from_pathname,
		       to_dfd, to_pathname, flags);
}

#define E_fsconfig(fd, cmd, key, val, aux)				\
	do {								\
		if (fsconfig(fd, cmd, key, val, aux) == -1)		\
			mount_error(fd, key ?: "create");		\
	} while (0)

int main(int argc, char *argv[])
{
	int fsfd, mfd;

	/* Mount a publically available AFS filesystem */
	fsfd = fsopen("afs", 0);
	if (fsfd == -1) {
		perror("fsopen");
		exit(1);
	}

	E_fsconfig(fsfd, fsconfig_set_string, "source", "#grand.central.org:root.cell.", 0);
	E_fsconfig(fsfd, fsconfig_cmd_create, NULL, NULL, 0);

	mfd = fsmount(fsfd, 0, MS_RDONLY);
	if (mfd < 0)
		mount_error(fsfd, "fsmount");
	E(close(fsfd));

	if (move_mount(mfd, "", AT_FDCWD, "/mnt", MOVE_MOUNT_F_EMPTY_PATH) < 0) {
		perror("move_mount");
		exit(1);
	}

	E(close(mfd));
	exit(0);
}