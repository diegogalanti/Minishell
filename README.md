# Minishell

Fix:

input "" should result in 


          "": command not found

Right now, the parser does not set command->argv 

 " " works perfectly.

Fix:
open file descriptors when piping

          ls | grep minishell
