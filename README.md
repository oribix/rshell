# rshell
shell project created for CS100

This shell is currently incomplete.

Currently, the shell is able to execute commands with flags. It also outputs the current user and session when waiting for a prompt. The shell (surprisingly) is able to run itself within itself. Typing "exit" will exit the shell. White space doesnt affect how commands run, however, while testing tabbing, it failed once out of many tests.

The Shell can handle comments by typing '#' before your comment. It can also handle multiple shell commands by using the connector ';'. The '|' and '&' connectors are in the works.

Entering an empty prompt will simply output another prompt.

LS has now been added to the shell. LS supports the -a --all -l --long -R and --recursive flags.  

LS can also handle multiple flags such as "-l -l -l -l -a -a -a -all --all --long -R --recursive". It can also handle combined flags such as "-llallaRaaRalRRaRllRl".

LS can also have a path parameter passed in. Passing in a path will print the contents of that path. You can combine a path parameter with flags as well (ex. ls bin/ -alR). This does not limit its ability to take in multiple flags and/or complex flags.

I have also implemented some input and output redirection. Currently, the shell is able to handle '<', '>', and '>>' redirection operators. The code for i/o redirection is still very fragile and is very easy to break. Inputting multple parameters past the i/o operators will probably break the program. Entering a command after the filename portion of the operators will likely run the command. I have plans to add pipe '|' to the shell.
