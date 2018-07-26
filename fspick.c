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

int main(){

	int fd = fspick(0 , "/mnt",
			FSPICK_CLOEXEC | FSPICK_NO_AUTOMOUNT);
	if (fd < 0) {
		perror("fspick: ");
		exit(2);
	}

	write(fd, "o intr", sizeof("o intr"));
	write(fd, "o noac", sizeof("o noac"));
	write(fd, "x reconfigure", sizeof("x reconfigure"));

	return 0;
}
