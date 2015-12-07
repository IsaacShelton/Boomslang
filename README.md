#The Boomslang Programming Lanuage

##What is boomslang?
The Boomslang Programming Language is an open-source programming language that compiles into static executables for Windows. We are working on Boomslang for Linux also. See <a href="http://dockysoft.com/boomslang">dockysoft.com</a> for documentation and tutorials.

##Boomslang program examples

###Hello World
<code>"Hello World".output()</code>

NOTE: Ensure to use the -console compiler flag when compiling console programs such as this one.

###Taking input
<code>
userInput = ""<br/>
<br/>
"Please enter something:".output()<br/>
userInput.input()<br/>
("You entered : " + userInput).output()
</code>
