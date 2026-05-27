from combi_utilities import bracelets, filter_valid_bracelets, valid_arms
from geom_utilities import Star, replace_sequences
from io__utilities import write_constraint, write_equality
import pandas as pd

global k_for_k_planar
k_for_k_planar = 4 



# Constructs stars
unique_bracelets = list(bracelets(5, 7))
valid_bracelets = filter_valid_bracelets(list(bracelets(5, 7)), k_for_k_planar)
print(f"Bracelets: {len(unique_bracelets)} | Valid: {len(valid_bracelets)}")

STARS = [Star(b) for b in valid_bracelets]

valid_arms = valid_arms(range(5)) # for trails ending in triangles 

# first, analyse stars and put the relevant information into a CSV 

trails = [
    "t0_c3_tri_c5_pen",
    "t1_c3_tri_c5_pen",
    "t0_c4_tri_c5_pen", 
    "t1_c4_tri_c5_pen", 
    "t2_c4_tri_c5_pen", 
    "t0_c5_pen_c5_pen", 
    "t1_c5_pen_c5_pen",
]

relevant_arms = [
    (0,2),
    (0,3),
    (1,2),
    (2,2),
    (2,3),
    (2,4),
    (3,3),
]
relevant_qs = [
    #(0,2),
    #(0,3),
    #(1,2),
    #(2,2),
    #(2,3),
    #(2,4),
    (3,3),  ###
    (2,9),
    (3,9),
    (9,9)
]
relevant_corners = [
    "c6_qua", 
    "mysterycell",
    #"hook",
    #"cone"
]

table_data = []
for i, S in enumerate(STARS):
    row = {'NAME': f"STAR_{i+1}"}
    row["bracelet"] = str(S.bracelet)
    row["sharp"] = 1 if len({5,6}.intersection(S.bracelet)) == 0 else 0

    # record trails
    for T in trails:
        row[T] = S.get_number_of_trails(T)
    
    # record edges
    row["E_2"] = S.get_e2()
    row["E_3"] = S.get_e3()

    # record q_sequences
    for A, B in relevant_qs:
        row[f"Q_{A}_{B}"] = S.count_qs(A,B)

    for A, B in relevant_arms:
        row[f"ARM_{A}_{B}"] = S.count_arms(A,B)

    # record corners
    corner_sequences = replace_sequences(S.C)
    for RC in relevant_corners:
        row[RC] = corner_sequences.count(RC)

    table_data.append(row)


df = pd.DataFrame(table_data)
df.to_csv("../include4/star_data.csv", index=False)
    
    
# VARIABLE NAMES
STAR_NAMES = [name for name in df.iloc[:,0]]

with open("../include4/stardefs.inc", "w") as f:
    for name in STAR_NAMES:
        f.write(f'  const int {name} = VAR++; var_names.push_back("{name}");\n')

# EQUALITY SUM STAR = sum_i (star_i)
with open("../include4/star_sum_equality.inc", "w") as f:
    LHS = [(name, 1) for name in STAR_NAMES]
    LHS.append(("SUM_STAR", -1))
    write_equality(LHS, f"8A - starSUM ", f)

# Write Trail count lower bounds 
with open("../include4/star_sum_lbs.inc", "w") as f:
    for T in trails: 
        LHS = []
        for _, row in df.iterrows():
            val = row[T]
            if pd.notna(val) and int(val) != 0:
                LHS.append((row["NAME"], val))
        if T in ("t0_c5_pen_c5_pen", "t1_c5_pen_c5_pen"):
            LHS.append((T, -2))         # For dull stars, double count!
        else:
            LHS.append((T, -1))
        write_constraint(LHS, False, f"A1 - trail LBs:  {T}", f)        

with open("../include4/star_arms.inc", "w") as f:
    for A,B in relevant_arms: 
        LHS = []
        for _, row in df.iterrows():
            val = row[f"ARM_{A}_{B}"]
            if pd.notna(val) and int(val) != 0:
                LHS.append((row["NAME"], val))
        LHS.append((f"ARM_{A}_{B}", -1))
        write_constraint(LHS, False, f"A2 - ARM_{A}_{B}", f)     

with open("../include4/star_qs.inc", "w") as f:
    for A,B in relevant_qs: 
        LHS = []
        for _, row in df.iterrows():
            val = row[f"Q_{A}_{B}"]
            if pd.notna(val) and int(val) != 0:
                LHS.append((row["NAME"], val))
        LHS.append((f"Q_{A}_{B}", -1))
        write_constraint(LHS, False, f"A2.8-11 - Q_{A}_{B}", f)  

with open("../include4/star_corners.inc", "w") as f:
    for C in relevant_corners: 
        LHS = []
        for _, row in df.iterrows():
            val = row[C]
            if pd.notna(val) and int(val) != 0:
                LHS.append((row["NAME"], val))
        if C in ("c6_qua"):
            LHS.append((C, -2))         # For dull stars, double count!
        else:
            LHS.append((C, -1))
        write_constraint(LHS, False, f"A3 - corner {C}",  f)   


with open("../include4/edge_sums.inc", "w") as f:
    for E in ("E_2", "E_3"): 
        LHS = []
        for _, row in df.iterrows():
            val = row[E]
            if pd.notna(val) and int(val) != 0:
                LHS.append((row["NAME"], val))
        if E == "E_2":
            LHS.append(("t1_c4_tri_c5_qua", 1))
            LHS.append(("F1_AA", 1))         
        elif E == "E_3":
            LHS.append(("t2_c4_tri_c5_qua", 1))
            LHS.append(("F2_AA", 1))         
        LHS.append((E, -1))
        write_constraint(LHS, False, f"9.B-C - star edges {E}", f)  