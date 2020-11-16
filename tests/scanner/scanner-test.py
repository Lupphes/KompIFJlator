import os
import subprocess
import random
from decimal import Decimal

_REALPATH = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
_FILENAME = _REALPATH + "/test.txt"

_TESTEDFILE1 = _REALPATH + "/../build/scanner-test.out"
_TESTEDFILE2 = _REALPATH + "/../build/scanner-test2.out"
_TESTEDFILE3 = _REALPATH + "/../build/scanner-test3.out"
_MAKEFILE = _REALPATH + "/../src/"

falseChars = [
    ['`', "TokenUndefined"],
    ['%', "TokenUndefined"],
    ['@', "TokenUndefined"]
]

blankChars = [
    [' ', "TokenEOF"],
    ['\t', "TokenEOF"]
]

commentChars = [
    ['//', "TokenEOF"],
    ['/*', "TokenEOL"]
]

basicChars = [
    ['=', "TokenAssignment"],
    [',', "TokenComma"],
    [';', "TokenSemicolon"],
    ['(', "TokenLeftBracket"],
    [')', "TokenRightBracket"],
    ['{', "TokenLeftCurlyBracket"],
    ['}', "TokenRightCurlyBracket"],
    ['/', "TokenDivide"],
    ['+', "TokenAdd"],
    ['*', "TokenMultiply"],
    ['-', "TokenSubtract"]
]

nullChars = [
    ['\n', "TokenEOL"],
    ['', "TokenEOF"]
]

doubleChars = [
    ['!=', "TokenNotEqual"],  
    ['==', "TokenIsEqual"],
    ['>=', "TokenIsGreaterEqual"],
    ['<=', "TokenIsLessEqual"],
    [':=', "TokenVarDefine"],
    ['+=', "TokenAddEqual"],
    ['-=', "TokenSubtractEqual"],
    ['*=', "TokenMultiplyEqual"],
    ['/=', "TokenDivideEqual"]
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
    ['4.', "TokenUndefined"],
    ['00', "TokenUndefined"],
    ['000010', "TokenUndefined"],
    ['010010', "TokenUndefined"]
]

literalValues = [
    ['""', ""],
    ['"Henlo"', "Henlo"],
    ['"Im here"', "Im here"],
    ['"Hello World1234567892345678901"', "Hello World1234567892345678901"],
    ['"\\\\"', "\\"],
    ['"\\t"', "\t"],
    ['"\\""', "\""],
    ['"\\n"', "\n"],
    ['"\\x36"', "6"],
    ['"\\x6F"', "o"],
    ['"\\x6f"', "o"],
    ['"\\\\\\\\"', "\\\\"],
    ['"\\t\\t"', "\t\t"],
    ['"\\n\\n"', "\n\n"],
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
    with open(_FILENAME, 'w+') as file:
        file.write(array[i][0])
    return

def simpleTest():
    print("'Simple' tests!")
    for i in range(len(basicChars)): 
        createWriteFile(i, basicChars)
        with open(_FILENAME, 'r') as file:
            result = subprocess.run([_TESTEDFILE1], stdin=file, capture_output=True, text=True)
        if result.stdout != basicChars[i][1]:
            print(f"Test {i} didn't pass. Expected {basicChars[i][1]} got {result.stdout} with '{basicChars[i][0]}'")
        if result.returncode != 0:
            print(f"Test {i} returned non-zero code!")
    print("-------------------------------------------------------------------")
    return

def simpleTestInline():
    print("'SimpleInLine' tests!")
    with open(_FILENAME, 'w+') as file:
        for i in range(len(basicChars)): 
            file.write(basicChars[i][0])
    with open(_FILENAME, 'r') as file:
        result = subprocess.run([_TESTEDFILE2], stdin=file, capture_output=True, text=True)
    outputlist = result.stdout.split("\n")
    for j in range(len(basicChars)): 
        if outputlist[j] != basicChars[j][1]:
            print(f"Test {j} didn't pass. Expected {basicChars[j][1]} got {outputlist[j]} with '{basicChars[j][0]}'")
        if result.returncode != 0:
            print(f"Test {j} returned non-zero code!")
    print("-------------------------------------------------------------------")
    return

def doubleTestInline():
    print("'DoubleInLine' tests!")
    with open(_FILENAME, 'w+') as file:
        for i in range(len(doubleChars)): 
            file.write(doubleChars[i][0])
    with open(_FILENAME, 'r') as file:
        result = subprocess.run([_TESTEDFILE2], stdin=file, capture_output=True, text=True)
    outputlist = result.stdout.split("\n")
    for j in range(len(doubleChars)): 
        if outputlist[j] != basicChars[j][1]:
            print(f"Test {j} didn't pass. Expected {doubleChars[j][1]} got {outputlist[j]} with '{doubleChars[j][0]}'")
        if result.returncode != 0:
            print(f"Test {j} returned non-zero code!")
    print("-------------------------------------------------------------------")
    return

def simpleTestNullChars():
    print("'Simple Null-chars' tests!")
    for i in range(len(nullChars)): 
        createWriteFile(i, nullChars)
        with open(_FILENAME, 'r') as file:
            result = subprocess.run([_TESTEDFILE1], stdin=file, capture_output=True, text=True)
        if result.stdout != nullChars[i][1]:
            print(f"Test {i} didn't pass. Expected {nullChars[i][1]} got {result.stdout} with '{nullChars[i][0]}'")
        if result.returncode != 0:
            print(f"Test {i} returned non-zero code!")
    print("-------------------------------------------------------------------")
    return

def errorChecks():
    print("'Simple error' tests!")
    for i in range(len(falseChars)): 
        createWriteFile(i, falseChars)
        with open(_FILENAME, 'r') as file:
            result = subprocess.run([_TESTEDFILE1], stdin=file, capture_output=True, text=True)
        if result.stdout != falseChars[i][1]:
            print(f"Test {i} didn't pass. Expected {falseChars[i][1]} got {result.stdout} with '{falseChars[i][0]}'")
        if result.returncode == 0:
            print(f"Test {i} returned zero code (incorrect)!")       
    print("-------------------------------------------------------------------")
    return

def doubleCharsTest():
    print("'Double chars' tests!")
    for i in range(len(doubleChars)): 
        createWriteFile(i, doubleChars)
        with open(_FILENAME, 'r') as file:
            result = subprocess.run([_TESTEDFILE1], stdin=file, capture_output=True, text=True)
        if result.stdout != doubleChars[i][1]:
            print(f"Test {i} didn't pass. Expected {doubleChars[i][1]} got {result.stdout} with '{doubleChars[i][0]}'")
        if result.returncode != 0:
            print(f"Test {i} returned non-zero code!")
    print("-------------------------------------------------------------------")
    return


def skippingTest():
    print("'Skipping chars' tests!")
    for i in range(len(blankChars)): 
        j = 0
        randomNbr = random.randint(1,25)
        with open(_FILENAME, 'w+') as file:
            while j != randomNbr:
                file.write(blankChars[i][0])
                j += 1
        with open(_FILENAME, 'r') as file:
            result = subprocess.run([_TESTEDFILE1], stdin=file, capture_output=True, text=True)
        if result.stdout != blankChars[i][1]:
                print(f"Test {i} didn't pass. Expected {blankChars[i][1]} got {result.stdout} with '{blankChars[i][0]}'")
        if result.returncode != 0:
            print(f"Test {i} returned non-zero code!")
    print("-------------------------------------------------------------------")
    return


def commentLineTest():
    print("'CommentLine EOL & EOF' test!")
    with open(_FILENAME, 'w+') as file:
        file.write(commentChars[0][0])
        file.write(commentChars[0][0])
        randomNbr = random.randint(1,25)
        j = 0
        while j != randomNbr:
            file.write(chr(random.randint(32,127)))
            j += 1
    with open(_FILENAME, 'r') as file:
        result = subprocess.run([_TESTEDFILE1], stdin=file, capture_output=True, text=True)
    if result.stdout != commentChars[0][1]:
        print(f"Test {0} didn't pass. Expected {commentChars[0][1]} got {result.stdout} with '{commentChars[0][0]}'")
    if result.returncode != 0:
        print(f"Test {0} returned non-zero code!")
    with open(_FILENAME, 'a') as file:
        file.write("\n")
    with open(_FILENAME, 'r') as file:
        result = subprocess.run([_TESTEDFILE1], stdin=file, capture_output=True, text=True)  
    if result.stdout != "TokenEOL":
        print(f"Test {0} didn't pass. Expected TokenEOL got {result.stdout} with '{commentChars[0][0]}'")
    if result.returncode != 0:
        print(f"Test {0} returned non-zero code!")
    print("-------------------------------------------------------------------")
    return


def commentsBlockTest(): # maybe EOL?
    print("'CommentBlock Error' test!")
    with open(_FILENAME, 'w+') as file:
        file.write("/*/*")
        randomNbr = random.randint(1,25)
        j = 0
        while j != randomNbr:
            file.write(chr(random.randint(32,127)))
            j += 1
    with open(_FILENAME, 'r') as file:
        result = subprocess.run([_TESTEDFILE1], stdin=file, capture_output=True, text=True)
    if result.returncode != 1:
        print(f"Test did not return 1 as exit code")
    print("-------------------------------------------------------------------")

    print("'CommentBlock Line' test!")
    with open(_FILENAME, 'a') as file:
        file.write("*/")
    with open(_FILENAME, 'r') as file:
        result = subprocess.run([_TESTEDFILE1], stdin=file, capture_output=True, text=True)
    if result.stdout != "TokenEOF":
        print(f"Test {0} didn't pass. Expected TokenEOF got {result.stdout}'")
    if result.returncode != 0:
        print(f"Test {0} returned non-zero code!")
    print("-------------------------------------------------------------------")

    print("'CommentBlock EOL' test!")
    with open(_FILENAME, 'w+') as file:
        file.write("/*/*")
        randomNbr = random.randint(1,25)
        j = 0
        while j != randomNbr:
            file.write(chr(random.randint(32,127)))
            j += 1
            file.write("\n")
        file.write("*/")
    with open(_FILENAME, 'r') as file:
        result = subprocess.run([_TESTEDFILE1], stdin=file, capture_output=True, text=True)
    if result.stdout != "TokenEOL":
        print(f"Test {0} didn't pass. Expected TokenEOL got {result.stdout}'")
    if result.returncode != 0:
        print(f"Test {0} returned non-zero code!")
    print("-------------------------------------------------------------------")
    return

def identifierTests():
    print("'Identifiers' tests!")
    for i in range(len(identifiers)): 
        createWriteFile(i, identifiers)
        with open(_FILENAME, 'r') as file:
            result = subprocess.run([_TESTEDFILE1], stdin=file, capture_output=True, text=True)
        if result.stdout != identifiers[i][1]:
            print(f"Test {i} didn't pass. Expected {identifiers[i][1]} got {result.stdout} with '{identifiers[i][0]}'")
        if result.returncode != 0:
            print(f"Test {i} returned non-zero code!")
    print("-------------------------------------------------------------------")
    return

def commandsTests():
    print("'Commands' tests!")
    for i in range(len(commands)): 
        createWriteFile(i, commands)
        with open(_FILENAME, 'r') as file:
            result = subprocess.run([_TESTEDFILE1], stdin=file, capture_output=True, text=True)
        if result.stdout != commands[i][1]:
            print(f"Test {i} didn't pass. Expected {commands[i][1]} got {result.stdout} with '{commands[i][0]}'")
        if result.returncode != 0:
            print(f"Test {i} returned non-zero code!")
    print("-------------------------------------------------------------------")
    return

def literalsTest():
    print("'Literal' tests!")
    for i in range(len(literals)): 
        createWriteFile(i, literals)
        with open(_FILENAME, 'r') as file:
            result = subprocess.run([_TESTEDFILE1], stdin=file, capture_output=True, text=True)
        if result.stdout != literals[i][1]:
            print(f"Test {i} didn't pass. Expected {literals[i][1]} got {result.stdout} with '{literals[i][0]}'")
        if result.returncode != 0:
            print(f"Test {i} returned non-zero code!")
    print("-------------------------------------------------------------------")
    return


def literalsErrorTest():
    print("'Literal error' tests!")
    for i in range(len(errorLiterals)): 
        createWriteFile(i, errorLiterals)
        with open(_FILENAME, 'r') as file:
            result = subprocess.run([_TESTEDFILE1], stdin=file, capture_output=True, text=True)
        if result.stdout != errorLiterals[i][1]:
            print(f"Test {i} didn't pass. Expected {errorLiterals[i][1]} got {result.stdout} with '{errorLiterals[i][0]}'")
        if result.returncode != 1:
            print(f"Test did not return 1 as exit code")
    print("-------------------------------------------------------------------")
    return


def numberTests():
    print("'Number tests' tests!")
    for i in range(len(numbers)): 
        createWriteFile(i, numbers)
        with open(_FILENAME, 'r') as file:
            result = subprocess.run([_TESTEDFILE1], stdin=file, capture_output=True, text=True)
        if result.stdout != numbers[i][1]:
            print(f"Test {i} didn't pass. Expected {numbers[i][1]} got {result.stdout} with '{numbers[i][0]}'")
        if result.returncode != 0:
            print(f"Test {i} returned non-zero code!")
    print("-------------------------------------------------------------------")
    return

def numberErrorTests():
    print("'Number error' tests!")
    for i in range(len(numbersErrors)): 
        createWriteFile(i, numbersErrors)
        with open(_FILENAME, 'r') as file:
            result = subprocess.run([_TESTEDFILE1], stdin=file, capture_output=True, text=True)
        if result.stdout != numbersErrors[i][1]:
            print(f"Test {i} didn't pass. Expected {numbersErrors[i][1]} got {result.stdout} with '{numbersErrors[i][0]}'")
        if result.returncode != 1:
            print(f"Test did not return 1 as exit code")
    print("-------------------------------------------------------------------")
    return

def numberReturnTests():
    print("'Number Return' tests!")
    for i in range(len(numbersReturn)): 
        createWriteFile(i, numbersReturn)
        with open(_FILENAME, 'r') as file:
            result = subprocess.run([_TESTEDFILE3], stdin=file, capture_output=True, text=True)
        if Decimal(result.stdout) != Decimal(numbersReturn[i][1]):
            print(f"Test {i} didn't pass. Expected value {Decimal(numbersReturn[i][1])} got {Decimal(result.stdout)} with inserted value '{Decimal(numbersReturn[i][0])}'")
        if result.returncode != 0:
            print(f"Test {i} returned non-zero code!")
    print("-------------------------------------------------------------------")
    return

def stringLiteralTests():
    print("'StringLiteral' return tests!")
    for i in range(len(literalValues)): 
        createWriteFile(i, literalValues)
        with open(_FILENAME, 'r') as file:
            result = subprocess.run([_TESTEDFILE3], stdin=file, capture_output=True, text=True)
        if result.stdout != literalValues[i][1]:
            print(f"Test {i} didn't pass. Expected value {literalValues[i][1]} got {result.stdout} with inserted value '{literalValues[i][0]}'")
        if result.returncode != 0:
            print(f"Test {i} returned non-zero code!")
    print("-------------------------------------------------------------------")
    return


def literalsValueErrorTest():
    print("'Literal error' tests!")
    for i in range(len(errorLiteralsValues)): 
        createWriteFile(i, errorLiteralsValues)
        with open(_FILENAME, 'r') as file:
            result = subprocess.run([_TESTEDFILE3], stdin=file, capture_output=True, text=True)
        if result.stdout != errorLiteralsValues[i][1]:
            print(f"Test {i} didn't pass. Expected {errorLiteralsValues[i][1]} got {result.stdout} with '{errorLiteralsValues[i][0]}'")
        if result.returncode == 0:
            print(f"Test returned zero code (incorrect)!")
    print("-------------------------------------------------------------------")
    return

def identifierValueTests():
    print("'Identifier values' tests!")
    for i in range(len(identifiers)): 
        createWriteFile(i, identifiers)
        with open(_FILENAME, 'r') as file:
            result = subprocess.run([_TESTEDFILE3], stdin=file, capture_output=True, text=True)
        if result.stdout != identifiers[i][0]:
            print(f"Test {i} didn't pass. Expected value {identifiers[i][0]} got {result.stdout} with inserted value '{identifiers[i][0]}'")
        if result.returncode != 0:
            print(f"Test {i} returned non-zero code!")
    print("-------------------------------------------------------------------")
    return


def dataTypeValueTests():
    print("'DataType' value tests!")
    for i in range(len(dataTypeValues)): 
        createWriteFile(i, dataTypeValues)
        with open(_FILENAME, 'r') as file:
            result = subprocess.run([_TESTEDFILE3], stdin=file, capture_output=True, text=True)
        if result.stdout != dataTypeValues[i][1]:
            print(f"Test {i} didn't pass. Expected value {dataTypeValues[i][1]} got {result.stdout} with inserted value '{dataTypeValues[i][0]}'")
        if result.returncode != 0:
            print(f"Test {i} returned non-zero code!")
    print("-------------------------------------------------------------------")
    return

def makefileRun():
    clean = subprocess.Popen("make clean -C " + _MAKEFILE, shell = True, stderr=subprocess.PIPE)
    clean.wait()
    if clean.returncode != 0:
        print("Makefile clean error")
        exit(1)
    result = subprocess.Popen("make -C " + _MAKEFILE, shell = True, stderr=subprocess.PIPE)
    result.wait()
    if result.returncode != 0:
        print("Makefile make error")
        exit(1)
    return

def main():
    makefileRun()
    print("-------------------------------------------------------------------")
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
