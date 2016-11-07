Lab 2
=====

cat / copy
----------

Crate a program that copies

1. data from standard input to standard output
1. input file to an output file

Take care to catch all errors and edge cases.

Try to /somehow/ measure its effectivity. Try different buffer sizes.

Try to run your program on various interesting files: one you don't have
the rights to read/write, special files like `/dev/null` etc.

Try using `mmap` instead of `read` (can you also use it instead of `write`?).
Compare... Will it work with everything that `read`/`write` did?

Try to use `sendfile`.
