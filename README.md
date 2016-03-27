#The Boomslang Programming Language
<br>

##What is boomslang?
The Boomslang Programming Language is an open-source programming language that compiles into static executables for Windows and soon Linux. See <a href="http://dockysoft.com/boomslang">dockysoft.com/boomslang</a> for documentation and tutorials.
<br><br><br>

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
<code>use org.boomslang.time</code><br>
<br>
<code>time = new Time</code><br>
<code>time.now()</code><br>
<br>
<code>time.output()</code>

###Creating a basic window
<code>use org.boomslang.window</code><br>
<br>
<code>application = new Window</code><br>
<code>application.show()</code><br>

###Drawing a rectangle
<code>use org.boomslang.window</code><br>
<br>
<code>new Window application</code><br>
<code>&nbsp;&nbsp;&nbsp;&nbsp;on display</code><br>
<code>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;new Rectangle(100,100).draw(Aqua(),0,0)</code><br>
<br>
<code>application.show()</code><br>

##Contacting Us
develop.dockysoft@gmail.com
