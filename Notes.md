# Test Lab - Notes

## Command Line / Environment Variables / User Permissions

1. The logged in users directory
2. Set local variable VV to 3
	- Export local variable as environment variable
	- Enter the bash shell
	- Unest the local variable
	- Exit the bash shell
	- Display the environment variable

3. One can create an alias using echo and redirecting it to the shell configuration file
	- echo alias 'ls="/home/user/ls"' >> .zshrc

4. 3 different streams. Use 2> to redirect the content on stderr. (-p writes the performance report to stdderr)
	- flex -p file.txt 2> resu.txt 

5. chown --reference=result.txt file.txt