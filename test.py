import ctypes
import os

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

p = utils.parse(b"cat -n test.py")

def compare(p: ParsedInput, expected):
    assert p.command.decode('utf-8') == expected[0], f"Command wrong. expected {expected[0]} got {p.command.decode('utf-8')}"
    assert p.flag.decode('utf-8') == expected[1], f"Flag wrong. expected {expected[1]} got {p.flag.decode('utf-8')}"
    assert p.argument.decode('utf-8') == expected[2], f"Argument wrong. expected {expected[2]} got {p.argument.decode('utf-8')}"
    assert p.redirect == expected[3], f"Redirect wrong. expected {expected[3]} got {p.argument}"
    assert p.second_arg.decode('utf-8') == expected[4], f"Second Argument wrong. expected {expected[4]} got {p.second_arg.decode('utf-8')}"
    assert p.alloc == expected[5], f"Alloc wrong. expected {expected[5]} got {p.alloc}"




compare(p, ["cat", "-n", "test.py", 0, "", 0])
