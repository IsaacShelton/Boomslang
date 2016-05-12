#The Boomslang Programming Language
<br>

##What is boomslang?
The Boomslang Programming Language is an open-source programming language that compiles into static executables for Windows and soon Linux. See <a href="http://dockysoft.com/boomslang">dockysoft.com/boomslang</a> for documentation and tutorials.
<br><br><br>

##Boomslang program examples

###Hello World
<code>on main()</code><br/>
<code>&nbsp;&nbsp;&nbsp;&nbsp;"Hello World".output()</code>

NOTE: Ensure to use the -console compiler flag when compiling console programs such as this one.

###Taking input
<code>on main()</code><br>
<code>&nbsp;&nbsp;&nbsp;&nbsp;userInput = ""</code><br><br>
<code>&nbsp;&nbsp;&nbsp;&nbsp;"Please enter something:".output()</code><br>
<code>&nbsp;&nbsp;&nbsp;&nbsp;userInput.input()</code><br>
<code>&nbsp;&nbsp;&nbsp;&nbsp;("You entered : " + userInput).output()</code><br>

###Displaying the current time
<code>import time from boomslang</code><br>
<br>
<code>on main()</code><br>
<code>&nbsp;&nbsp;&nbsp;&nbsp;Time.now().output()</code><br>

###Creating a basic window
<code>import window from boomslang</code><br>
<br>
<code>on main()</code><br>
<code>&nbsp;&nbsp;&nbsp;&nbsp;application = new Window</code><br>
<code>&nbsp;&nbsp;&nbsp;&nbsp;application.show()</code><br>

###Drawing a rectangle
<code>import window from boomslang</code><br>
<br>
<code>unique Window application</code><br>
<code>&nbsp;&nbsp;&nbsp;&nbsp;on display()</code><br>
<code>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;new Rectangle(100,100).draw(0,0)</code><br>
<br>
<code>on main()</code>
<code>&nbsp;&nbsp;&nbsp;&nbsp;application.show()</code><br>

##Contacting Us
develop.dockysoft@gmail.com
