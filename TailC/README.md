# Tail C like implementation 

Tail allows you to read last 10 lines of any file (by default) - number of lines can be modified with flag '-n'. 

I will do my best to implement '-F' option, as this allows you to keep reading into a file on "real-time" till user manually stops it with CTRL + C.

Memory should be freed when CTRL + C is pressed 

Options
- -n <number> number of lines to print. 
- -F keep looking into the file on real time. 
