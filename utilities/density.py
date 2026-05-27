from fractions import Fraction
from math import lcm

def get_density_constraint(tn:int, td:int)->str:
    """Here we just calculate the density formula-s coefficents automatically.
       We are lazy.
       tn is the Numerator of (t) expressed as a fraction
       td is the Denominator.
    """

    t = Fraction(tn,td)
    t = 6
    p = Fraction(t - 1, 4)

    def coeff(c):
        return p * c - t

    def best_lb(a):
        m = Fraction(1, p) * t
        b = 1 - Fraction(m, a)
        return p * b


    c3 = coeff(3)
    c4 = coeff(4)
    c5 = coeff(5)
    c6 = best_lb(6)

    coeffs = [f.denominator for f in (c3,c4,c5,c6)]
    x = lcm(*coeffs)

    it = (int) (t.numerator * x / t.denominator)
    i3 = (int) (c3.numerator * x / c3.denominator)
    i4 = (int) (c4.numerator * x / c4.denominator)
    i5 = (int) (c5.numerator * x / c5.denominator)
    i6 = (int) (c6.numerator * x / c6.denominator)

    print(f"{x} |E| <= {it} (|V| - 2) + {i3} |C3| + {i4} |C4| - {i5} |C5| - {i6} SumLarge")


    inject= f""" // 1.A -- density formula with t = {t}
                START_C("1.A / Density Formula")
                lp.set_a(SUM_LARGE, C, {i6}); lp.set_a(E, C, {x}); lp.set_a(X, C, {x}); 
                lp.set_a(c3_tri, C, {i3}); lp.set_a(c4_tri, C, {i4}); lp.set_a(c4_qua, C, {i4});
                lp.set_a(c5_pen, C, {i5}); lp.set_a(c5_qua, C, {i5}); lp.set_a(c5_tri, C, {i5});
                END_C({it})"""
    
    return inject

with open("../include4/df.inc", "w+") as f: 
    f.write(get_density_constraint(6,1))