import sys
from fractions import Fraction as fr

def readfile(path):
    with open(path, "r") as f:
        return eval(f.read())       

if len(sys.argv) < 2 or sys.argv[1] not in ['E', 'X']:
    print("Usage: python script.py [E|X]")
    sys.exit(1)

target = sys.argv[1]

# Adjust these paths if your verification folder is somewhere else!
DIR = "." 
M = readfile(f"{DIR}/M.txt")
V = readfile(f"{DIR}/V.txt")
C_names = readfile(f"{DIR}/C_names.txt")
c = readfile(f"{DIR}/c{target.lower()}.txt") # Duals

# Load the new primal values
try:
    p = readfile(f"{DIR}/p{target.lower()}.txt") # Primals
except FileNotFoundError:
    print("Error: Could not find primal values file (pe.txt / px.txt).")
    print("Did you add 'save_primal_to_file' to your C++ code?")
    sys.exit(1)

num_constraints = len(M)
num_vars = len(V)

print("\n==================================================")
print(f"             LP DIAGNOSTICS ({target})            ")
print("==================================================")

# --- 1. THE CURRENT STATE (Primals) ---
print("\n=== 1. CURRENT SOLUTION (Non-Zero Variables) ===")
print("What the solver actually managed to build:")
nonzero_count = 0
for j in range(num_vars):
    if p[j] != 0:
        print(f"  {V[j].ljust(25)} = {p[j]}")
        nonzero_count += 1
print(f"  (Total non-zero variables: {nonzero_count})")

# --- 2. THE CEILING (Duals) ---
print("\n=== 2. ACTIVE BOTTLENECKS (Non-Zero Duals) ===")
print("These constraints are actively preventing further optimization:")
for i in range(num_constraints):
    if c[i] != 0:
        print(f"  [{str(c[i]).rjust(5)}] {C_names[i]}")

# --- 3. THE WISH LIST (Reduced Costs) ---
print("\n=== 3. STARVING VARIABLES (Reduced Costs < 0) ===")
print("The solver wants to increase these variables to improve the score,")
print("but doing so would violate the bottlenecks listed above:")

# Calculate reduced costs: s = c * M
s = [fr(0, 1)] * num_vars
for i in range(num_constraints):
    if c[i] != 0:
        for j in range(num_vars):
            if M[i][j] != 0:
                s[j] += c[i] * fr(M[i][j], 1)

starving_count = 0
for j in range(num_vars):
    if s[j] < 0:
        print(f"  {V[j].ljust(25)} : {s[j]}")
        starving_count += 1

if starving_count == 0:
    print("  (None! The solver has literally no ideas left.)")
print("\n==================================================\n")