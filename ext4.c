/* ext4 mount
*
* Copyright (C) 2018 IBM corp
* Written by Breno Leitao <leitao@debian.org>
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

#include "fs.h"

#define MOUNTPOINT "/dev/vdb2"

int main(){
	int fd, mfd, foo;

	fd = fsopen("ext4", FSOPEN_CLOEXEC);
	if (fd < 0) {
		perror("fsopen");
		exit(1);
	}

        foo = fsconfig(fd, fsconfig_set_string, "source", MOUNTPOINT, 0);
	if (foo != 0)
		perror("fsconfig: ");
	
        foo = fsconfig(fd, fsconfig_cmd_create, NULL, NULL, 0);
	if (foo != 0)
		perror("fsconfig: ");
        mfd = fsmount(fd, FSMOUNT_CLOEXEC, MS_NOEXEC);
	if (mfd < 0)
		perror("fsmount: ");

	foo = move_mount(mfd, "", fd, "/foo", MOVE_MOUNT_F_EMPTY_PATH);
	if (foo != 0)
		perror("move_mount: ");

	return 0;
}
