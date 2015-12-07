#The Boomslang Programming Lanuage

##What is boomslang?
The Boomslang Programming Language is an open-source programming language that compiles into static executables for Windows. We are working on Boomslang for Linux also. See <a href="http://dockysoft.com/boomslang">dockysoft.com</a> for documentation and tutorials.

##Boomslang program examples

###Hello World
<code>"Hello World".output()</code>

NOTE: Ensure to use the -console compiler flag when compiling console programs such as this one.

###Taking input
<code>userInput = ""</code><br>
<br>
<code>"Please enter something:".output()</code><br>
<code>userInput.input()</code><br>
<code>("You entered : " + userInput).output()</code><br>

###Displaying the current time
<code>use library.time</code><br>
<br>
<code>time = new Time</code><br>
<code>time.now()</code><br>
<br>
<code>time.output()</code>

###Creating a basic window
<code>use library.interface</code><br>
<br>
<code>window = new Interface</code><br>
<code>window.show()</code><br>

###Drawing a rectangle
<code>use library.interface</code><br>
<br>
<code>window = new Interface</code><br>
<br>
<code>new Overseer overseer</code><br>
<code>    on display()</code><br>
<code>        (new Rectangle(0,0,100,100)).draw()</code><br>
<br>
<code>overseer.bind(window)</code><br>
<code>window.show()</code><br>
