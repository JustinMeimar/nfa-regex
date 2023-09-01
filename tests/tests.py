import subprocess 
import sys
import os

cmd = "../bin/regex"

def run_test(re, string, expected_result):
     
    args = [re, string]
    process = subprocess.run([cmd, *args], stdout=subprocess.PIPE, text=True)
    if process.returncode == expected_result:
        print(" ===\033[32m" + " PASS" + "\033[0m", "re:", '\033[33m' + re + "\033[0m", "\tstring:" , '\033[33m' + string + "\033[0m")
        return True 
    else:
        print(" ===\033[31m" + " FAILED" + "\033[0m", "re:", '\033[33m' + re + "\033[0m", "\tstring:" , '\033[33m' + string + "\033[0m") 
        return False 

def prepare_tests(filename):
    pass_count = 0
    count = 0
 
    with open(filename) as file:
        for line in file:
            line = line.rstrip('\n')
            inputs = line.split(" ")
            re = inputs[0]
            string = inputs[1]
            expected_result = int(ord(inputs[2]) - ord('0'))
            if run_test(re, string, expected_result):
                pass_count += 1
            count += 1
     
    print("PASSED:", pass_count, "/", count)
    if pass_count != count:
        raise Exception 
    return 
    
if __name__ == "__main__": 

    test_dir = os.path.join(os.path.dirname(__file__), "keys") 
    for file in os.listdir(test_dir):
        try:  
            prepare_tests(os.path.join(test_dir, file))
        except Exception:
            print(f"Test: {file} failed")
            # sys.exit(1)