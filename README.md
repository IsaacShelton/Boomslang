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

###Creating a Method
```python

on greet(String name)
    ("Welcome " + name).output()

on main()
    greet("Isaac")
```

###Specifing Default Arguments
```python

on greet(name = "Anonymous")
    ("Welcome " + name).output()

on main()
    greet()
```

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
    window = new Window
    window.show()
```
###Drawing a rectangle
```python
import window from boomslang

window = unique Window
    on display()
        new Rectangle(100,100).draw(0,0)

on main()
    window.show()
```

###Declaring Variables
```python
on main()
    # All the following are equivalent
    Number a
    Number b = new Number
    Number c = 0
    
    # Types can also be infered
    d = new Number
    e = 0
```

###Declaring Templates
```python

template Dog
    String name
    UInteger id

on Dog.bark()
    "BARK ".print()

on main()
    Dog ruff
    
    ruff.bark()
```

###Declaring Constructors for Templates
```python

template Dog
    String name = "Unnamed"
    UInteger id

on Dog.new(String newName, UInteger newId)
    name = newName
    id = newId

on Dog.bark()
    "BARK ".print()

on Dog.info()
    ("Dog " + name + " has an id of " + id.toString()).output()

on main()
    Dog ruff("Ruff", 1)
    
    ruff.info()
    ruff.bark()
```
##Contacting Us
develop.dockysoft@gmail.com
