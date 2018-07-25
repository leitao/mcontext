/* move_mount test.
 *
 * Copyright (C) 2018 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
 */

#define _GNU_SOURCE
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/prctl.h>
#include <sys/wait.h>

#define O_CLONE_MOUNT	040000000	/* Used with O_PATH to clone the mount subtree at path */
#define O_NON_RECURSIVE	0100000000	/* Used with O_CLONE_MOUNT to only clone one mount */

#define MOVE_MOUNT_F_SYMLINKS		0x00000001 /* Follow symlinks on from path */
#define MOVE_MOUNT_F_AUTOMOUNTS		0x00000002 /* Follow automounts on from path */
#define MOVE_MOUNT_F_EMPTY_PATH		0x00000004 /* Empty from path permitted */
#define MOVE_MOUNT_T_SYMLINKS		0x00000010 /* Follow symlinks on to path */
#define MOVE_MOUNT_T_AUTOMOUNTS		0x00000020 /* Follow automounts on to path */
#define MOVE_MOUNT_T_EMPTY_PATH		0x00000040 /* Empty to path permitted */
#define MOVE_MOUNT__MASK		0x00000077

static inline int move_mount(int from_dfd, const char *from_pathname,
			     int to_dfd, const char *to_pathname,
			     unsigned int flags)
{
	return syscall(390, from_dfd, from_pathname, to_dfd, to_pathname, flags);
}

void format(void)
{
	printf("Format: move_mount [-a] [-c] [-r]\n");
	exit(2);
}

int main(int argc, char *argv[])
{
	bool preopen = false;
	int ret, fd;
	int o_flags = O_PATH;

	if (argc < 3)
		format();

	for (; argc > 3; argc--, argv++) {
		if (strcmp(argv[1], "-a") == 0)
			preopen = true;
		else if (strcmp(argv[1], "-c") == 0)
			o_flags |= O_CLONE_MOUNT;
		else if (strcmp(argv[1], "-r") == 0)
			o_flags |= O_NON_RECURSIVE;
		else
			format();
	}

	if (preopen) {
		fd = open(argv[1], o_flags);
		if (fd < 0) {
			fprintf(stderr, "open(%s, O_PATH|...): %m\n", argv[1]);
			exit(1);
		}

		ret = move_mount(fd, "", AT_FDCWD, argv[2],
				 MOVE_MOUNT_F_EMPTY_PATH);
		if (ret != 0) {
			fprintf(stderr, "move_mount([%s],%s) = %d: %m\n",
				argv[1], argv[2], ret);
			exit(1);
		}
	} else {
		ret = move_mount(AT_FDCWD, argv[1], AT_FDCWD, argv[2],
				 MOVE_MOUNT_F_EMPTY_PATH);
		if (ret != 0) {
			fprintf(stderr, "move_mount(%s,%s) = %d: %m\n",
				argv[1], argv[2], ret);
			exit(1);
		}
	}
	
	exit(0);
}

