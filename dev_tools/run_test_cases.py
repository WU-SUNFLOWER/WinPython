import os
import subprocess
from concurrent.futures import ThreadPoolExecutor, as_completed

# ANSI 转义码定义颜色
GREEN = '\033[92m'  # 绿色
RED = '\033[91m'    # 红色
RESET = '\033[0m'   # 重置颜色

root_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..")
test_cases_dir = os.path.join(root_dir, "./test_cases")
executable_path = os.path.join(root_dir, "./x64/Release/WinPython.exe")

def run_test_case(test_case_path):
    command = [executable_path, test_case_path]
    result = subprocess.run(command, 
                            capture_output=True, 
                            text=False)
    
    test_case_name = os.path.basename(test_case_path)
    
    if result.returncode == 0:
        outcome = f"{GREEN}Test Case {test_case_name} Passed!{RESET}"
    else:
        outcome = f"{RED}Test Case {test_case_name} Failed!\nError: {result.stderr.decode('utf-8')}{RESET}"
        
    pyc_file_path = test_case_path + 'c'
    if os.path.exists(pyc_file_path):
        os.remove(pyc_file_path)
        
    return outcome

def get_test_cases():
    result = []
    for root, _, filenames in os.walk(test_cases_dir):
        for filename in filenames:
            if filename.endswith(".py"):
               result.append(os.path.join(root, filename))
    return result

def run_test_concurrently():
    test_cases = get_test_cases()
    with ThreadPoolExecutor(max_workers=os.cpu_count()) as executor:
        future_to_file = dict()
        for test_case in test_cases:
            future_to_file[executor.submit(run_test_case, test_case)] = test_case
            
        for future in as_completed(future_to_file):
            print(future.result())
    
if __name__ == "__main__":
    # https://stackoverflow.com/questions/40754673/python-fails-to-output-ansi-color-codes-for-the-terminal
    os.system('')
    
    run_test_concurrently()