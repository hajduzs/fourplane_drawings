import itertools
from geom_utilities import CrossingData

######################### 
# Funtions to generate bracelets 
######################### 


def rotations(seq):
    n = len(seq)
    return [seq[i:] + seq[:i] for i in range(n)]

def reflections(seq):
    rev = seq[::-1]
    n = len(seq)
    return [rev[i:] + rev[:i] for i in range(n)]

def canonical(seq):
    """return the canonical representative for grouping"""
    forms = rotations(seq) + reflections(seq)
    return min(forms)

def bracelets(n, k):
    """generate unique bracelets"""
    seen = set()
    for seq in itertools.product(range(k), repeat=n):
        canon = canonical(seq)
        if canon not in seen:
            seen.add(canon)
            yield canon 

######################### 
# Filtering bracelets (valid stars) under geometric constraints 
######################### 

def filter_valid_bracelets(bracelets, max_crossings):
    valid_bracelets = []
    for bracelet in bracelets:
        valid = True
        # we check for every e_i (starting edge)
        for i in range(5):
            # The two trails contribute this many additional crossings
            cr_cw = CrossingData.crossing(bracelet[(i + 1) % 5])   
            cr_ccw = CrossingData.crossing(bracelet[(i - 1) % 5])
            if cr_cw + cr_ccw > max_crossings - 2:  # Account for the 2 crossings already on the boundary of the core
                valid = False
                break
        if valid:
            valid_bracelets.append(bracelet)
    return valid_bracelets

######################## 
# generate all unique 2-tuples from a given set 
######################### #
def valid_arms(items):
    return sorted(itertools.combinations_with_replacement(items, 2))


def expand_sequence(sequence, symbols):
    """
    Takes a sequence of symbol keys and generates all possible strings
    by performing a single-step expansion.
    """
    options = [symbols.get(s) for s in sequence]
    return [list(combo) for combo in itertools.product(*options)]