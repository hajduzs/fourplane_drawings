  START_C("4. -- c3_tri _LEQ")
    lp.set_a(t0_c3_tri_c5_pen, C, 1);
    lp.set_a(t0_c3_tri_c5_qua, C, 1);
    lp.set_a(t0_c3_tri_large, C, 1);
    lp.set_a(t1_c3_tri_c5_pen, C, 1);
    lp.set_a(t1_c3_tri_c5_qua, C, 1);
    lp.set_a(t1_c3_tri_large, C, 1);
    lp.set_a(t2_c3_tri_large, C, 1);
    lp.set_a(c3_tri, C, -3);
    END_C(0)

  START_C("4. -- c3_tri _GEQ")
    lp.set_a(t0_c3_tri_c5_pen, C, -1);
    lp.set_a(t0_c3_tri_c5_qua, C, -1);
    lp.set_a(t0_c3_tri_large, C, -1);
    lp.set_a(t1_c3_tri_c5_pen, C, -1);
    lp.set_a(t1_c3_tri_c5_qua, C, -1);
    lp.set_a(t1_c3_tri_large, C, -1);
    lp.set_a(t2_c3_tri_large, C, -1);
    lp.set_a(c3_tri, C, 3);
    END_C(0)


  START_C("4. -- c4_tri _LEQ")
    lp.set_a(t0_c4_tri_c5_pen, C, 1);
    lp.set_a(t0_c4_tri_c5_qua, C, 1);
    lp.set_a(t0_c4_tri_large, C, 1);
    lp.set_a(t1_c4_tri_c5_pen, C, 1);
    lp.set_a(t1_c4_tri_c5_qua, C, 1);
    lp.set_a(t1_c4_tri_large, C, 1);
    lp.set_a(t2_c4_tri_c5_pen, C, 1);
    lp.set_a(t2_c4_tri_c5_qua, C, 1);
    lp.set_a(t2_c4_tri_large, C, 1);
    lp.set_a(t3_c4_tri_large, C, 1);
    lp.set_a(c4_tri, C, -1);
    END_C(0)

  START_C("4. -- c4_tri _GEQ")
    lp.set_a(t0_c4_tri_c5_pen, C, -1);
    lp.set_a(t0_c4_tri_c5_qua, C, -1);
    lp.set_a(t0_c4_tri_large, C, -1);
    lp.set_a(t1_c4_tri_c5_pen, C, -1);
    lp.set_a(t1_c4_tri_c5_qua, C, -1);
    lp.set_a(t1_c4_tri_large, C, -1);
    lp.set_a(t2_c4_tri_c5_pen, C, -1);
    lp.set_a(t2_c4_tri_c5_qua, C, -1);
    lp.set_a(t2_c4_tri_large, C, -1);
    lp.set_a(t3_c4_tri_large, C, -1);
    lp.set_a(c4_tri, C, 1);
    END_C(0)




  START_C("4. -- c5_qua _LEQ")
    lp.set_a(t0_c3_tri_c5_qua, C, 1);
    lp.set_a(t0_c4_tri_c5_qua, C, 1);
    lp.set_a(t0_c5_pen_c5_qua, C, 1);
    lp.set_a(t0_c5_qua_c5_qua, C, 2);
    lp.set_a(t0_c5_qua_large, C, 1);
    lp.set_a(t1_c3_tri_c5_qua, C, 1);
    lp.set_a(t1_c4_tri_c5_qua, C, 1);
    lp.set_a(t1_c5_pen_c5_qua, C, 1);
    lp.set_a(t1_c5_qua_c5_qua, C, 2);
    lp.set_a(t1_c5_qua_large, C, 1);
    lp.set_a(t2_c4_tri_c5_qua, C, 1);
    lp.set_a(t2_c5_qua_c5_qua, C, 2);
    lp.set_a(t2_c5_qua_large, C, 1);
    lp.set_a(c5_qua, C, -2);
    END_C(0)

  START_C("4. -- c5_qua _GEQ")
    lp.set_a(t0_c3_tri_c5_qua, C, -1);
    lp.set_a(t0_c4_tri_c5_qua, C, -1);
    lp.set_a(t0_c5_pen_c5_qua, C, -1);
    lp.set_a(t0_c5_qua_c5_qua, C, -2);
    lp.set_a(t0_c5_qua_large, C, -1);
    lp.set_a(t1_c3_tri_c5_qua, C, -1);
    lp.set_a(t1_c4_tri_c5_qua, C, -1);
    lp.set_a(t1_c5_pen_c5_qua, C, -1);
    lp.set_a(t1_c5_qua_c5_qua, C, -2);
    lp.set_a(t1_c5_qua_large, C, -1);
    lp.set_a(t2_c4_tri_c5_qua, C, -1);
    lp.set_a(t2_c5_qua_c5_qua, C, -2);
    lp.set_a(t2_c5_qua_large, C, -1);
    lp.set_a(c5_qua, C, 2);
    END_C(0)


  START_C("4. -- c5_pen _LEQ")
    lp.set_a(t0_c3_tri_c5_pen, C, 1);
    lp.set_a(t0_c4_tri_c5_pen, C, 1);
    lp.set_a(t0_c5_pen_c5_pen, C, 2);
    lp.set_a(t0_c5_pen_c5_qua, C, 1);
    lp.set_a(t0_c5_pen_large, C, 1);
    lp.set_a(t1_c3_tri_c5_pen, C, 1);
    lp.set_a(t1_c4_tri_c5_pen, C, 1);
    lp.set_a(t1_c5_pen_c5_pen, C, 2);
    lp.set_a(t1_c5_pen_c5_qua, C, 1);
    lp.set_a(t1_c5_pen_large, C, 1);
    lp.set_a(t2_c4_tri_c5_pen, C, 1);
    lp.set_a(t2_c5_pen_large, C, 1);
    lp.set_a(c5_pen, C, -5);
    END_C(0)

  START_C("4. -- c5_pen _GEQ")
    lp.set_a(t0_c3_tri_c5_pen, C, -1);
    lp.set_a(t0_c4_tri_c5_pen, C, -1);
    lp.set_a(t0_c5_pen_c5_pen, C, -2);
    lp.set_a(t0_c5_pen_c5_qua, C, -1);
    lp.set_a(t0_c5_pen_large, C, -1);
    lp.set_a(t1_c3_tri_c5_pen, C, -1);
    lp.set_a(t1_c4_tri_c5_pen, C, -1);
    lp.set_a(t1_c5_pen_c5_pen, C, -2);
    lp.set_a(t1_c5_pen_c5_qua, C, -1);
    lp.set_a(t1_c5_pen_large, C, -1);
    lp.set_a(t2_c4_tri_c5_pen, C, -1);
    lp.set_a(t2_c5_pen_large, C, -1);
    lp.set_a(c5_pen, C, 5);
    END_C(0)


  START_C("4. -- c4_qua_LEQ")
    lp.set_a(t1_c3_tri_c5_pen, C, 1);
    lp.set_a(t1_c3_tri_c5_qua, C, 1);
    lp.set_a(t1_c3_tri_large, C, 1);
    lp.set_a(t1_c4_tri_c5_pen, C, 1);
    lp.set_a(t1_c4_tri_c5_qua, C, 1);
    lp.set_a(t1_c4_tri_large, C, 1);
    lp.set_a(t1_c5_pen_c5_pen, C, 1);
    lp.set_a(t1_c5_pen_c5_qua, C, 1);
    lp.set_a(t1_c5_pen_large, C, 1);
    lp.set_a(t1_c5_qua_c5_qua, C, 1);
    lp.set_a(t1_c5_qua_large, C, 1);
    lp.set_a(t1_large_large, C, 1);
    lp.set_a(t2_c3_tri_large, C, 2);
    lp.set_a(t2_c4_tri_c5_pen, C, 2);
    lp.set_a(t2_c4_tri_c5_qua, C, 2);
    lp.set_a(t2_c4_tri_large, C, 2);
    lp.set_a(t2_c5_pen_large, C, 2);
    lp.set_a(t2_c5_qua_c5_qua, C, 2);
    lp.set_a(t2_c5_qua_large, C, 2);
    lp.set_a(t2_large_large, C, 2);
    lp.set_a(t3_c4_tri_large, C, 3);
    lp.set_a(t3_large_large, C, 3);
    lp.set_a(c4_qua, C, -2);
    END_C(0)

  START_C("4. -- c4_qua_GEQ")
    lp.set_a(t1_c3_tri_c5_pen, C, -1);
    lp.set_a(t1_c3_tri_c5_qua, C, -1);
    lp.set_a(t1_c3_tri_large, C, -1);
    lp.set_a(t1_c4_tri_c5_pen, C, -1);
    lp.set_a(t1_c4_tri_c5_qua, C, -1);
    lp.set_a(t1_c4_tri_large, C, -1);
    lp.set_a(t1_c5_pen_c5_pen, C, -1);
    lp.set_a(t1_c5_pen_c5_qua, C, -1);
    lp.set_a(t1_c5_pen_large, C, -1);
    lp.set_a(t1_c5_qua_c5_qua, C, -1);
    lp.set_a(t1_c5_qua_large, C, -1);
    lp.set_a(t1_large_large, C, -1);
    lp.set_a(t2_c3_tri_large, C, -2);
    lp.set_a(t2_c4_tri_c5_pen, C, -2);
    lp.set_a(t2_c4_tri_c5_qua, C, -2);
    lp.set_a(t2_c4_tri_large, C, -2);
    lp.set_a(t2_c5_pen_large, C, -2);
    lp.set_a(t2_c5_qua_c5_qua, C, -2);
    lp.set_a(t2_c5_qua_large, C, -2);
    lp.set_a(t2_large_large, C, -2);
    lp.set_a(t3_c4_tri_large, C, -3);
    lp.set_a(t3_large_large, C, -3);
    lp.set_a(c4_qua, C, 2);
    END_C(0)
