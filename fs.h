#define __NR_open_tree         389
#define __NR_move_mount        390
#define __NR_fsopen            391
#define __NR_fsmount           392
#define __NR_fspick            393
#define __NR_fsinfo            394
#define __NR_fsconfig          395

#define OPEN_TREE_CLONE                1               /* Clone the target tree and at
tach the clone */
#define OPEN_TREE_CLOEXEC      O_CLOEXEC       /* Close the file on execve() */

static inline int open_tree(int dirfd, const char *pathname, unsigned int flags)
{
        return syscall(__NR_open_tree, dirfd, pathname, flags);
}

#define O_CLONE_MOUNT			040000000       /* Used with O_PATH to clone the mount subtree at path */
#define O_NON_RECURSIVE			0100000000      /* Used with O_CLONE_MOUNT to only clone one mount */
#define MOVE_MOUNT_F_SYMLINKS		0x00000001 /* Follow symlinks on from path */
#define MOVE_MOUNT_F_AUTOMOUNTS		0x00000002 /* Follow automounts on from path */
#define MOVE_MOUNT_F_EMPTY_PATH		0x00000004 /* Empty from path permitted */
#define MOVE_MOUNT_T_SYMLINKS		0x00000010 /* Follow symlinks on to path */
#define MOVE_MOUNT_T_AUTOMOUNTS		0x00000020 /* Follow automounts on to path */
#define MOVE_MOUNT_T_EMPTY_PATH		0x00000040 /* Empty to path permitted */
#define MOVE_MOUNT__MASK		0x00000077


#define FSOPEN_CLOEXEC			0x00000001
#define FSMOUNT_CLOEXEC			0x00000001

#define FSPICK_CLOEXEC			0x00000001
#define FSPICK_SYMLINK_NOFOLLOW		0x00000002
#define FSPICK_NO_AUTOMOUNT		0x00000004
#define FSPICK_EMPTY_PATH		0x00000008



enum {
       fsconfig_set_flag,              /* Set parameter, supplying no value */
       fsconfig_set_string,            /* Set parameter, supplying a string value */
       fsconfig_set_binary,            /* Set parameter, supplying a binary blob value
*/
       fsconfig_set_path,              /* Set parameter, supplying an object by path */

       fsconfig_set_path_empty,        /* Set parameter, supplying an object by (empty
 path */
       fsconfig_set_fd,                /* Set parameter, supplying an object by fd */
       fsconfig_cmd_create,            /* Invoke superblock creation */
       fsconfig_cmd_reconfigure,       /* Invoke superblock reconfiguration */
};


#define MOVE_MOUNT_F_EMPTY_PATH                0x00000004
#define E(x) do { if ((x) == -1) { perror(#x); exit(1); } } while(0)

static inline int move_mount(int from_dfd, const char *from_pathname,
                            int to_dfd, const char *to_pathname,
                            unsigned int flags)
{
       return syscall(__NR_move_mount, from_dfd, from_pathname, to_dfd, to_pathname, flags);
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


static inline int fspick(int dirfd, const char *pathname, unsigned int flags)
{
        return syscall(__NR_fspick, dirfd, pathname, flags);
}


