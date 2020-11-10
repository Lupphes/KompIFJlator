import os
import subprocess
import random
from decimal import Decimal

_FILENAME = "tests/test.txt"
_TESTEDFILE1 = "build/scanner-test.out"
_TESTEDFILE2 = "build/scanner-test2.out"
_TESTEDFILE3 = "build/scanner-test3.out"
_ERROR = False


falseChars = [
    ['`', "TokenUndefined"],
    ['%', "TokenUndefined"],
    ['@', "TokenUndefined"],
]

blankChars = [
    [' ', "TokenEOF"],
    ['\t', "TokenEOF"]
]

commentChars = [
    ['//', "TokenEOF"],
    ['/*', "TokenEOF"]
]

basicChars = [
    [',', "TokenComma"],
    [';', "TokenSemicolon"],
    ['(', "TokenLeftBracket"],
    [')', "TokenRightBracket"],
    ['{', "TokenLeftCurlyBracket"],
    ['}', "TokenRightCurlyBracket"],
    ['/', "TokenDivide"],
    ['+', "TokenAdd"],
    ['*', "TokenMultiply"],
    ['-', "TokenSubtract"],
    ['=', "TokenEquals"]
]

nullChars = [
    ['\n', "TokenEOF"],
    ['', "TokenEOF"]
]

doubleChars = [
    ['!=', "TokenNotEqual"],  
    ['==', "TokenIsEqual"],
    ['<=', "TokenIsGreaterEqual"],
    ['>=', "TokenIsLessEqual"],
    [':=', "TokenVarDefine"]
]

identifiers = [
    ['a', "TokenIdentifier"],
    ['Z', "TokenIdentifier"],
    ['_', "TokenIdentifier"],
    ['_A', "TokenIdentifier"],
    ['AZ', "TokenIdentifier"],
    ['A4', "TokenIdentifier"],
    ['az', "TokenIdentifier"],
    ['_5', "TokenIdentifier"],
    ['a45dfwe45fe', "TokenIdentifier"],
    ['_a45dfwe45fe', "TokenIdentifier"],
    ['oprtijdfn', "TokenIdentifier"],
    ['_4545', "TokenIdentifier"]
]

commands = [
    ['else', "TokenElse"],
    ['float64', "TokenDataType"],
    ['for', "TokenFor"],
    ['func', "TokenFunc"],
    ['if', "TokenIf"],
    ['int', "TokenDataType"],  
    ['package', "TokenPackage"],
    ['return', "TokenReturn"],
    ['string', "TokenDataType"]
]

literals = [
    ['""', "TokenStringLiteral"],
    ['"Henlo"', "TokenStringLiteral"],
    ['"Im here"', "TokenStringLiteral"],
    ['"Hello World1234567892345678901"', "TokenStringLiteral"],
    ['"[];,./;`-*/+    ><?!@#$%^&*()__+="', "TokenStringLiteral"]
]

errorLiterals = [
    ['"', "TokenUndefined"],
    ['"Henlo', "TokenUndefined"],
    ['"Im here', "TokenUndefined"],
    ['"Hello World1234567892345678901', "TokenUndefined"],
    ['"[];,./;`-*/+    ><?!@#$%^&*()__+=', "TokenUndefined"]
]

numbers = [
    ['0', "TokenWholeNbr"],
    ['1', "TokenWholeNbr"],
    ['15', "TokenWholeNbr"],
    ['123456789', "TokenWholeNbr"],
    ['0.0', "TokenDecimalNbr"],
    ['123.0', "TokenDecimalNbr"],
    ['123456789.123456789', "TokenDecimalNbr"],
    ['0.123456789', "TokenDecimalNbr"],
    ['0.123456789', "TokenDecimalNbr"],
    ['5E4', "TokenDecimalNbr"],
    ['5E+4', "TokenDecimalNbr"],
    ['5E-4', "TokenDecimalNbr"],
    ['0.5E4', "TokenDecimalNbr"],
    ['0.5E-4', "TokenDecimalNbr"],
    ['0.5E+4', "TokenDecimalNbr"]
]

numbersReturn = [
    ['0', "0"],
    ['1', "1"],
    ['15', "15"],
    ['123456789', "123456789"],
    ['0.0', "0.0"],
    ['123.0', "123.0"],
    ['123456789.123456789', "123456789.123456791"],
    ['0.123456789', "0.123456789"],
    ['5E4', "50000.000000000"],
    ['5E+4', "50000.000000000"],
    ['5E-4', "0.000500000"],
    ['0.5E4', "5000"],
    ['0.5E-4', "0.00005"],
    ['0.5E+4', "5000"]
]

numbersErrors = [
    ['0..45', "TokenUndefined"],
    ['0.4E.5', "TokenUndefined"],
    ['0.4E-.5', "TokenUndefined"],
    ['0.4E+.5', "TokenUndefined"],
    ['45E+.5', "TokenUndefined"],
    ['45..E+..5', "TokenUndefined"],
    ['4.', "TokenUndefined"]
]

literalValues = [
    ['""', ""],
    ['"Henlo"', "Henlo"],
    ['"Im here"', "Im here"],
    ['"Hello World1234567892345678901"', "Hello World1234567892345678901"],
    ['"\\\\"', "\\"],
    ['"\\t"', "\t"],
    ['"\\""', "\""],
    ['"\\x36"', "6"],
    ['"\\x6F"', "o"],
    ['"\\x6f"', "o"],
    ['"\\\\\\\\"', "\\\\"],
    ['"\\t\\t"', "\t\t"],
    ['"\\"\\""', "\"\""],
    ['"\\x36\\x36"', "66"],
    ['"\\x40\\x40"', "@@"],
]

errorLiteralsValues = [
    ['"\\f"', "TokenUndefined"],
    ['"\\45"', "TokenUndefined"],
    ['"\\fefefe"', "TokenUndefined"],
    ['"\\x5"', "TokenUndefined"],
    ['"\\x4\\"', "TokenUndefined"]
]

dataTypeValues = [
    ['float64', "TypeFloat64"],
    ['string', "TypeString"],
    ['int', "TypeInt"]
]

def createWriteFile(i, array):
    f = open(_FILENAME,"w+")
    f.write(array[i][0])
    f.close() 
    return

def simpleTest():
    global _ERROR
    for i in range(len(basicChars)): 
        createWriteFile(i, basicChars)
        f = open(_FILENAME, "r")
        result = subprocess.run([_TESTEDFILE1], stdin=f, capture_output=True, text=True)  
        f.close()
        if result.stdout != basicChars[i][1]:
            _ERROR = True
            print(f"Test {i} didn't pass. Expected {basicChars[i][1]} got {result.stdout} with '{basicChars[i][0]}'")
        if result.returncode != 0:
            _ERROR = True
            print(f"Test {i} returned non-zero code!")
    if not _ERROR:
        print("All 'Simple tests' passed!")
    else:
        print("Tests didn't pass!")
    print("-------------------------------------------------------------------")
    _ERROR = False
    return

def simpleTestInline():
    global _ERROR
    f = open(_FILENAME,"w+")
    for i in range(len(basicChars)): 
        f.write(basicChars[i][0])
    f.close() 
    for j in range(len(basicChars)): 
        f = open(_FILENAME, "r")
        f.seek(j)
        result = subprocess.run([_TESTEDFILE1], stdin=f, capture_output=True, text=True)  
        f.close()
        if result.stdout != basicChars[j][1]:
            _ERROR = True
            print(f"Test {j} didn't pass. Expected {basicChars[j][1]} got {result.stdout} with '{basicChars[j][0]}'")
        if result.returncode != 0:
            _ERROR = True
            print(f"Test {i} returned non-zero code!")
    if not _ERROR:
        print("All 'SimpleInLine tests' passed!")
    else:
        print("Tests didn't pass!")
    print("-------------------------------------------------------------------")
    _ERROR = False
    return

def simpleTestNullChars():
    global _ERROR
    for i in range(len(nullChars)): 
        createWriteFile(i, nullChars)
        f = open(_FILENAME, "r")
        result = subprocess.run([_TESTEDFILE1], stdin=f, capture_output=True, text=True)  
        f.close()
        if result.stdout != nullChars[i][1]:
            _ERROR = True
            print(f"Test {i} didn't pass. Expected {nullChars[i][1]} got {result.stdout} with '{nullChars[i][0]}'")
        if result.returncode != 0:
            _ERROR = True
            print(f"Test {i} returned non-zero code!")
    if not _ERROR:
        print("All 'Simple Null-chars tests' passed!")
    else:
        print("Tests didn't pass!")
    print("-------------------------------------------------------------------")
    _ERROR = False
    return

def errorChecks():
    global _ERROR
    for i in range(len(falseChars)): 
        createWriteFile(i, falseChars)
        f = open(_FILENAME, "r")
        result = subprocess.run([_TESTEDFILE1], stdin=f, capture_output=True, text=True)  
        f.close()
        if result.stdout != falseChars[i][1]:
                _ERROR = True
                print(f"Test {i} didn't pass. Expected {falseChars[i][1]} got {result.stdout} with '{falseChars[i][0]}'")
    if result.returncode == 0:
        _ERROR = True
        print(f"Test {i} returned zero code (incorrect)!")
    if not _ERROR:
        print("All 'Simple error tests' passed!")
    else:
        print("Tests didn't pass!")
    print("-------------------------------------------------------------------")
    _ERROR = False
    return

def doubleCharsTest():
    global _ERROR
    for i in range(len(doubleChars)): 
        createWriteFile(i, doubleChars)
        f = open(_FILENAME, "r")
        result = subprocess.run([_TESTEDFILE1], stdin=f, capture_output=True, text=True)  
        f.close()
        if result.stdout != doubleChars[i][1]:
                _ERROR = True
                print(f"Test {i} didn't pass. Expected {doubleChars[i][1]} got {result.stdout} with '{doubleChars[i][0]}'")
    if result.returncode != 0:
        _ERROR = True
        print(f"Test {i} returned non-zero code!")
    if not _ERROR:
        print("All 'Double chars tests' passed!")
    else:
        print("Tests didn't pass!")
    print("-------------------------------------------------------------------")
    _ERROR = False
    return


def skippingTest():
    global _ERROR
    for i in range(len(blankChars)): 
        j = 0
        f = open(_FILENAME,"w+")
        randomNbr = random.randint(1,25)
        while j != randomNbr:
            f.write(blankChars[i][0])
            j += 1
        f.close() 
        f = open(_FILENAME, "r")
        result = subprocess.run([_TESTEDFILE1], stdin=f, capture_output=True, text=True)  
        f.close()
    if result.stdout != blankChars[i][1]:
            _ERROR = True
            print(f"Test {i} didn't pass. Expected {blankChars[i][1]} got {result.stdout} with '{blankChars[i][0]}'")
    if result.returncode != 0:
        _ERROR = True
        print(f"Test {i} returned non-zero code!")
    if not _ERROR:
        print("All 'Skipping chars tests' passed!")
    else:
        print("Tests didn't pass!")
    print("-------------------------------------------------------------------")
    _ERROR = False
    return


def commentLineTest():
    global _ERROR
    f = open(_FILENAME,"w+")
    f.write(commentChars[0][0])
    f.write(commentChars[0][0])
    randomNbr = random.randint(1,25)
    j = 0
    while j != randomNbr:
        f.write(chr(random.randint(32,127)))
        j += 1
    f.close() 
    f = open(_FILENAME, "r")
    result = subprocess.run([_TESTEDFILE1], stdin=f, capture_output=True, text=True)  
    f.close()
    if result.stdout != commentChars[0][1]:
            _ERROR = True
            print(f"Test {0} didn't pass. Expected {commentChars[0][1]} got {result.stdout} with '{commentChars[0][0]}'")
    if result.returncode != 0:
        _ERROR = True
        print(f"Test {0} returned non-zero code!")
    if not _ERROR:
        print("'CommentLine EOF test' passed!")
    else:
        print("Tests didn't pass!")
    print("-------------------------------------------------------------------")
    f = open(_FILENAME,"a")
    f.write("\n")
    f.close() 
    f = open(_FILENAME, "r")
    result = subprocess.run([_TESTEDFILE1], stdin=f, capture_output=True, text=True)  
    f.close()
    if result.stdout != "TokenEOF":
            _ERROR = True
            print(f"Test {0} didn't pass. Expected TokenEOF got {result.stdout} with '{commentChars[0][0]}'")
    if result.returncode != 0:
        _ERROR = True
        print(f"Test {0} returned non-zero code!")
    if not _ERROR:
        print("'CommentLine EOL test' passed!")
    else:
        print("Tests didn't pass!")
    print("-------------------------------------------------------------------")
    f = open(_FILENAME,"w+")
    _ERROR = False


def commentsBlockTest():
    global _ERROR
    f = open(_FILENAME,"w+")
    f.write("/*/*")
    randomNbr = random.randint(1,25)
    j = 0
    while j != randomNbr:
        f.write(chr(random.randint(32,127)))
        f.write("\n")
        j += 1
    f.close()
    f = open(_FILENAME, "r")
    result = subprocess.run([_TESTEDFILE1], stdin=f, capture_output=True, text=True)  
    f.close()
    if result.returncode == 0:
        _ERROR = True
        print(f"Test returned zero code (incorrect)!")
    if not _ERROR:
        print("'CommentBlock Error test' passed!")
    else:
        print("Tests didn't pass!")
    print("-------------------------------------------------------------------")
    _ERROR = False
    f = open(_FILENAME,"a")
    f.write("*/")
    f.close()
    f = open(_FILENAME, "r")
    result = subprocess.run([_TESTEDFILE1], stdin=f, capture_output=True, text=True)  
    f.close()
    if result.stdout != "TokenEOF":
            _ERROR = True
            print(f"Test {0} didn't pass. Expected TokenEOF got {result.stdout}'")
    if result.returncode != 0:
        _ERROR = True
        print(f"Test {0} returned non-zero code!")
    if not _ERROR:
        print("'CommentBlock test' passed!")
    else:
        print("Tests didn't pass!")
    print("-------------------------------------------------------------------")
    f = open(_FILENAME,"w+")
    return

def identifierTests():
    global _ERROR
    for i in range(len(identifiers)): 
        createWriteFile(i, identifiers)
        f = open(_FILENAME, "r")
        result = subprocess.run([_TESTEDFILE1], stdin=f, capture_output=True, text=True)  
        f.close()
        if result.stdout != identifiers[i][1]:
                _ERROR = True
                print(f"Test {i} didn't pass. Expected {identifiers[i][1]} got {result.stdout} with '{identifiers[i][0]}'")
        if result.returncode != 0:
            _ERROR = True
            print(f"Test {i} returned non-zero code!")
    if not _ERROR:
        print("All 'Identifiers tests' passed!")
    else:
        print("Tests didn't pass!")
    print("-------------------------------------------------------------------")
    _ERROR = False
    return

def commandsTests():
    global _ERROR
    for i in range(len(commands)): 
        createWriteFile(i, commands)
        f = open(_FILENAME, "r")
        result = subprocess.run([_TESTEDFILE1], stdin=f, capture_output=True, text=True)  
        f.close()
        if result.stdout != commands[i][1]:
                _ERROR = True
                print(f"Test {i} didn't pass. Expected {commands[i][1]} got {result.stdout} with '{commands[i][0]}'")
        if result.returncode != 0:
            _ERROR = True
            print(f"Test {i} returned non-zero code!")
    if not _ERROR:
        print("All 'Commands tests' passed!")
    else:
        print("Tests didn't pass!")
    print("-------------------------------------------------------------------")
    _ERROR = False
    return

def literalsTest():
    global _ERROR
    for i in range(len(literals)): 
        createWriteFile(i, literals)
        f = open(_FILENAME, "r")
        result = subprocess.run([_TESTEDFILE1], stdin=f, capture_output=True, text=True)  
        f.close()
        if result.stdout != literals[i][1]:
                _ERROR = True
                print(f"Test {i} didn't pass. Expected {literals[i][1]} got {result.stdout} with '{literals[i][0]}'")
        if result.returncode != 0:
            _ERROR = True
            print(f"Test {i} returned non-zero code!")
    if not _ERROR:
        print("All 'Literal tests' passed!")
    else:
        print("Tests didn't pass!")
    print("-------------------------------------------------------------------")
    _ERROR = False
    return


def literalsErrorTest():
    global _ERROR
    for i in range(len(errorLiterals)): 
        createWriteFile(i, errorLiterals)
        f = open(_FILENAME, "r")
        result = subprocess.run([_TESTEDFILE1], stdin=f, capture_output=True, text=True)  
        f.close()
        if result.stdout != errorLiterals[i][1]:
                _ERROR = True
                print(f"Test {i} didn't pass. Expected {errorLiterals[i][1]} got {result.stdout} with '{errorLiterals[i][0]}'")
        if result.returncode == 0:
            _ERROR = True
            print(f"Test returned zero code (incorrect)!")
    if not _ERROR:
        print("All 'Literal error tests' passed!")
    else:
        print("Tests didn't pass!")
    print("-------------------------------------------------------------------")
    _ERROR = False
    return


def numberTests():
    global _ERROR
    for i in range(len(numbers)): 
        createWriteFile(i, numbers)
        f = open(_FILENAME, "r")
        result = subprocess.run([_TESTEDFILE1], stdin=f, capture_output=True, text=True)  
        f.close()
        if result.stdout != numbers[i][1]:
                _ERROR = True
                print(f"Test {i} didn't pass. Expected {numbers[i][1]} got {result.stdout} with '{numbers[i][0]}'")
        if result.returncode != 0:
            _ERROR = True
            print(f"Test {i} returned non-zero code!")
    if not _ERROR:
        print("All 'Number tests' passed!")
    else:
        print("Tests didn't pass!")
    print("-------------------------------------------------------------------")
    _ERROR = False
    return

def numberErrorTests():
    global _ERROR
    for i in range(len(numbersErrors)): 
        createWriteFile(i, numbersErrors)
        f = open(_FILENAME, "r")
        result = subprocess.run([_TESTEDFILE1], stdin=f, capture_output=True, text=True)  
        f.close()
        if result.stdout != numbersErrors[i][1]:
                _ERROR = True
                print(f"Test {i} didn't pass. Expected {numbersErrors[i][1]} got {result.stdout} with '{numbersErrors[i][0]}'")
        if result.returncode == 0:
            _ERROR = True
            print(f"Test returned zero code (incorrect)!")
    if not _ERROR:
        print("All 'Number error tests' passed!")
    else:
        print("Tests didn't pass!")
    print("-------------------------------------------------------------------")
    _ERROR = False
    return

def numberReturnTests():
    global _ERROR
    for i in range(len(numbersReturn)): 
        createWriteFile(i, numbersReturn)
        f = open(_FILENAME, "r")
        result = subprocess.run([_TESTEDFILE3], stdin=f, capture_output=True, text=True)  
        f.close()
        if Decimal(result.stdout) != Decimal(numbersReturn[i][1]):
                _ERROR = True
                print(f"Test {i} didn't pass. Expected value {Decimal(numbersReturn[i][1])} got {Decimal(result.stdout)} with inserted value '{Decimal(numbersReturn[i][0])}'")
        if result.returncode != 0:
            _ERROR = True
            print(f"Test {i} returned non-zero code!")
    if not _ERROR:
        print("All 'Number Return tests' passed!")
    else:
        print("Tests didn't pass!")
    print("-------------------------------------------------------------------")
    _ERROR = False
    return

def stringLiteralTests():
    global _ERROR
    for i in range(len(literalValues)): 
        createWriteFile(i, literalValues)
        f = open(_FILENAME, "r")
        result = subprocess.run([_TESTEDFILE3], stdin=f, capture_output=True, text=True)  
        f.close()
        if result.stdout != literalValues[i][1]:
                _ERROR = True
                print(f"Test {i} didn't pass. Expected value {literalValues[i][1]} got {result.stdout} with inserted value '{literalValues[i][0]}'")
        if result.returncode != 0:
            _ERROR = True
            print(f"Test {i} returned non-zero code!")
    if not _ERROR:
        print("All 'StringLiteral Return tests' passed!")
    else:
        print("Tests didn't pass!")
    print("-------------------------------------------------------------------")
    _ERROR = False
    return


def literalsValueErrorTest():
    global _ERROR
    for i in range(len(errorLiteralsValues)): 
        createWriteFile(i, errorLiteralsValues)
        f = open(_FILENAME, "r")
        result = subprocess.run([_TESTEDFILE3], stdin=f, capture_output=True, text=True)  
        f.close()
        if result.stdout != errorLiteralsValues[i][1]:
                _ERROR = True
                print(f"Test {i} didn't pass. Expected {errorLiteralsValues[i][1]} got {result.stdout} with '{errorLiteralsValues[i][0]}'")
        if result.returncode == 0:
            _ERROR = True
            print(f"Test returned zero code (incorrect)!")
    if not _ERROR:
        print("All 'Literal error tests' passed!")
    else:
        print("Tests didn't pass!")
    print("-------------------------------------------------------------------")
    _ERROR = False
    return

def identifierValueTests():
    global _ERROR
    for i in range(len(identifiers)): 
        createWriteFile(i, identifiers)
        f = open(_FILENAME, "r")
        result = subprocess.run([_TESTEDFILE3], stdin=f, capture_output=True, text=True)  
        f.close()
        if result.stdout != identifiers[i][0]:
                _ERROR = True
                print(f"Test {i} didn't pass. Expected value {identifiers[i][0]} got {result.stdout} with inserted value '{identifiers[i][0]}'")
        if result.returncode != 0:
            _ERROR = True
            print(f"Test {i} returned non-zero code!")
    if not _ERROR:
        print("All 'Identifier value tests' passed!")
    else:
        print("Tests didn't pass!")
    print("-------------------------------------------------------------------")
    _ERROR = False
    return


def dataTypeValueTests():
    global _ERROR
    for i in range(len(dataTypeValues)): 
        createWriteFile(i, dataTypeValues)
        f = open(_FILENAME, "r")
        result = subprocess.run([_TESTEDFILE3], stdin=f, capture_output=True, text=True)  
        f.close()
        if result.stdout != dataTypeValues[i][1]:
                _ERROR = True
                print(f"Test {i} didn't pass. Expected value {dataTypeValues[i][1]} got {result.stdout} with inserted value '{dataTypeValues[i][0]}'")
        if result.returncode != 0:
            _ERROR = True
            print(f"Test {i} returned non-zero code!")
    if not _ERROR:
        print("All 'Data Type value tests' passed!")
    else:
        print("Tests didn't pass!")
    print("-------------------------------------------------------------------")
    _ERROR = False
    return

def makefileRun():
    clean = subprocess.Popen("make clean -C ./src/", shell = True, stderr=subprocess.STDOUT)
    clean.wait()
    result = subprocess.Popen("make -C ./src/", shell = True, stderr=subprocess.STDOUT)
    result.wait()
    return

def main():
    makefileRun()
    simpleTest()
    simpleTestInline()
    simpleTestNullChars()
    errorChecks()
    doubleCharsTest()
    skippingTest()
    commentLineTest()
    commentsBlockTest()
    identifierTests()
    commandsTests()
    literalsTest()
    literalsErrorTest()
    numberTests()
    numberErrorTests()
    numberReturnTests()
    stringLiteralTests()
    literalsValueErrorTest()
    identifierValueTests()
    dataTypeValueTests()
    return

main()
