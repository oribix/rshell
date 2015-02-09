# rshell
shell project created for CS100

This shell is currently incomplete.

Currently, the shell is able to execute commands with flags. It also outputs the current user and session when waiting for a prompt. The shell (surprisingly) is able to run itself within itself. Typing "exit" will exit the shell. White space doesnt affect how commands run, however, while testing tabbing, it failed once out of many tests.

The Shell can handle comments by typing '#' before your comment. It can also handle multiple shell commands by using the connector ';'. The '|' and '&' connectors are in the works.

Although it is not in the exec.script file, the shell does handle empty inputs. When it recieves an empty input, it will simply go to the next line and output another prompt.

LS has now been added to the shell. LS supports the -a -all -l -long flags. -R is in the works. -l has strange formatting because the columns were not coded to be adaptable.

LS can also handle multiple flags such as "-l -l -l -l -a -a -a -all -all -long". It can also handle combined flags such as "-llallaaaalalll". 
