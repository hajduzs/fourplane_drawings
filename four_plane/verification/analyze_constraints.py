import sys
import os
from fractions import Fraction as fr

def readfile(path):
    if not os.path.exists(path):
        return None
    with open(path, "r") as f:
        content = f.read()
        # Define fr so eval can handle fr(n,d)
        return eval(content, {"fr": fr})

def analyze(target='X', dir_path="./verification"):
    names = readfile(f"{dir_path}/C_names.txt")
    disabled_path = f"{dir_path}/disabled_constraints.txt"
    
    disabled_count = 0
    if os.path.exists(disabled_path):
        with open(disabled_path, "r") as f:
            disabled_count = len([line for line in f if line.strip()])

    if target == 'E':
        dual = readfile(f"{dir_path}/ce.txt")
    else:
        dual = readfile(f"{dir_path}/cx.txt")
    
    if names is None or dual is None:
        print(f"Error: Missing verification files for target {target}.")
        return

    print(f"Analysis for target {target}:")
    unused = []
    used = []
    
    for i in range(min(len(names), len(dual))):
        val = dual[i]
        if val == 0:
            unused.append(names[i])
        else:
            used.append((names[i], val))
            
    print(f"Constraints currently enabled: {len(names)}")
    print(f"Constraints currently disabled: {disabled_count}")
    print(f"--- Of those enabled ---")
    print(f"Active (Dual != 0): {len(used)}")
    print(f"Redundant (Dual == 0): {len(unused)}")

    # Half-constraint analysis (Equalities with one-sided tightness)
    rule_duals = {}
    for i in range(min(len(names), len(dual))):
        full_name = names[i]
        base_name = full_name
        if full_name.endswith("_LEQ"): base_name = full_name[:-4]
        elif full_name.endswith("_GEQ"): base_name = full_name[:-4]
        
        if base_name not in rule_duals:
            rule_duals[base_name] = []
        rule_duals[base_name].append(dual[i])

    half_constraints = 0
    total_equalities = 0
    for base_name, duals in rule_duals.items():
        if len(duals) >= 2:
            total_equalities += 1
            # Check if it's a "half-constraint" (bottleneck in one direction)
            # i.e., at least one is non-zero, and at least one is zero.
            has_active = any(d != 0 for d in duals)
            has_inactive = any(d == 0 for d in duals)
            if has_active and has_inactive:
                half_constraints += 1

    print(f"\n--- Detailed Constraint Status ---")
    print("(RED = Unused/Redundant, DEFAULT = Active Bottleneck)")
    
    RED = "\033[91m"
    RESET = "\033[0m"

    # Sort base names for consistent output
    for base_name in sorted(rule_duals.keys()):
        duals = rule_duals[base_name]
        
        # Determine if we should color the whole line or parts
        parts = []
        if len(duals) == 1:
            d = duals[0]
            color = RED if d == 0 else ""
            print(f"  {color}{base_name.ljust(40)} (Dual: {d}){RESET}")
        else:
            # It's an equality (or multiple parts)
            active_parts = [d for d in duals if d != 0]
            
            # Label the parts for clarity
            # Note: We don't have the original suffixes here easily, 
            # so we'll just show the values.
            status_str = ""
            for d in duals:
                color = RED if d == 0 else ""
                status_str += f"{color}[Dual: {d}]{RESET} "
            
            # Color the name RED only if ALL parts are 0
            name_color = RED if not active_parts else ""
            print(f"  {name_color}{base_name.ljust(40)}{RESET} {status_str}")

    # Save unused to a file for easy cutting
    with open(f"{dir_path}/unused_constraints.txt", "w") as f:
        for name in unused:
            f.write(name + "\n")
            
    print(f"\nList of unused constraints saved to {dir_path}/unused_constraints.txt")
    
    if used:
        print("\active constraints (sorted by dual coefficient):")
        used.sort(key=lambda x: abs(x[1]), reverse=True)
        for name, val in used:
            print(f"  {str(val).ljust(20)} {name}")

if __name__ == "__main__":
    target = 'X'
    if len(sys.argv) > 1:
        target = sys.argv[1].upper()
    analyze(target)
