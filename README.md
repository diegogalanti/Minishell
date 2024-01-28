# Minishell

### Fix (FIXED):

input "" should result in 


          "": command not found

Right now, the parser does not set command->argv 

 " " works perfectly.

### Fix 2 (FIXED): Exit status

In parser: exit status is saved with quotes: "0" instead of 0.

### Fix 3 (FIXED): Multiple output redirections

Right now I am coding in a way that we will only keep the last redirection, however for output redirection it will be missing some behavior. E.g.:

This creates a abc file with "sample text" inside
echo "sample text" > abc

This creates a xyz file with "sample text" inside. However it will also create an empty abc file. The same happens for >>. The diff is only that >> will just create new file, but if the file is already there it will not delete the content, while > will delete the content of all files from teh redirection.
echo "sample text" > abc > xyz

Question: Do we want to replicate that behavior? If so, I think we could create an struct with specific data regarding redirections and in the command struct include a linked list of this struct. Maybe it is an overkill.

Tatianas Answer: Somehow the updated parser does not delete the additional redirections, in the command->argv: 
ls > abc > xzy
results in command->argv[0] = ls, command->argv[1]= >, command->argv[2] = abc. 
This is quite lucky, because with that I could now implement bashs behaviour (see redirections_multiple.c) by checking for multiple output redirections before executing the command.

### Fix 4 (FIXED):

Right now the behavior for open single/double quotes is quite unpredictable. From the subject we dont have to manage it, but to avoid bigger problems maybe I can print a parser error and abort the process.

In the end I just ignore and it prints nothing.

### Fix 5:
Compilation with "make -n" (as required in evaluation sheet) gives the following error: 
	
	cp: cannot stat 'libft.a': No such file or directory
	make: *** [Makefile:46: libft.a] Error 1

 DIEGO's comment: make -n basically means you show the commands that will be executed but do not execute them, so it will fail as we have to compile and copy the lib to be able to build, only ways to make it work is to have the lib already compiled and present in the folder instead of compiling it during build. I think we can explain to evaluator that when we build minishell, we also re build libft and copy the newly build to our project, so that is why it will fail, is expected. If you build normally with make and them use make -n, it will not fail as the lib is already in the folder.
 TATIANAS COMMENT: Thank you for the explanation! That makes sense. I think you're right and we could argue that. Anyways, we have to look into which files we actually need from libft (if any). If it is just two or three we could just add those functions to our utils files.

### Fix 6:
clear does not work (should it?)

DIEGO's comment: I think to make calling clear work we have to do stuff that is out of scope, however we can hardcode the following ft_printf instead of calling the clear command:
ft_printf("\033[H\033[J");
TATIANA's comment: Okay, thanks but for me there is no need to hardcode it.

## TESTS RUN 

### Passed Tests (which means minishell behaves as bash):

PIPES

    commands                       exit status

    ls | grep file                  0
    
    ls | grp                        127

    cat | cat | ls

OUTPUT REDIRECTIONS

    ls > test

    ls > test1 > test2

OUTPUT REDIRECTIONS TESTED FOR ALL BUILTINS 

    echo "sample text" > abc

    echo "sample text" > xyz > abc

    echo "sample text" >> xyz >> abc

    cd > abc

    cd > xyz > abc

    cd >> abc

    cd >> xyz >> abc

same for: pwd, export, unset, env, exit (exit status always: 0)

    echo < test (note: echo cannot redirect input)

INPUT REDIRECTIONS WITH HEREDOC

    command                       exit status

    cat << limiter                  0


ERROR INPUT

    sl | grep t                      1

    sl | gfi something              127

    sl | gfi something > test.txt   127

    echo -nnnnnnnnnnnn hello        0

    echo anything | exit            0

    echo -n -n hi                   0

    echo "-n -n -n" -n hi           0

    env -i ./minishell


    > testfile

Note: The last test (no command but a redirection) caused a segfault. I fixed it by adding in src/parser/command_creator.c:17:

    if (!command->argv[0])
        return ;

    



@Diego: Feel free to add more test, especially error input :)

### Tests that have not the exact same output but that I (Tatiana) think OK:

#### 1
    test > echo

-> BASH: creates empty file "echo" (exits with 1) | MINISHELL: creates empty file "echo" AND outputs error message: "test: permission denied" (exits with 13)

DIEGO: On mine works fine, I think it may be some permission thing in your machine.

#### 2
    ls | grep l > text.txt

-> BASH: creates file text.txt and lists output from ls and grep in alphabetical order | minishell does the same but not in alphabetical order

DIEGO's: On my pc, even BASH is not sorted.

#### 3

If user has no permissions for text file (chmod 111):

    Minishell>$ ls | grep file > test.txt
    minishell: test.txt: Permission denied

-> same output, different exit status: bash (1), minishell (13) -> does that play a role?

DIEGO: On my pc, minishell returns 1.

#### 4 (please look into this one, Diego)

    command                         exit status
    ls | >testfile                  1

In bash the exit status is 0. Moreover, in minishell "testfile" contains an empty line. I tried to find the error and found out that command->cmd contains "ECHO" before it is assigned. But I could not find out, why. Could you please look into it, Diego?

DIEGO: Probably because ECHO is equivalent to 0, and as there is no command the compiler should assign a initial valuie of 0. I created a "NONE" command, this already fixed the empty line. You just have now to check the exit code to be 0 instead of 1.

## FAILED TESTS

#### Input redirection seems not to work properly (FIXED):

QUESTION: Why is "<" not read as redirection? 

    Minishell>$ echo < mylist.txt 
    < mylist.txt

-> bash: empty line

    Minishell>$ < mylist.txt grep t
    <: command not found

-> bash: outputs "tatiana"

#### Same for heredoc redirection (<<) when piped:

    Minishell>$ cat << limiter | grep hello > test
    Redirection output = [test]
    /usr/bin/cat: '<<': No such file or directory
    /usr/bin/cat: limiter: No such file or directory

    Minishell>$ sleep 3 | << end cat > out1 | echo2 | << end2 cat > out2 | echo 3
    Command input = [sleep 3 ]
    Command has 2 arguments.
    Command input = [ << end cat > out1 ]
    Command has 5 arguments.
    Command input = [ echo2 ]
    Command has 1 arguments.
    Command input = [ << end2 cat > out2 ]
    Command has 5 arguments.
    Command input = [ echo 3]
    Command has 2 arguments.
    Argument = [3]
    <<: command not found
    3   
    echo2: command not found
    <<: command not found

#### Empty quotes (single and double) (FIXED)

    Minishell>$ ''
    Command input = ['']
    Command has 1 arguments.
    Argument = []

Minishell prints an empty line. Whereas bash returns:

    Command '' not found

Same for double quotes. Can you easily change in the parser that empty quotes are not interpreted but saved in command->argv? 

DIEGO: It was working, not sure how i brake it. For me it was easier to set command->cmd to a new value "NOT_FOUND", it it ok?

#### # sign (FIXED)

    #echo echo wtf u execute? > ~/echo 2> /dev/null; chmod 777 ~/echo;export EXPATH="$PATH" PATH="/Users/$USER:$PATH"

should not print anything. Minishell prints: " #echo: command not found"

DIEGO: Bash interprets # as a commented line and ignores it, I am not sure we need to implement it. But as it is fairly easy I did it.

### More Tests

on https://github.com/ChewyToast/mpanic/tree/main/test/mandatory

-> did almost all commands in echo.txt

## Open Questions

#### 1. echo something ; something_else

In bash ";" is interpreted as separator, thus "something_else" is interpreted as command. We do not have to implement that, right?

DIEGO: Right.




