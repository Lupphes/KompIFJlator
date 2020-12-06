import os
import subprocess

_REALPATH = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

_TESTEDFILE1 = _REALPATH + "/../build/parser-test.out"
_MAKEFILE = _REALPATH + "/../src/"
_TESTSFOLDER = _REALPATH + "./parser/parser-errors/"


def testErrorFiles():
    print("'Sanity Check Error' test!")
    for filename in os.listdir(_TESTSFOLDER):
        with open(filename, 'r') as file:
            result = subprocess.run([_TESTEDFILE1], stdin=file, capture_output=True, text=True)
            if result.returncode == 0:
                print(f"Test did not return 1 as exit code!")
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
    testErrorFiles()
    return

main()