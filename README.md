#The Boomslang Programming Language
<br>

##What is boomslang?
The Boomslang Programming Language is an open-source programming language that compiles into static executables for Windows and soon Linux. See <a href="http://dockysoft.com/boomslang">dockysoft.com/boomslang</a> for documentation and tutorials.
<br><br><br>

##Boomslang program examples

###Hello World
```python
def main()
    "Hello World".output()
```

NOTE: Ensure to use the -console compiler flag when compiling console programs such as this one.

###Creating a Method
```python

def greet(String name)
    ("Welcome " + name).output()

def main()
    greet("Isaac")
```

###Specifing Default Arguments
```python

def greet(name = "Anonymous")
    ("Welcome " + name).output()

def main()
    greet()
```

###Taking input
```python
def main()
    String name
    
    "Enter your name:".output()
    name.input()
    
    ("Welcome " + name).output()
```

###Displaying the current time
```python
import boomslang/time

def main()
    Time.now().output()
```
###Creating a basic window
```python
import boomslang/ui

def main()
    Window window
    window.show()
```
###Drawing a rectangle
```python
import boomslang/ui

class AppWindow < Window
    Rectangle rect(100,100)
    
    def display()
        rect.draw(0,0)

def main()
    AppWindow window
    window.show()
```

###Declaring Variables
```python
def main()
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

class Dog
    String name
    Integer id
    
    def bark()
        "BARK ".print()

def main()
    Dog ruff
    ruff.bark()
```

###Declaring Constructors for Templates
```python

class Dog
    String name = "Unnamed"
    Integer id = 0
    
    def new(String name, Integer id)
        self.name = name
        self.id = id
    
    def bark()
        "BARK ".print()
    
    def info()
        ("Dog " + name + " has an id of " + id.toString() + ".").output()

def main()
    Dog ruff("Ruff", 1)
    
    ruff.info()
    ruff.bark()
```

###Working with Pointers
```python

def main()
    String^ message = create String("Hello World")
    message.output()
    
    # Make sure to free dynamically allocated memory
    delete message
```

###Getting a Value from a pointer
```python

def main()
    String name = "Isaac"
    String^ greeting = create String("Welcome ")
    
    (^greeting + name).output()
    
    # Make sure to free dynamically allocated memory
    delete greeting
```

###Using Pointers with Already Existing Values
```python

def main()
    String name = "Isaac"
    String^ pointer = &name
    
    name.output()
    pointer.output()
    
    # This will change the value pointed to by nameAddress
    ^pointer = "Changed"

    name.output()
    pointer.output()
```

##Contacting Us
develop.dockysoft@gmail.com
