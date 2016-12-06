Homework assignment 02
======================

*This assignment is due Friday Dec 16 23:59:59. See [Submitting](#submitting)
below for instructions*

Asynchronous shell (30pt)
-------------------------

Create a simple "asynchronous shell": it reads commands from stdin and executes
them.  Immediately after starting a process (command) it will return back to
reading another command.  When a process (command) finishes, it will
immediately print it's commandline and return code.

*Note: this means that the output might "intermix" with a commandline
that is just being typed. This is acceptable behaviour, you don't
have to make it "nicer" (i.e. by reprinting the commandline etc.).*

The shell shall display `>` as a prompt when waiting for a command.

The commands are of the form:

    command [arg1 [arg2 ...]]

where command and its arguments are separated by whitespace (space or tab
characters, there is no quoting to include whitespace in arguments) and the
whole line is terminated by a newline character.

You can assume that no command line will be longer then 255 characters (bytes)
and that there will be at most 10 separate arguments (including the command).

For each command (except the special commands below) a message of the form

    = started [N] PID COMMAND

should be show when it is started and another one of the form

    = finished [N] PID COMMAND EXIT_STATUS

should be shown when it finishes. In both messages, N is a sequential
number assigned to the commands, PID is the pid of that process, COMMAND is
the started command (only the first argument / command name) and EXIT_STATUS
is the exit status of the process.

The shell should recognize a couple of special commands:

- `exit`: the shell finishes (it is undefined what should happen with running
  commands),

- `jobs`: lists active (running) commands with their pids. For each currently
  running process print a line of the form

      = running [N] PID COMMAND

- empty command: does nothing, just reprints the prompt.

Note: you don't have to use the low level `read` and `write` calls in this
assignment (i.e. you can use c FILE streams or c++ iostreams), however,
you might have to use them anyway depending on how you handle asynchrony.

The provided `shell.c` contains a simple example that reads a command,
executes it and (synchronously) waits for it to finish before reading the
next one.

### Example

```
> sleep 10
= started [1] 432 sleep
> echo Hi
= started [2] 435 echo
Hi
= finished [2] 435 echo
> jobs
= running [1] 432 sleep
... 10 seconds later ...
= finished [1] 432 sleep
> exit
```

Note: the prompt ('>') before the `jobs` and `exit` commands will be actually
interleaved with the other output, so it will more probably look like:

```
...
= started [2] 435 echo
> Hi
= finished [2] 435 echo
jobs
= running [1] 432 sleep
> = finished [1] 432 sleep
exit
```

Bonus (10pt)
------------

Implement "job control" for your shell. This includes two additional special
commands:

- `stop N`: "pauses" (sends SIGSTOP to) the job number N,
- `cont N`: "resumes" (sends SIGCONT to) the job number N.

When the `stop` command is issued a message of the form

    = stopped [N] PID COMMAND

should be displayed. Similarly when `cont` is issued a message of the form

    = running [N] PID COMMAND

should be displayed. The output of the `jobs` command should be adjusted
accordingly to show the correct state of the job.

Note: you don't have to correctly track stop / continue changes issued from
outside your shell (i.e. you don't have to check the real status of the
process in case someone else sent it a SIGSTOP or SIGCONT).  However doing so
might gain you another five bonus points (please state it clearly in the pull
request, if you do so).

Submitting
----------

Submit your solution by committing  file named `shell.c` (or `shell.cpp`) in
the `as02` folder (of your fork of your private repository) and create a
pull request against the **`as02` branch** of you private repository.
