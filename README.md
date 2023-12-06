# Minishell

Fix:

input "" should result in 


          "": command not found

Right now, the parser does not set command->argv 

 " " works perfectly.

Fix 2: Exit status

In parser: exit status is saved with quotes: "0" instead of 0.

Fix 3: Multiple output redirections

Right now I am coding in a way that we will only keep the last redirection, however for output redirection it will be missing some behavior. E.g.:

This creates a abc file with "sample text" inside
echo "sample text" > abc

This creates a xyz file with "sample text" inside. However it will also create an empty abc file. The same happens for >>. The diff is only that >> will just create new file, but if the file is already there it will not delete the content, while > will delete the content of all files from teh redirection.
echo "sample text" > abc > xyz

Question: Do we want to replicate that behavior? If so, I think we could create an struct with specific data regarding redirections and in the command struct include a linked list of this struct. Maybe it is an overkill.

Fix 4:

Right now the behavior for open single/double quotes is quite unpredictable. From the subject we dont have to manage it, but to avoid bigger problems maybe I can print a parser error and abort the process.

Fix 5:
Compilation with "make -n" (as required in evaluation sheet) gives the following error: 
	
	cp: cannot stat 'libft.a': No such file or directory
	make: *** [Makefile:46: libft.a] Error 1

Fix 6:
clear does not work (should it?)
