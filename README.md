# Unix Command - ls
In the Unix environment, the most commonly used commands are: `cd` and `ls`.

`ls` provides all files within the current directory, with options to view details and recursive traversal of subdirectories. 

This project re-implements the ls command, with options for:
- full details (-l)
- recursive traversal (-R)
- hidden files (-a)
- sorting by time (-t)
- sorting in reverse (-r)

This implementation also supports:
- multiple arguments
- multiple options

## Usage

To compile repository, run `make`.

Then, use command: 
```
./ft_ls [options] [files...]
options: -lRart
```

## How it works
Using system calls, file information is stored within a singly linked list. 

Options are parsed and stored within a single `flags` variable.

Based on options and arguments, appropriate data is printed on console. Sorting uses a recursive insertion sort, and runs O(n) time with O(n^2) worst case. However, average runtime runs closer to optimal as lists are used almost sorted from pre-sorts. 


## Notes
My ft_ls uses the size of current terminal window to determine column width, while system ls uses preset defaults, causing differences in horizontal aligmnent of columns.

Things to improve:
- ft_ls does not handle sticky-bits when showing file permissions
- additional flags like -1 (shows one entry per line)
- colors. We always need colors
