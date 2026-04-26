# Divide by 0 crashed NS:
```ns
1 / 0
```
Should give error instead

# No line number on invalid opp
```ns
2 * "d"
```

# Things continue after error
```ns
this_does_not_exist()
Print("Continuing")
```
This should not continue

# Does not handle \" escape character
```ns
Print("It loopin'\"")
```
Be ready for it to currently run indefinetly. Don't brick you PC.

# FMOD returns false.
```ns
Print(5.0 % 2.0) # Should result in 1.0, currently results in false
```
This happens every time LH is a float.
```ns
Print(5 % 2.0) # This currently works
```

# Modulo with one float results in an error
```ns
Print(5.0 % 2)
```
Outputs
Float failed to perform 14 type operation with a value of Int
1.00000
So it works, but raises an error as well. Huh.

# I WANNA TYPE AND I WANNA NOW :`(

# %= is evaluated as +=
```ns
var c = 22.0
c %= 2.0
Print(c)
```
c should be 0.0
You might want to fix the FMOD error first.

# No error message when adding string + int\
```ns
"String" + 1
```
Should raise an error

# Make double dipping allowed!!!!

```ns
"test".ToUpper().ToUpper()
```

# It breaky sometime :(

```ns
include "io"
include "os"

var quote = ReadFile("quotation.txt")

var time_file_name = "current_time.txt"
var better_script = "
from datetime import datetime
time_as_string = str(datetime.now())
with open(" + quote + time_file_name + quote + ", " + quote + "w" + quote + ") as f:
\tf.write(time_as_string)"



WriteFile("better_script.py", better_script)
Run("C:\\Users\\JGEle\\AppData\\Local\\Microsoft\\WindowsApps\\python.exe better_script.py")

type DateTime{
    func DateTime(str)
    {
        var date_and_time = str.Split(" ")
        this.date_str = date_and_time[0]
        var d = this.date_str
        this.year = Int(d.Substr(0, 4))
        this.month = Int(d.Substr(5, 2))
        this.day = Int(d.Substr(8, 2))
        this.time_str = date_and_time[1]
        var t = this.time_str
        this.hour = Int(t.Substr(0, 2))
        this.minute = Int(t.Substr(3, 2))
        this.second = Int(t.Substr(6, 2))
        this.milisecond = Float(t.Substr(9, 6)) / 1000000.0
    }
    var date_str = ""
    var year = 0
    var month
    var time_str = ""
    var day
    var hour
    var minute
    var second
    var milisecond
}

var time = DateTime(ReadFile(time_file_name))
Print(time)
```

# Static type functions would be soooo cool
plz
pretty plz

# Read from standard out

s = Run("something")

# Get current working directory

io.WorkingDir()

# Inlcude does not work

See test_include1.ns & test_include2.ns