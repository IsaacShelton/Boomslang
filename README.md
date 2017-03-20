# The Boomslang Programming Language
<br>

## What is boomslang?
The Boomslang Programming Language is an open-source programming language that compiles into static executables for Windows and soon Linux. See <a href="http://dockysoft.com/boomslang">dockysoft.com/boomslang</a> for documentation and tutorials.
<br><br><br>

## Boomslang program examples

### Hello World
```python
def main()
    "Hello World".output()
```

NOTE: Ensure to use the -console compiler flag when compiling console programs such as this one.

### Creating a Method
```python

def greet(String name)
    ("Welcome " + name).output()

def main()
    greet("Isaac")
```

### Taking input
```python
def main()
    String name
    
    "Enter your name:".output()
    name.input()
    ("Welcome " + name).output()
```

### Declaring Variables
```python
def main()
    # All the following are equivalent
    Double a
    Double b = new Double
    Double c = 0
    
    # Types can also be infered
    var d = new Double
    var e = 0
```

### Declaring Classes
```python

class Dog
    String name
    Integer id
    
    def bark()
        "Bark Bark Bark Bark".output()

def main()
    Dog ruff
    ruff.bark()
```

### Declaring Constructors for Classes
```python

class Dog
    String name = "An Unnamed Dog"
    Integer id = -1
    
    def new(String name, Integer id)
        self.name = name
        self.id = id
    
    def bark()
        "Bark Bark Bark Bark".output()
    
    def info()
        ("The dog " + name + " has an id of " + id.toString() + ".").output()

def main()
    Dog ruff("Ruff", 21)
    
    ruff.info()
    ruff.bark()
```

### Working with Pointers
```python

def main()
    String^ message = create String("Hello World")
    message.output()
    
    # Make sure to free dynamically allocated memory
    delete message
```

### Getting a Value from a pointer
```python

def main()
    String name = "Isaac"
    String^ greeting = create String("Welcome ")
    
    (^greeting + name).output()
    
    # Make sure to free dynamically allocated memory
    delete greeting
```

### Using Pointers with Already Existing Values
```python

def main()
    String name = "Isaac"
    String^ pointer = &name
    
    name.output()
    pointer.output()
    
    # This will change the value pointed to by pointer
    ^pointer = "Changed"

    name.output()
    pointer.output()
```

### Class Inheritance
```python

class Animal
    Integer legs
    Integer weight

class Dog Animal
    String name
    
    def new(String name)
        self.name = name
        self.legs = 4
        self.weight = 150
```

### Basic Generic Classes
```python

class Box<Type>
    Type item

def main()
    Box<String> mailbox
    mailbox.item = "Hello World, from Someone"
    mailbox.item.output()
```

### Numeric Literal Suffixes
```python

def main()
    Float pi = 3.14f
    Double speed = 74.6d
    Integer gears = 14i
    UnsignedInteger wheels = 4u
```

### Random Numbers
```python

import boomslang/random

def main()
    # Set random seed based on time
    randomize()
    
    # Get a floating point number (0.0 through 9.0)
    Double amount = random_under(10)
    
    # See the result
    amount.output()
```

### Primitives
```python

def sum(Integer a, Integer b)
    return a + b

def main()
    # Primitives are defined as they would be in C
    uint u = 21
    int i = -132
    float f = 3.14
    double d = 0.49832
    
    # Primitives are auto converted to their Object counterpart
    UnsignedInteger uintObject = u
    Integer integerObject = i
    Float floatObject = f
    Double doubleObject = d
    
    # That includes method calls
    int result = sum(24i, i)
    
    # Note however that primitives can only convert to their own Object type
    uint x = i  # Incompatible Classes 'uint' and 'int'
    Float y = d # Incompatible Classes 'Float' and 'double'
```

### One line conditionals
```python

def main()
    Boolean ok = true
    
    # Examples of one line conditionals
    unless ok, die()
    if !ok, die()
    while ok, ok = false
    until ok, ok = true
    
    # It also works with 'forever'
    forever, break
```

## Contacting Us
develop.dockysoft@gmail.com
