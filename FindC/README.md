# Locate like C implementation

---
## Quick overview
Unix locate command does not exactly goes through ever folder and subfolder looking for matches. 
Instead it checks a file called _mlocate.db_ which contains references to stored files on your system. 
This data base is usually located at /var/lib/mlocate/ and is updated every 24h by _updatedb_ which can be executed manually as well (sudo updatedb). 
So if you look for a recently created file using _locate_ before _updatedb_ is executed, you will not find your desired result. 

Initially I intended to implement this command, but as no DB will be mantain, I will Implement 'find' command
---
## C implementation 
For this implementation using C, I will use threads, structures and dynamic allocated memory. 
I will not create a database nor check on an existing one, instead I will go through every folder and subfolder looking for matches. 

You can run looking for exact match name.extension or name or .extension 
Examples 
- ./findC -d path/to/dir -f .c
- ./findC -d path/to/dir -f file
- ./findC -d path/to/dir -f file.c

Intentionally I avoided using 'strcmp' 
---
## Goals 
- Current goal is to look for exact matches, whether a dir or a file is located. 
- TODO: Implement regular expressions search to find approximate results and to create a more look alike functionality to Unix locate/find command. 
