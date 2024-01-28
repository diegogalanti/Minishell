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

### FAILED TESTS

#### 1: Input redirection seems not to work properly:

QUESTION: Why is "<" not read as redirection? 

    Minishell>$ echo < mylist.txt 
    < mylist.txt

-> bash: empty line

    Minishell>$ < mylist.txt grep t
    <: command not found

-> bash: outputs "tatiana"

#### 2: Same for heredoc redirection (<<) when piped:

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

#### 3: Empty quotes (single and double)

    Minishell>$ ''
    Command input = ['']
    Command has 1 arguments.
    Argument = []

Minishell prints an empty line. Whereas bash returns:

    Command '' not found

Same for double quotes. Can you easily change in the parser that empty quotes are not interpreted but saved in command->argv? 

#### (SOLVED) 4: Quotes when using export

    export PATH='/usr/bin'

Minishell prints an error because of '/' which is not allowed in shell and environment variables. However, characters in quotes are ignored. I tried to implement it, but the quotes are omitted in the parser. Is there a way to leave the quotes in the command if it is export builtin?

#### 5: error message and exit status

    bash >$ echo hi < nofile | echo hii
    hii
    bash: nofile: No such file or directory
    bash>$ echo $?
    0

    Minishell>$ echo hi < nofile | echo hii
    hii
    Minishell>$ echo $?
    1

I guess the problem lies in the input redirection which is not working. c->stdin is empty. Try again when input redirections in parser are fixed. 
Same for:

    echo hi < nofile | nonexistcmd



### Tests that have not the exact same output but that I (Tatiana) think OK:

#### (SOLVED) 1
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
#### 5

    Minishell>$ xxxx | echo hola
    xxxx: command not found
    hola
    Minishell>$ echo $?
    0

#### Input redirection seems not to work properly (FIXED):
    bash$ asdf | echo hola
    hola
    xxxx: command not found
    bash$ echo $?
    0

prints in different order. I would argue that it is ok, because it depends on which child process is faster or prioritized. I think to remember from pipex that bash also prints these messages in different order but when I tested this time, it was always the same order. So, I included it into this list, so you know and can form an opinion about it yourself...

#### 6: exit builtin

        bash>$ exit --
        exit
   >$ echo $?
   0

        Minishell>$ exit --
        exit
        minishell: exit: --: numeric argument required
    >$ echo $?
    2


(Both exit the shell.) What I do not understand is that in every case ('-', '---', '+', '++', '+++') bash behaves the same as minishell in this example. And it would only be logical that 'exit --' also exits with 2 and an error message. I don't understand why '--' is different. I am not sure if I should hardcode this special case or if we just leave it as it is. In the tests on github (Chewy Toast) 'exit --' is left out, although 'exit ++' is tested.

Maybe it has to do with the following: "More precisely, a double dash (--) is used in most Bash built-in commands and many other commands to signify the end of command options, after which only positional arguments are accepted." (https://unix.stackexchange.com/questions/11376/what-does-double-dash-mean)

Then I would argue that minishell's behaviour is ok because we have not implemented the '--'.

#### Empty quotes (single and double) (FIXED)
    bash>$ exit 9223372036854775808
    exit
    bash: exit: 9223372036854775808: numeric argument required

From this value on, bash prints the error message. minishell still works and exits normally with exit status codes from 0 - 255. It seems that every thing that exceeds the size of a long, returns error. Should we set this boundary, too? I do not see why...

#### 7: cd //

    Bash>$ cd /
    Bash>$ pwd
    /
    Bash>$ ls
    bin    dev   lib    libx32      mnt   root  snap      sys  var
    boot   etc   lib32  lost+found  opt   run   srv       tmp
    cdrom  home  lib64  media       proc  sbin  swapfile  usr
    Bash>$ cd //
    Bash>$ pwd
    //
    Bash>$ ls
    bin    dev   lib    libx32      mnt   root  snap      sys  var
    boot   etc   lib32  lost+found  opt   run   srv       tmp
    cdrom  home  lib64  media       proc  sbin  swapfile  usr

    Minishell>$ cd /
    Minishell>$ pwd
    /
    Minishell>$ ls
    bin    dev   lib    libx32	mnt   root  snap      sys  var
    boot   etc   lib32  lost+found	opt   run   srv       tmp
    cdrom  home  lib64  media	proc  sbin  swapfile  usr
    Minishell>$ cd //
    Minishell>$ pwd
    /
    Minishell>$ ls
    bin    dev   lib    libx32	mnt   root  snap      sys  var
    boot   etc   lib32  lost+found	opt   run   srv       tmp
    cdrom  home  lib64  media	proc  sbin  swapfile  usr

DIEGO: It was working, not sure how i brake it. For me it was easier to set command->cmd to a new value "NOT_FOUND", it it ok?

#### # sign (FIXED)
I do not understand the command "cd //" in bash. It seems to be exactly the same directory as "/" but when pwd is called it returns "//" instead of "/". In minishell "/" & "//" are executed by chdir. So, I could code some special case for "//" but I do not see the point of doing it. What do you think?
Shortly googled "/" and "//" but did not find any entries... By the way, "cd ///" becomes "/"

#### 8: error message

    Minishell>$ /Users/nonexist/directory
    minishell: /Users/nonexist/directory: command not found
    Minishell>$ echo $?
    127

    bash>$ /Users/nonexist/directory
    bash: /Users/nonexist/directory: No such file or directory
    bash>$ echo $?
    127

Bash and Minishell print a different error message. Minishell treats this argument as a command and gives therefore 'command not found'. If bash is given only the name of a directory, it also prints 'command not found'. The 'no such file or directory' message seems to be dependent on the slash '/':

    bash>$ includes
    includes: command not found
    bash>$ echo $?
    127
    bash>$ ~/42/minishell/includes/
    bash: /home/vm/42/minishell/includes/: Is a directory
    bash>$ echo $?
    126

I think it is over the top to implement a search if the directory exists or not. It seems to be a lot of work for a different error message. What do you think?

#### 9: shell levels

I implemented that the shell-level will increase each time minishell is called in minishell. If minishell is started for the first time, it inherits a higher shell level automatically. Thus, the shell name will also be changed from e.g. 'bin/bash' to 'minishell'. If the inherited shell level is 5000000, an error message is printed and the shell level is set to 1 (see function init_env). However, the following does not work in minishell:

    $ export $SHLVL=5000000
    $ env | grep SHLVL
    bash: warning: shell level (4999999) too high, resetting to 1
    bash: warning: shell level (4999999) too high, resetting to 1
    SHLVL=1

I do not really understand when this happens. Not when you set it, not when just 'env' is called. Only when you grep SHLVL. So, I did not implement it so far. And in my opinion it is sufficient to leave it as it is. What do you think?


DIEGO: Bash interprets # as a commented line and ignores it, I am not sure we need to implement it. But as it is fairly easy I did it.

### More Tests

on https://github.com/ChewyToast/mpanic/tree/main/test/mandatory

-> did almost all commands in echo.txt
-> passed all in pipes.txt
-> passed all in env.txt (except for ";", see open questions 1)
-> passed all in export.txt
-> passed all in exit.txt (except for ";", see open questions 1 & see Tests OK but not same #6)
-> passed all in dir.txt (except for "cd //", see Tests OK but not same #7)
-> passed all in status.txt (except for: see Tests OK but not same #8, Failed Tests #5)
-> passed all in shlvl.txt (not sure if I really understood the tests but I implemented that the shell-level will increase and the shell name is set to "minishell" and a shell level higher than 5000000 will be set to 1, see function init_env)

## Open Questions

#### 1. echo something ; something_else

In bash ";" is interpreted as separator, thus "something_else" is interpreted as command. We do not have to implement that, right?

DIEGO: Right.
But it is in the mandatory tests part in https://github.com/ChewyToast/mpanic/blob/main/test/mandatory/env/env.txt:

    export TESTVAR="value";env | grep TESTVAR

The above command fails.

#### 2. export

I did not implement that export sorts values alphabetically. It does not seem to be required in the subject or the evaluation sheet. I would argue to an evaluator that it is not needed, we all are able to do it and it is just unnecessary work. What do you think?

#### 3. not closed quotes

If the user does not close the quotes, the prompt is just given back. Maybe it would be useful to print an error message, saying that the quotes are not closed?   


### Passed Tests (which means minishell behaves as bash regarding output and exit status):

#### PIPES

    commands                                          

    ls | grep file                                        

    cat | cat | ls

    cat -e /etc/shells | cat -e | cat -e | head -c 10

    echo hola | cat -e | cat -n   

    echo hola | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e

    ls | ls | ls | ... etc ... (257 pipes)

#### OUTPUT REDIRECTIONS

    ls > test

    ls > test1 > test2

#### OUTPUT REDIRECTIONS TESTED FOR ALL BUILTINS 

    echo "sample text" > abc

    echo "sample text" > xyz > abc

    echo "sample text" >> xyz >> abc

    cd > abc

    cd > xyz > abc

    cd >> abc

    cd >> xyz >> abc

same for: pwd, export, unset, env, exit (exit status always: 0)

    echo < test (note: echo cannot redirect input)

#### INPUT REDIRECTIONS WITH HEREDOC

    command                       exit status

    cat << limiter                  0


#### ERROR INPUT

    sl | grep t                      1

    ls | grp                        127

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

#### env

    Minishell>$ export TESTVAR
    Minishell>$ env | grep TESTVAR
    Minishell>$ echo $?
    1

    Minishell>$ export TESTVAR=
    Minishell>$ env | grep TESTVAR
    TESTVAR=

    Minishell>$ export TESTVAR=value
    Minishell>$ env | grep TESTVAR
    TESTVAR=value

#### export

    Minishell>$ export 1A=value
    minishell: export: '1A=value': not a valid identifier

    Minishell>$ export /A=value
    minishell: export: '/A=value': not a valid identifier
    Minishell>$ echo $?
    1

    Minishell>$ export '    A=value'
    minishell: export: '    A=value': not a valid identifier    
    Minishell>$ echo $?
    1

    Minishell>$ export TEST=value TEST=value2
    Minishell>$ export | grep TEST
    declare -x TEST="value2"
 
    Minishell>$ export TEST=value TEST+=' add value'
    Minishell>$ export | grep TEST
    declare -x TEST="value add value"

all tests from https://github.com/ChewyToast/mpanic/blob/main/test/mandatory/export/export.txt passed!
! Memory leaks from parse input !

@Diego: Feel free to add more test, especially error input :)

## Valgrind Errors

#### 1

    Minishell>$ export | grep tat
    declare -x tat
    ==69190== 
    ==69190== HEAP SUMMARY:
    ==69190==     in use at exit: 209,273 bytes in 274 blocks
    ==69190==   total heap usage: 568 allocs, 294 frees, 240,595 bytes allocated
    ==69190== 
    ==69190== 32 bytes in 2 blocks are definitely lost in loss record 39 of 97
    ==69190==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
    ==69190==    by 0x10FB32: ft_lstnew (in /home/vm/42/minishell/Minishell/minishell)
    ==69190==    by 0x10CA0B: create_command (command_creator.c:46)
    ==69190==    by 0x10BDF8: check_final_cs (command_splitter.c:70)
    ==69190==    by 0x10BF30: split_commands (command_splitter.c:102)
    ==69190==    by 0x10A014: parse_input (parser.c:24)
    ==69190==    by 0x109C8A: main (main.c:37)
    ==69190== 
    ==69190== LEAK SUMMARY:
    ==69190==    definitely lost: 32 bytes in 2 blocks
    ==69190==    indirectly lost: 0 bytes in 0 blocks
    ==69190==      possibly lost: 0 bytes in 0 blocks
    ==69190==    still reachable: 209,241 bytes in 272 blocks
    ==69190==         suppressed: 0 bytes in 0 blocks
    ==69190== Reachable blocks (those to which a pointer was found) are not shown.
    ==69190== To see them, rerun with: --leak-check=full --show-leak-kinds=all
    ==69190== 
    ==69190== For lists of detected and suppressed errors, rerun with: -s
    ==69190== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)





