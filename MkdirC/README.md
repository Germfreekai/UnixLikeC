# Mkdir C implementation
---

This implementations should be fairly straight forward as C has mkdir command at sys/stat header

Some reference: https://linux.die.net/man/3/mkdir

int mkdir(const char *path, mode_t mode);

Some examples to set permissions

S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH

S_ set
I_ 
R_ read
W_ write
X_ execute
U_ user
G_ group
OTH_ others

so S_IRWXU sets permissions read, write, execute to user
so S_IRWXG sets permissions read, write, execute to group
