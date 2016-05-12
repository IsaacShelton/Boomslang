#The Boomslang Programming Language
<br>

##What is boomslang?
The Boomslang Programming Language is an open-source programming language that compiles into static executables for Windows and soon Linux. See <a href="http://dockysoft.com/boomslang">dockysoft.com/boomslang</a> for documentation and tutorials.
<br><br><br>

##Boomslang program examples

###Hello World
```python
on main()
    "Hello World".output()
```

NOTE: Ensure to use the -console compiler flag when compiling console programs such as this one.

###Taking input
```python
on main()
    String name
    
    "Enter your name:".output()
    name.input()
    
    ("Welcome " + name).output()
```

###Displaying the current time
```python
import time from boomslang

on main()
    Time.now().output()
```
###Creating a basic window
```python
import window from boomslang

on main()
    application = new Window
    application.show()
```
###Drawing a rectangle
```python
import window from boomslang

unique Window application
    on display()
        new Rectangle(100,100).draw(0,0)

on main()
    application.show()
```
##Contacting Us
develop.dockysoft@gmail.com
