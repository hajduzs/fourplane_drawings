# Script to "turn off" certain star types, completely forbidding them from the drawing. 
import sys
from pathlib import Path

# Calculate the path to the root 'fp' directory
root_path = Path(__file__).resolve().parent.parent
sys.path.append(str(root_path))

# Now you can use an absolute import instead of a relative one
from drawing_explorer.io__utilities import write_constraint

f_stars = [i for i in range(1, 172 + 1)]
f_stars.remove(86)

with open("./turn_off_the_sky.inc", "w") as f:
    LHS = [(f"STAR_{i}", 1) for i in f_stars]
    write_constraint(LHS, False, f)
    
