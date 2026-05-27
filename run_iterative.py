import subprocess
import os
import sys
from fractions import Fraction as fr

def read_verification_file(path):
    if not os.path.exists(path):
        return None
    with open(path, "r") as f:
        try:
            return eval(f.read(), {"fr": fr})
        except:
            return None

def run_solver():
    print("Running fourplane solver...")
    result = subprocess.run(["./fourplane"], capture_output=True, text=True)
    if result.returncode != 0:
        print("Solver failed!")
        print(result.stderr)
        return None
    
    # Extract objective value from output
    obj_val = None
    for line in result.stdout.split('\n'):
        if "Value:" in line:
            obj_val = line.split("Value:")[1].strip()
            break
    return obj_val

def get_unused_constraints(target='X'):
    names = read_verification_file("verification/C_names.txt")
    dual = read_verification_file(f"verification/c{target.lower()}.txt")
    
    if names is None or dual is None:
        return []

    unused = []
    for i in range(min(len(names), len(dual))):
        if dual[i] == 0:
            unused.append(names[i])
    return unused

def main():
    target = 'X'
    if len(sys.argv) > 1:
        target = sys.argv[1].upper()

    disabled_path = "verification/disabled_constraints.txt"
    log_path = "verification/iteration_log.csv"
    
    # Reset disabled constraints
    if os.path.exists(disabled_path):
        os.remove(disabled_path)
    open(disabled_path, 'a').close()

    with open(log_path, "w") as log:
        log.write("iteration,num_constraints,objective_value,unused_count\n")

    iteration = 0
    while True:
        iteration += 1
        print(f"\n--- Iteration {iteration} ---")
        
        obj_val = run_solver()
        if obj_val is None:
            break
            
        names = read_verification_file("verification/C_names.txt")
        unused = get_unused_constraints(target)
        
        num_constr = len(names) if names else 0
        print(f"Objective: {obj_val}")
        print(f"Total Constraints: {num_constr}")
        print(f"Unused Constraints: {len(unused)}")
        
        with open(log_path, "a") as log:
            log.write(f"{iteration},{num_constr},{obj_val},{len(unused)}\n")
            
        if not unused:
            print("No more unused constraints found. Finished!")
            break
            
        # Strategy: Disable a chunk of unused constraints.
        # Removing all at once might be risky if dependencies exist, 
        # but since they are "redundant" in the current LP, it should be fine.
        # To be safe, we'll remove up to 50 at a time, or 20% of unused.
        to_disable = unused[:max(1, min(len(unused), 50, len(unused)//5))]
        
        print(f"Disabling {len(to_disable)} constraints...")
        with open(disabled_path, "a") as f:
            for name in to_disable:
                f.write(name + "\n")

if __name__ == "__main__":
    main()
