#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/prctl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/fs.h>
#include <linux/unistd.h>

#include "fs.h"

int main(){
	struct stat buf;
	int fd, ret;

	fd = open_tree(0 , "/mnt",
			OPEN_TREE_CLOEXEC);
	if (fd < 0) {
		perror("fspick: ");
		exit(2);
	}
	ret = fstat(fd, &buf);
	if (ret){
		perror("fstat\n");
		exit(2);
	}

	printf("inode %d\n", buf.st_ino);

	return 0;
}
