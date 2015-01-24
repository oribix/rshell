# rshell
shell project created for CS100

This shell is currently incomplete.

Currently, the shell is able to execute commands with flags. It also outputs the current user and session when waiting for a prompt. The shell (surprisingly) is able to run itself within itself. Typing "exit" will exit the shell. White space doesnt affect how commands run, however, while testing tabbing, it failed once out of many tests.

The Shell is supposed to be able to allow comments by typing '#' then the comment afterwards. It cannot do this. Instead it will try to execute the comments as a command or flags and will throw an error, but wont crash. The shell cannot handle connectors yet and will try to execute them or pass them as flags.

Although it is not in the exec.script file, the shell does handle empty inputs. When it recieves an empty input, it will simply go to the next line and output another prompt.
