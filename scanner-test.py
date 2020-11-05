import os
import subprocess
import random


_FILENAME = "./test.txt"
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
    ['=', "TokenEquals"],
    ['_', "TokenIdentifier"]
]

nullChars = [
    ['\n', "TokenEOF"],
    ['', "TokenEOF"],
]

doubleChars = [
    ['!=', "TokenNotEqual"],  
    ['==', "TokenIsEqual"],
    ['<=', "TokenIsGreaterEqual"],
    ['>=', "TokenIsLessEqual"],
    ['=:', "TokenVarDefine"]
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
        result = subprocess.run(["./scanner-test.out"], stdin=f, capture_output=True, text=True)  
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
        result = subprocess.run(["./scanner-test.out"], stdin=f, capture_output=True, text=True)  
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
        result = subprocess.run(["./scanner-test.out"], stdin=f, capture_output=True, text=True)  
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
        result = subprocess.run(["./scanner-test.out"], stdin=f, capture_output=True, text=True)  
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
        result = subprocess.run(["./scanner-test.out"], stdin=f, capture_output=True, text=True)  
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
        result = subprocess.run(["./scanner-test.out"], stdin=f, capture_output=True, text=True)  
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
    result = subprocess.run(["./scanner-test.out"], stdin=f, capture_output=True, text=True)  
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
    result = subprocess.run(["./scanner-test.out"], stdin=f, capture_output=True, text=True)  
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
    result = subprocess.run(["./scanner-test.out"], stdin=f, capture_output=True, text=True)  
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
    result = subprocess.run(["./scanner-test.out"], stdin=f, capture_output=True, text=True)  
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
    f = open(_FILENAME,"w+")
    f.write("_")
    
    # randomNbr = random.randint(1,25)
    # j = 0
    # while j != randomNbr:
    #     f.write(chr(random.randint(32,127)))
    #     f.write("\n")
    #     j += 1
    # f.close()
    # return

def main():
    simpleTest()
    simpleTestInline()
    simpleTestNullChars()
    errorChecks()
    doubleCharsTest()
    skippingTest()
    commentLineTest()
    commentsBlockTest()
    return


main()