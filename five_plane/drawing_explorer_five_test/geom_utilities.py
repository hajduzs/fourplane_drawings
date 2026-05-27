k_for_k_planar = 5

class CrossingData:
    """Local crossing number, type, and boundary for star-trails."""
    
    # Types are indexed lazy in the code.
    types = [
        "t0_c3_tri_c5_pen",     # 0
        "t1_c3_tri_c5_pen",     # 1
        "t2_c3_tri_c5_pen",     # NEW 2
        "t0_c4_tri_c5_pen",     # 2 3
        "t1_c4_tri_c5_pen",     # 3 4
        "t2_c4_tri_c5_pen",     # 4 5
        "t3_c4_tri_c5_pen",     # NEW 6
        "t0_c5_pen_c5_pen",     # 5 7
        "t1_c5_pen_c5_pen",     # 6 8
        "t2_c5_pen_c5_pen",     # NEW 9
    ]
    crossings = [1, 2, 3, 0, 1, 2, 3, 1, 2, 3] # Additional crossings provided by trail i, not counting the crossing on the boundary of a star S
    segments = [0, 1, 2, 0, 1, 2, 3, 1, 2, 3]  # "unfinshed" edges crossing the trails, which provide the interior inner edge-segments, 
                                      # or the inner segments on the opposing boundaries og other pentagons

    # The "combinatorial" patterns on the bounding edges of the trails, starting from the end of the trail, going towards the core 
    # For c3_tri-s or c5_pen-ending trails, additional elements "hiding" the true bounding edges are also considered
    boundaries = [  
        ['s', 's'],                 # e.g. We start with the other segment, hosted by the other edge bounding, 
        ['s', 's', 'he', 's'],      # We record "independent" edges "sticking out" ot the trail as he (halfedge)
        ['s', 's', 's', 's', 'he', 's'],
        ['v', 's'],                 # v menas a vertex
        ['v', 's', 'he', 's'],
        ['v', 's', 'he', 's', 'he', 's'],
        ['v', 's', 'he', 's', 'he', 's', 'he', 's'],
        ['s', 'hei', 's'],          # hei means the edges with only one discovered crossing at the other pentagons
        ['s', 'hei', 's', 'he', 's'],
        ['s', 'hei', 's', 'he', 's', 'he', 's'],
    ]                               # We will later also add "c" to denote crossing points on the core boundary

    @classmethod
    def crossing(cls, idx): return cls.crossings[idx]
    @classmethod
    def inner_seg(cls, idx): return cls.segments[idx]
    @classmethod
    def boundary(cls, idx): return cls.boundaries[idx]
    @classmethod
    def type_name(cls, idx): return cls.types[idx]


class Edge:
    """Class representing edges and providing functions for ease of use"""
    def __init__(self, k):
        self.id = k
        self.t = None
        self.crossings = set()         # Set of other edges crossing 
        self.halfedges = 0
        self.sureCrossed = False       # Record if we have 'closed' edges, meaning exhausted crossings OR discovered vertices at both ends

    def add_crossing(self, other):
        self.crossings.add(other)

    def CR(self):
        return len(self.crossings) + self.halfedges    # We return the number of counted crossings and the crossings with 'unnamed' halfedges (the ones sticking out)

    def __hash__(self):
        return id(self)

    def __eq__(self, other):
        return self is other

    def __str__(self):
        return str(self.id)
    
class Trail:
    """Small class connecting trails to bounding edges"""
    def __init__(self, t, re: Edge, le: Edge):
        self.t = t
        self.R_edge = re
        self.L_edge = le


class Star:
    """Class for building and analyzing stars"""
    def __init__(self, bracelet):
        self.bracelet = bracelet   # Bounding trail sequence
        self.T = []                # converted to trails
        self.E = []                # Corresponding edges
        self.C = []                # to record corner boundaries 
        self.__construct_star(bracelet)

    def __construct_star(self, bracelet):
        # construct edges
        self.E = [Edge(k) for k in range(5)]
        for i, ti in enumerate(bracelet):
            # Add trail with bounding edges
            tt = Trail(ti, self.E[(i - 1) % 5], self.E[(i + 1) % 5])
            # Add sticking out edges
            tt.R_edge.halfedges += CrossingData.inner_seg(ti)
            tt.L_edge.halfedges += CrossingData.inner_seg(ti)
            
            # If trail ends in a c3_tri, record the bounding edges crossing as well.
            if ti in [0, 1]:
                tt.R_edge.add_crossing(tt.L_edge)
                tt.L_edge.add_crossing(tt.R_edge)
            
            self.T.append(tt)

        # Cross edges along the boundary of the core 
        for i in range(5):
            j = (i + 1) % 5
            self.E[i].add_crossing(self.E[j])
            self.E[j].add_crossing(self.E[i])

        # Probe for exhausted number of crossings
        for i in range(5):
            if self.E[i].CR() == k_for_k_planar:
                self.E[i].sureCrossed = True
            # Otherwise see if we end in vertices (c4_tri cells)
            if self.T[(i - 1) % 5].t in [5, 3, 4, 6] and self.T[(i + 1) % 5].t in [5, 3, 4, 6]:
                self.E[i].sureCrossed = True

        # Record corner boundaries for later exploitation - add vertices if discovered
        for i in range(5):
            j = (i + 1) % 5
            B = CrossingData.boundary(self.bracelet[i]) + ['c'] + list(reversed(CrossingData.boundary(self.bracelet[j])))
            # if we have c3_tri-cells, we need to consider if the other edge crossing from the other side is exhausted or not.
            if self.bracelet[i] in [0, 1] and self.E[(i - 1) % 5].CR() == k_for_k_planar:
                B = ['v'] + B
            if self.bracelet[j] in [0, 1] and self.E[(i + 2) % 5].CR() == k_for_k_planar:
                B += ['v']
            self.C.append(B)

    def get_number_of_trails(self, trailtype):
        """Return the number of trails of a give type"""
        id = CrossingData.types.index(trailtype)
        return self.bracelet.count(id)
    
    def get_e2(self):
        """Get the number of edges crossed 2x on the boundary of the core"""
        return [e.CR() for e in self.E if e.sureCrossed].count(2)

    def get_e3(self):
        """Get the number of edges crossed 3x on the boundary of the core"""
        return [e.CR() for e in self.E if e.sureCrossed].count(3)
    
    def get_e4(self):
        """Get the number of edges crossed 3x on the boundary of the core"""
        return [e.CR() for e in self.E if e.sureCrossed].count(3)

    def count_arms(self, T_left, T_right):
        """Get the number of T_left - t0_c4tri - T_right trails"""
        # First we scan form arms
        arms = list()
        for i in range(5):
            if self.bracelet[i] == 2:
                a = self.bracelet[(i-1) % 5] 
                b = self.bracelet[(i+1) % 5]
                if len({5,6}.intersection({a,b})) == 0: 
                    arms.append(tuple(sorted([a,b])))
        # Return matching arms
        return arms.count((T_left, T_right))
    
    def count_qs(self, T_left, T_right):
        """Get the number of T_left - t0_c4tri - T_right trails"""
        # TODO: this is a hack, come up with a cleaner way.
        if T_left == 9:
            arms = list()
            for i in range(5):
                if self.bracelet[i] == 3:
                    a = self.bracelet[(i-1) % 5] 
                    b = self.bracelet[(i+1) % 5]
                    if a != 0 or b != 0: 
                       continue
  
                    if self.E[(i-2) % 5].CR() == k_for_k_planar:
                        if self.E[(i+2) % 5].CR() == k_for_k_planar:
                            arms.append((9, 9))
            return arms.count((T_left, T_right))
        if T_right == 9: 
            # This is the special case when we want to count the t0_c3_tri-trails where
            # the opposing edge ends in a vertex, restricting the cell
            arms = list()
            for i in range(5):
                if self.bracelet[i] == 3:
                    a = self.bracelet[(i-1) % 5] 
                    b = self.bracelet[(i+1) % 5]
                    if len({5,6}.intersection({a,b})) != 0: 
                       continue
                    if a == 0:
                        if self.E[(i-2) % 5].CR() == k_for_k_planar:
                            arms.append((b, 9))
                    if b == 0:
                        if self.E[(i+2) % 5].CR() == k_for_k_planar:
                            arms.append((a, 9))
            return arms.count((T_left, T_right))
        # First we scan form arms
        arms = list()
        for i in range(5):
            if self.bracelet[i] == 3:
                a = self.bracelet[(i-1) % 5] 
                b = self.bracelet[(i+1) % 5]
                if len({5,6}.intersection({a,b})) != 0: 
                    continue
                if a == 0 and self.E[(i-2) % 5].CR() == k_for_k_planar:
                    continue ## this is picked up before
                if b == 0 and self.E[(i+2) % 5].CR() == k_for_k_planar:
                    continue # this as well
                arms.append(tuple(sorted([a,b])))
        # Return matching arms
        return arms.count((T_left, T_right))


CORNER_PATTERNS = {
    "c6_qua": ['v', 's', 'c', 's', 's', 'v'],
    "mysterycell": ['v', 's', 's', 'c', 's', 's', 'v'],
    "hook": ['v', 's', 'he', 's', 'c', 's', 's', 'v'],
    "cone": ['s', 's', 'c', 's', 's']
}

def replace_sequences(observed):
    """Replaces corner sequences with identifying keys for easier use"""
    exchanged = []
    for seq in observed:
        for key, val in CORNER_PATTERNS.items():
            if seq == val or list(reversed(seq)) == val:
                exchanged.append(key)
                break
        else:
            exchanged.append("(uncategorized)")
    return exchanged