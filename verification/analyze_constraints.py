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

def analyze(target='X'):
    dir_path = "./verification"
    names = readfile(f"{dir_path}/C_names.txt")
    
    if target == 'E':
        dual = readfile(f"{dir_path}/ce.txt")
    else:
        dual = readfile(f"{dir_path}/cx.txt")
    
    if names is None or dual is None:
        print(f"Error: Missing verification files for target {target}.")
        print(f"Expected {dir_path}/C_names.txt and {dir_path}/c{target.lower()}.txt")
        return

    print(f"Analysis for target {target}:")
    unused = []
    used = []
    
    # Dual values correspond to constraints.
    for i in range(min(len(names), len(dual))):
        val = dual[i]
        if val == 0:
            unused.append(names[i])
        else:
            used.append((names[i], val))
            
    print(f"Total constraints: {len(names)}")
    print(f"Used (active) constraints: {len(used)}")
    print(f"Unused (redundant) constraints: {len(unused)}")
    
    # Save unused to a file for easy cutting
    with open(f"{dir_path}/unused_constraints.txt", "w") as f:
        for name in unused:
            f.write(name + "\n")
            
    print(f"\nList of unused constraints saved to {dir_path}/unused_constraints.txt")
    
    if used:
        print("\nTop 10 active constraints (by dual coefficient):")
        used.sort(key=lambda x: abs(x[1]), reverse=True)
        for name, val in used[:10]:
            print(f"  {str(val).ljust(20)} {name}")

if __name__ == "__main__":
    target = 'X'
    if len(sys.argv) > 1:
        target = sys.argv[1].upper()
    analyze(target)
