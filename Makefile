all:
	gcc -g -o movemount movemount.c
	gcc -g -o test-fsmount test-fsmount.c
	gcc -g -o ext4 ext4.c
	gcc -g -o fspick fspick.c
	gcc -g -o open_tree open_tree.c

clean:
	rm open_tree fspick ext4 test-fsmount movemount
