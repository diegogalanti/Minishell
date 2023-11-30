# Minishell

Fix:

input "" should result in 


          "": command not found

Right now, the parser does not set command->argv 

 " " works perfectly.

Fix 2: Exit status

In parser: exit status is saved with quotes: "0" instead of 0.
