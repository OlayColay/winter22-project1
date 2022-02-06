# CS118 Project 0

This is the repo for winter22 cs118 project 0.

## Academic Integrity Note

You are encouraged to host your code in private repositories on [GitHub](https://github.com/), [GitLab](https://gitlab.com), or other places.  At the same time, you are PROHIBITED to make your code for the class project public during the class or any time after the class.  If you do so, you will be violating academic honestly policy that you have signed, as well as the student code of conduct and be subject to serious sanctions.

## Provided Files

`docker-cs118-run.sh` is file to setup and configure Docker container.
`project` is folder to develop codes for future projects.

## Report
Name: Cole Strain
Student ID: 005376979
Design: A server that allows HTTP GET requests of files stored in this filepath.
Problems: I had a very tough time getting names from files and getting the substring before '.'. I finally managed to
    find an example of readdir() that worked for me.
Libraries: <stdio.h> <stdlib.h> <string.h> <strings.h> <errno.h> <sys/types.h> <sys/socket.h> <sys/wait.h> <netinet/in.h> 
    <fcntl.h> <sys/stat.h> <dirent.h> <time.h> <unistd.h> <string> <iostream>
Acknowledgements: [readdir tutorial](https://c-for-dummies.com/blog/?p=3246), Discussion 1C