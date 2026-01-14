import itertools
import functools
from io__utilities import count_cells_by_trails, write_equality

global k_for_k_planar
k_for_k_planar = 4 

class Trail:
    def __init__(self, A, B, i):
        if A >= B:
            A, B = B, A
        self.A = A
        self.B = B
        self.i = i
    
    def countcell(self, cell):
        r = 1 if self.A == cell else 0
        r += (1 if self.B == cell else 0)
        return r 

    def __eq__(self, other):
        return self.A == other.A and self.B == other.B and self.i== other.i  
    
    @functools.total_ordering
    def __lt__(self,other):
        return str(self) < str(other)

    def __hash__(self):
        return hash(f"{self.A}{self.B}{self.i}")
    
    def __str__(self):
        return f"t{self.i}_{self.A}_{self.B}"



k_for_k_planar = 4

# Crossing points on cell boundaries of bounding edges 1 and 2 
CR_1 = [2,1,1,2,1]
CR_2 = [2,1,2,2,1]

Names = [
    "c3_tri",
    "c4_tri",
    "c5_qua",
    "c5_pen",
    "large"   
]
INNER_SEGMENTS = {
    "c3_tri": 3,
    "c4_tri": 1,
    "c4_qua": 4,
    "c5_qua": 2,
    "c5_pen": 5
}

def get_valid_trails(k_for_k_planar=4):
    VT = set()
    for i in range(k_for_k_planar):
        for a,b in itertools.combinations_with_replacement(range(5), 2):

            # This is to ensure no triangle-to-triangle trails exist 
            if a in [0,1] and b in [0,1]:       
                continue

            # Check if we exhausted the number of crossings 
            e_1_cr = CR_1[a] + CR_1[b] + i - 1        # crossings on bounding edge 1
            e_2_cr = CR_2[a] + CR_2[b] + i - 1        # crossings on bounding edge 2
            if(e_1_cr <= k_for_k_planar and e_2_cr <= k_for_k_planar):
                VT.add(Trail(Names[a],Names[b],i))
            elif b == 2 and (e_1_cr + 1 <= k_for_k_planar and e_2_cr - 1 <= k_for_k_planar): 
                # If b is a c5qua-cell with flip orientation
                VT.add(Trail(Names[a],Names[b],i))
    return VT

valid_trails = get_valid_trails(k_for_k_planar)
print(len(valid_trails))


# Now we generate the variable names and the corresponding constraints. 

valid_trails = sorted(list(valid_trails))



## Generate variable names and definitions
# 
with open("../include4/traildefs.inc", "w") as f:
    for i in range(len(valid_trails)):
        t_name = valid_trails[i]
        f.write(f'   const int {t_name}  = VAR++; var_names.push_back("{t_name}");\n')

# 
#   Generate Constraints 4.*
#
with open("../include4/trailcounts.inc", "w") as f:
    for k_for_k_planar,v in INNER_SEGMENTS.items():
        count_cells_by_trails(valid_trails, k_for_k_planar, f)
        f.write("\n")
        f.write("\n")

    quacells = [(str(T), T.i) for T in valid_trails if T.i > 0]
    quacells.append(("c4_qua", -2))
    write_equality(quacells, f)