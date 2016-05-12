#The Boomslang Programming Language
<br>

##What is boomslang?
The Boomslang Programming Language is an open-source programming language that compiles into static executables for Windows and soon Linux. See <a href="http://dockysoft.com/boomslang">dockysoft.com/boomslang</a> for documentation and tutorials.
<br><br><br>

##Boomslang program examples

###Hello World
```c++
on main()
    "Hello World".output()
```

NOTE: Ensure to use the -console compiler flag when compiling console programs such as this one.

###Taking input
```c++
on main()
    userInput = ""
    "Please enter something:".output()
    userInput.input()
    ("You entered : " + userInput).output()
```

###Displaying the current time
```c++
import time from boomslang

on main()
    Time.now().output()
```
###Creating a basic window
```c++
import window from boomslang

on main()
    application = new Window
    application.show()
```
###Drawing a rectangle
```c++
import window from boomslang

unique Window application
    on display()
        new Rectangle(100,100).draw(0,0)

on main()
    application.show()
```
##Contacting Us
develop.dockysoft@gmail.com
