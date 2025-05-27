import ctypes
import os
from dataclasses import dataclass

os.system("gcc -shared -fPIC -o utils.so utils.c")

utils = ctypes.CDLL("./utils.so")

class ParsedInput(ctypes.Structure):
    _fields_ = [
        ("command", ctypes.c_char_p),
        ("flag", ctypes.c_char_p),
        ("argument", ctypes.c_char_p),
        ("redirect", ctypes.c_int),
        ("second_arg", ctypes.c_char_p),
        ("alloc", ctypes.c_int),
    ]
utils.parse.restype = ParsedInput

@dataclass
class Expected:
    command: str
    flag: str
    argument: str
    redirect: int
    second_arg: str
    alloc: int

# Function to be called for each test
def compare(testname: str, p: ParsedInput, expected: Expected):
    assert p.command.decode('utf-8') == expected.command, \
        f"Command wrong. expected {expected.command} got {p.command.decode('utf-8')}"
    assert p.flag.decode('utf-8') == expected.flag, \
        f"Flag wrong. expected {expected.flag} got {p.flag.decode('utf-8')}"
    assert p.argument.decode('utf-8') == expected.argument,\
        f"Argument wrong. expected {expected.argument} got {p.argument.decode('utf-8')}"
    assert p.redirect == expected.redirect, \
        f"Redirect wrong. expected {expected.redirect} got {p.argument}"
    assert p.second_arg.decode('utf-8') == expected.second_arg, \
        f"Second Argument wrong. expected {expected.second_arg} got {p.second_arg.decode('utf-8')}"
    assert p.alloc == expected.alloc, \
        f"Alloc wrong. expected {expected.alloc} got {p.alloc}"
    print(f"   Test: {testname} passed")

# Command only
p = utils.parse(b"cat ")
compare("Command only" ,p, Expected("cat", "", "", 0, "", 0))

p = utils.parse(b"cat -n")
compare("Command and flag", p, Expected("cat", "-n", "", 0, "", 0))

p = utils.parse(b"cat test.py")
compare("Command and argument", p, Expected("cat", "", "test.py", 0, "", 0))

p = utils.parse(b"cat -n test.py")
compare("Command, flag, and argument", p, Expected("cat", "-n", "test.py", 0, "", 0))

p = utils.parse(b"cat test.py >>")
compare("Command and redirect", p , Expected("cat", "", "test.py", 1, "", 0))

p = utils.parse(b"cat -n test.py >>")
compare("Command, flag, argument, and redirect", p, Expected("cat", "-n", "test.py", 1, "", 0))

p = utils.parse(b"cat test.py >> new.txt")
compare("Command, argument, redirect, and second_arg", p, Expected("cat", "", "test.py", 1, "new.txt", 0))

p = utils.parse(b"cat -n test.py >> new.txt")
compare("Command, flag, argument, redirect, and second_arg", p, Expected("cat", "-n", "test.py", 1, "new.txt", 0))

p = utils.parse(b"echo \"Hello world\"")
compare("Command and quotes", p, Expected("echo", "", "Hello world", 0, "", 1))

p = utils.parse(b"echo -a \"Hello world\"")
compare("Command, flag, and quotes", p, Expected("echo", "-a", "Hello world", 0, "", 1))

p = utils.parse(b"echo \"Hello world\" >>")
compare("Command, quote, and redirect", p, Expected("echo", "", "Hello world", 1, "", 1))

p = utils.parse(b"echo -n \"Hello world\" >>")
compare("Command, flag, quote, and redirect", p, Expected("echo", "-n", "Hello world", 1, "", 1))

p = utils.parse(b"echo \"Hello world\" >> new.txt")
compare("Command, quote, redirect, and second_arg", p, Expected("echo", "", "Hello world", 1, "new.txt", 1))

p = utils.parse(b"echo -n \"Hello world\" >> new.txt")
compare("Command, flag, quote, redirect, and second_arg", p, Expected("echo", "-n", "Hello world", 1, "new.txt", 1))
