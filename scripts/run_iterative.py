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

def get_unused_rules(target='X'):
    names = read_verification_file("verification/C_names.txt")
    dual = read_verification_file(f"verification/c{target.lower()}.txt")
    
    if names is None or dual is None:
        return []

    # Group dual values by their base rule name
    rules_dual_values = {} # {base_name: [list of dual values]}
    
    for i in range(min(len(names), len(dual))):
        full_name = names[i]
        # Strip suffixes used by io__utilities.py
        base_name = full_name
        if full_name.endswith("_LEQ"): base_name = full_name[:-4]
        elif full_name.endswith("_GEQ"): base_name = full_name[:-4]
        
        if base_name not in rules_dual_values:
            rules_dual_values[base_name] = []
        rules_dual_values[base_name].append(dual[i])

    # A rule is "unused" if ALL its components have dual 0.
    # This ensures that if any part of an equality is active, we keep the whole rule.
    unused = []
    for base_name, duals in rules_dual_values.items():
        if all(d == 0 for d in duals):
            unused.append(base_name)

    return unused

def main():
    if len(sys.argv) < 2:
        print("Usage: python run_iterative.py <project_dir> [target X|E]")
        sys.exit(1)
        
    project_dir = sys.argv[1]
    target = 'X'
    if len(sys.argv) > 2:
        target = sys.argv[2].upper()

    # Change to project directory
    if not os.path.exists(project_dir):
        print(f"Error: Project directory {project_dir} not found.")
        sys.exit(1)
    
    os.chdir(project_dir)
    
    # Adjust binary name
    binary = "./fourplane" if "four" in project_dir else "./fiveplane"

    disabled_path = "verification/disabled_constraints.txt"
    log_path = "verification/iteration_log.csv"
    
    # Load existing disabled constraints
    disabled_set = set()
    if os.path.exists(disabled_path):
        with open(disabled_path, "r") as f:
            for line in f:
                name = line.strip()
                if name:
                    disabled_set.add(name)
    else:
        # Create the file if it doesn't exist
        os.makedirs(os.path.dirname(disabled_path), exist_ok=True)
        open(disabled_path, 'a').close()

    # Append to log instead of overwriting to keep history
    if not os.path.exists(log_path):
        with open(log_path, "w") as log:
            log.write("iteration,num_constraints,objective_value,removed_rule\n")

    iteration = 0
    while True:
        iteration += 1
        print(f"\n--- Iteration {iteration} ---")
        
        print(f"Running {binary} solver...")
        result = subprocess.run([binary], capture_output=True, text=True)
        if result.returncode != 0:
            print("Solver failed!")
            print(result.stderr)
            break
            
        # Extract objective value from output
        obj_val = None
        for line in result.stdout.split('\n'):
            if "Value:" in line:
                obj_val = line.split("Value:")[1].strip()
                break
        
        if obj_val is None:
            print("Could not find objective value in solver output.")
            break
            
        names = read_verification_file("verification/C_names.txt")
        unused_rules = get_unused_rules(target)
        
        # Filter out rules that are already disabled
        unused_rules = [r for r in unused_rules if r not in disabled_set and 
                        (r + "_LEQ") not in disabled_set and 
                        (r + "_GEQ") not in disabled_set]
        
        num_constr = len(names) if names else 0
        print(f"Objective: {obj_val}")
        print(f"Total Active Constraints: {num_constr}")
        print(f"Unused Rules available: {len(unused_rules)}")
        
        if not unused_rules:
            print("No more unused rules found. Finished!")
            break
            
        # Strategy: Remove exactly ONE rule
        to_disable_base = unused_rules[0]
        
        print(f"Disabling rule: {to_disable_base}")
        new_entries = [to_disable_base, to_disable_base + "_LEQ", to_disable_base + "_GEQ"]
        
        with open(disabled_path, "a") as f:
            for entry in new_entries:
                if entry not in disabled_set:
                    f.write(entry + "\n")
                    disabled_set.add(entry)

        with open(log_path, "a") as log:
            log.write(f"{iteration},{num_constr},{obj_val},{to_disable_base}\n")

if __name__ == "__main__":
    main()
