 // 1.A -- density formula with t = 6
                lp.set_a(SUM_LARGE, C, 1); lp.set_a(E, C, 4); lp.set_a(X, C, 4); 
                lp.set_a(c3_tri, C, -9); lp.set_a(c4_tri, C, -4); lp.set_a(c4_qua, C, -4);
                lp.set_a(c5_pen, C, 1); lp.set_a(c5_qua, C, 1); lp.set_a(c5_tri, C, 1);
                lp.set_b(C++, 24);
                constr_names.push_back("1.A / Density Formula");