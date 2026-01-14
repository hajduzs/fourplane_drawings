from combi_utilities import expand_sequence
from io__utilities import write_constraint, write_vardef, write_equality

from trails import Trail, get_valid_trails

O = "c4_qua"
A = "c3_tri"
B = "c4_tri"
C = "c5_qua"
D = "c5_pen"
E = "large"

SYMBOLS = {
    "M": ("M"),
    "P": (B, C, E),
    "Q": (O, A, C, D, E),
    "R": (O, C, D, E),
    "S": (B, E),
    "T": (C, E)
}

Q_SEQUENCES = {
    (2,2): ("P", "M", "P"),
    (2,3): ("P", "M", "Q", "P"),
    (2,4): ("P", "M", "Q", "R", "P"),
    (3,3): ("P", "Q", "M", "Q", "P"),
    (0,2): ("S", "Q", "R", "M", "P"),
    (0,3): ("S", "Q", "R", "M", "Q", "P"),
    (1,2): ("S", "R", "M", "P"),
    (2,9): ("P", "M", "T"),
    (3,9): ("P", "Q", "M", "T"),
    (9,9): ("T", "M", "T")
}

def trim_to_trail(seq):
    """ Gets Trail charged to the edge"""
    # seq is a list of strings
    try:
        m_idx = seq.index("M")
    except ValueError:
        return tuple(seq) # Handle case where M is missing

    # 2. Identify chained "c4_qua" strings
    left = m_idx
    while left > 0 and seq[left - 1] == "c4_qua":
        left -= 1
    
    right = m_idx
    while right < len(seq) - 1 and seq[right + 1] == "c4_qua":
        right += 1

    # Include T_o and T_l+1
    start = max(0, left - 1)
    end = min(len(seq), right + 2)
    
    sub_seq = seq[start:end]
    
    # Final step: replace "M" with "c4_qua"
    return tuple("c4_qua" if x == "M" else x for x in sub_seq)

VALID = get_valid_trails()

def find_matching_trails(sequences):
    """gets matching trails names that fit anything from the sequences"""
    trail_candidates = set()
    for s in sequences: 
        i = len(s) - 2
        A = s[0]
        B = s[len(s)-1]
        trail_candidates.add(Trail(A, B, i))
    return sorted([str(t) for t in VALID.intersection(trail_candidates)])

if False:
    possible_sequences = list(set(trim_to_trail(x) for x in expand_sequence(Q_SEQUENCES[(2,3)],SYMBOLS))) 
    print(possible_sequences)
    print(len(possible_sequences))
    trails = find_matching_trails(possible_sequences)
    print(trails)
    print(len(trails))
    exit()

with open("../include4/q_definitions.inc", "w") as f:
    for a,b in Q_SEQUENCES.keys():
        write_vardef(f"Q_{a}_{b}", f)

with open("../include4/Q_Trail_containments.inc", "w") as f:

    # Dictionary to store the matching arm cases for every type of Q-arm.
    trail_lower_bounds = {k:[] for k in [str(t) for t in VALID]}

    for (A,B), arm_bounding_seq in Q_SEQUENCES.items():
        possible_sequences = list(set(trim_to_trail(x) for x in expand_sequence(arm_bounding_seq,SYMBOLS)))
        MT = find_matching_trails(possible_sequences)
        EQ = []
        
        for trail in MT:
            C_Name = f"QTC_{A}_{B}_{str(trail)}"
            # define the variable right away.
            write_vardef(C_Name, f)
            # fill equality list 
            EQ.append((C_Name, 1))
            trail_lower_bounds[trail].append(C_Name)
            
        # finish with defining equality
        EQ.append((f"Q_{A}_{B}", -1))
        write_equality(EQ, f)

    #print(fan_lower_bounds)
    # finally, finish with recording te LB constraints on trails 
    for k, v in trail_lower_bounds.items():
        if len(v) == 0:
            continue
        LHS = []
        for QTC in v:
            LHS.append((QTC, 1))
        LHS.append((k, -1))
        write_constraint(LHS, False, f)