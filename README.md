# Welcome to My Tar
*Development still in progress*
***

## Task
Recreate the functionality of the Unix-like tar command. 


## Description
my_tar is a command to manipulate tape archive. The first option to tar is a mode indicator from the following list:

    -c Create a new archive containing the specified items.
    -r Like -c, but new entries are appended to the archive. The -f option is required.
    -t List archive contents to stdout.
    -u Like -r, but new entries are added only if they have a modification date newer than the corresponding entry in the archive. The -f option is required.
    -x Extract to disk from the archive. If a file with the same name appears more than once in the archive, each copy will be extracted, with later copies overwriting (replacing) earlier copies.

In -c, -r, or -u mode, each specified file or directory is added to the archive in the order specified on the command line.  By default, the contents of each directory are also archived.

Unless specifically stated otherwise, options are applicable in all operating modes:

    -f file Read the archive from or write the archive to the specified file. The filename can be standard input or standard output.

my_tar will not handle file inside subdirectory.
## Installation
Run the commands 'make' and 'make clean' respectively to compile the program and create the my_tar executable.

## Usage
```
The following creates a new archive called file.tar that contains two files source.c and
source.h:

> ./my_tar -cf file.tar source.c source.h

To view a detailed table of contents for this archive:

> ./my_tar -tf file.tar
source.c
source.h

```

### The Core Team
* [Quinton MINCY](//github.com/Quinton-Mincy) - Developer


<span><i>Made at <a href='https://qwasar.io'>Qwasar SV -- Software Engineering School
