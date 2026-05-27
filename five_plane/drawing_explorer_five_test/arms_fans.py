from combi_utilities import expand_sequence
from io__utilities import write_constraint, write_vardef, write_equality

X = "X"   #"c4_qtri"
A = "A"   #"c5_tri"
B = "B"   #"c5_pen"
C = "C"   #"large"

FAN_SEQUENCES = {
    "F1_AA" : (A, X ,A),
    "F1_AB" : (A, X ,B),
    "F1_AC" : (A, X ,C),
    "F1_BB" : (B, X ,B),
    "F1_BC" : (B, X ,C),
    "F1_CC" : (C, X ,C),
    "F2_AA" : (A, X, X ,A),
    "F2_AB" : (A, X, X ,B),
    "F2_AC" : (A, X, X ,C),
    "F2_BB" : (B, X, X ,B),
    "F2_BC" : (B, X, X ,C),
    "F2_CC" : (C, X, X ,C),
    "F3_AA" : (A, X, X, X, A),
    "F3_AB" : (A, X, X, X, B),
    "F3_AC" : (A, X, X, X, C),
    "F4_AA" : (A, X, X, X, X, A)
}

M = "M"
P = "P"
Q = "Q"
R = "R"

SYMBOLS = {
    "M": ("M"),
    "P": (A),
    "Q": (X, B, C),
    "R": (B, C)
}

ARM_SEQUENCES = {
    (0,2): (R, M, P),
    (0,3): (R, M, Q, P),
    (1,2): (R, Q, M, P),
    (2,2): (P, M, P),
    (2,3): (P, M, Q, P),
    (2,4): (P, M, Q, Q, P),
    (3,3): (P, Q, M, Q, P)
}

def trim_to_fan(seq):
    """ Gets Fan sequence contained in the arm"""
    seq = "".join(seq)
    m_idx = seq.find("M")

    # 2. Identify chained X characters (c4_cells)
    left = m_idx
    while left > 0 and seq[left - 1] == "X":
        left -= 1
    
    right = m_idx
    while right < len(seq) - 1 and seq[right + 1] == "X":
        right += 1

    # Include D_o and D_l+1
    start = max(0, left - 1)
    end = min(len(seq), right + 2)
    
    sub_seq = seq[start:end]
    
    # Final step: replace M with X
    return tuple(sub_seq.replace("M", "X"))

def find_matching_fan_keys(sequences):
    """ Returns a list of keys from fan_dict where the value (or its reverse) matches """
    matched_keys = []

    target_subs = [s for s in sequences]
    
    for key, value in FAN_SEQUENCES.items():
        reversed_value = value[::-1]
        
        # Check if the fan sequence or its reverse is in our list of targets
        if value in target_subs or reversed_value in target_subs:
            matched_keys.append(key)
            
    return sorted(list(set(matched_keys)))

# print([x for x in expand_sequence((P, M, Q, Q, P), SYMBOLS)])
# print(list(set([trim_to_fan(x) for x in expand_sequence((P, M, Q, Q, P), SYMBOLS)])))


        
    
with open("../include5/arm_fan_definitions.inc", "w") as f:
    for k in FAN_SEQUENCES.keys():
        write_vardef(k, f)
    #for a,b in ARM_SEQUENCES.keys():
    #    write_vardef(f"ARM_{a}_{b}", f)

with open("../include5/fan_c4_cells.inc", "w") as f:
    LHS = []
    for k,v in FAN_SEQUENCES.items():
        LHS.append((k, v.count(X)))
    LHS.append(("c4_tri",-1))
    write_equality(LHS, f)
exit()
with open("../include4/arm_fan_containments.inc", "w") as f:

    # Dictionary to store the matching arm cases for every type of fan.
    fan_lower_bounds = {k:[] for k in FAN_SEQUENCES.keys()}

    for (A,B), arm_bounding_seq in ARM_SEQUENCES.items():
        possible_sequences = list(set(trim_to_fan(x) for x in expand_sequence(arm_bounding_seq,SYMBOLS)))
        MK = find_matching_fan_keys(possible_sequences)
        EQ = []
        
        for fan in MK:
            C_Name = f"AFC_{A}_{B}_{fan}"
            # define the variable right away.
            write_vardef(C_Name, f)
            # fill equality list 
            EQ.append((C_Name, 1))
            fan_lower_bounds[fan].append(C_Name)
            
        # finish with defining equality
        EQ.append((f"ARM_{A}_{B}", -1))
        write_equality(EQ, f)

    #print(fan_lower_bounds)
    # finally, finish with recording te LB constraints on fans 
    for k, v in fan_lower_bounds.items():
        LHS = []
        for AFC in v:
            LHS.append((AFC, 1))
        LHS.append((k, -1))
        write_constraint(LHS, False, f)


        

