import subprocess 

def regex_test():

    pass_count = 0
    count = 0
 
    with open("key.txt") as file:
        for line in file:
            line = line.rstrip('\n')
            inputs = line.split(" ")

            re = inputs[0]
            string = inputs[1]
            expected_result = ord(inputs[2]) - ord('0')

            return_code = subprocess.call(['bash', 'accept.sh', re, string])

            if return_code == expected_result:
                print(" ===\033[32m" + " PASS" + "\033[0m", "re:", '\033[33m' + re + "\033[0m", "\tstring:" , '\033[33m' + string + "\033[0m")
                pass_count+=1
            else:
                print("FAIL") 
            count+=1
    
    print("PASSED:", pass_count, "/", count)

if __name__ == "__main__": 
    regex_test()