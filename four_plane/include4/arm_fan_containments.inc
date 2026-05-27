  const int AFC_0_2_F1_AB = VAR++; var_names.push_back("AFC_0_2_F1_AB");
  const int AFC_0_2_F1_AC = VAR++; var_names.push_back("AFC_0_2_F1_AC");
  START_C("A4 - AFC_0_2_LEQ")
    lp.set_a(AFC_0_2_F1_AB, C, 1);
    lp.set_a(AFC_0_2_F1_AC, C, 1);
    lp.set_a(ARM_0_2, C, -1);
    END_C(0)

  START_C("A4 - AFC_0_2_GEQ")
    lp.set_a(AFC_0_2_F1_AB, C, -1);
    lp.set_a(AFC_0_2_F1_AC, C, -1);
    lp.set_a(ARM_0_2, C, 1);
    END_C(0)
  const int AFC_0_3_F1_BB = VAR++; var_names.push_back("AFC_0_3_F1_BB");
  const int AFC_0_3_F1_BC = VAR++; var_names.push_back("AFC_0_3_F1_BC");
  const int AFC_0_3_F1_CC = VAR++; var_names.push_back("AFC_0_3_F1_CC");
  const int AFC_0_3_F2_AB = VAR++; var_names.push_back("AFC_0_3_F2_AB");
  const int AFC_0_3_F2_AC = VAR++; var_names.push_back("AFC_0_3_F2_AC");
  START_C("A4 - AFC_0_3_LEQ")
    lp.set_a(AFC_0_3_F1_BB, C, 1);
    lp.set_a(AFC_0_3_F1_BC, C, 1);
    lp.set_a(AFC_0_3_F1_CC, C, 1);
    lp.set_a(AFC_0_3_F2_AB, C, 1);
    lp.set_a(AFC_0_3_F2_AC, C, 1);
    lp.set_a(ARM_0_3, C, -1);
    END_C(0)

  START_C("A4 - AFC_0_3_GEQ")
    lp.set_a(AFC_0_3_F1_BB, C, -1);
    lp.set_a(AFC_0_3_F1_BC, C, -1);
    lp.set_a(AFC_0_3_F1_CC, C, -1);
    lp.set_a(AFC_0_3_F2_AB, C, -1);
    lp.set_a(AFC_0_3_F2_AC, C, -1);
    lp.set_a(ARM_0_3, C, 1);
    END_C(0)
  const int AFC_1_2_F1_AB = VAR++; var_names.push_back("AFC_1_2_F1_AB");
  const int AFC_1_2_F1_AC = VAR++; var_names.push_back("AFC_1_2_F1_AC");
  const int AFC_1_2_F2_AB = VAR++; var_names.push_back("AFC_1_2_F2_AB");
  const int AFC_1_2_F2_AC = VAR++; var_names.push_back("AFC_1_2_F2_AC");
  START_C("A4 - AFC_1_2_LEQ")
    lp.set_a(AFC_1_2_F1_AB, C, 1);
    lp.set_a(AFC_1_2_F1_AC, C, 1);
    lp.set_a(AFC_1_2_F2_AB, C, 1);
    lp.set_a(AFC_1_2_F2_AC, C, 1);
    lp.set_a(ARM_1_2, C, -1);
    END_C(0)

  START_C("A4 - AFC_1_2_GEQ")
    lp.set_a(AFC_1_2_F1_AB, C, -1);
    lp.set_a(AFC_1_2_F1_AC, C, -1);
    lp.set_a(AFC_1_2_F2_AB, C, -1);
    lp.set_a(AFC_1_2_F2_AC, C, -1);
    lp.set_a(ARM_1_2, C, 1);
    END_C(0)
  const int AFC_2_2_F1_AA = VAR++; var_names.push_back("AFC_2_2_F1_AA");
  START_C("A4 - AFC_2_2_LEQ")
    lp.set_a(AFC_2_2_F1_AA, C, 1);
    lp.set_a(ARM_2_2, C, -1);
    END_C(0)

  START_C("A4 - AFC_2_2_GEQ")
    lp.set_a(AFC_2_2_F1_AA, C, -1);
    lp.set_a(ARM_2_2, C, 1);
    END_C(0)
  const int AFC_2_3_F1_AB = VAR++; var_names.push_back("AFC_2_3_F1_AB");
  const int AFC_2_3_F1_AC = VAR++; var_names.push_back("AFC_2_3_F1_AC");
  const int AFC_2_3_F2_AA = VAR++; var_names.push_back("AFC_2_3_F2_AA");
  START_C("A4 - AFC_2_3_LEQ")
    lp.set_a(AFC_2_3_F1_AB, C, 1);
    lp.set_a(AFC_2_3_F1_AC, C, 1);
    lp.set_a(AFC_2_3_F2_AA, C, 1);
    lp.set_a(ARM_2_3, C, -1);
    END_C(0)

  START_C("A4 - AFC_2_3_GEQ")
    lp.set_a(AFC_2_3_F1_AB, C, -1);
    lp.set_a(AFC_2_3_F1_AC, C, -1);
    lp.set_a(AFC_2_3_F2_AA, C, -1);
    lp.set_a(ARM_2_3, C, 1);
    END_C(0)
  const int AFC_2_4_F1_AB = VAR++; var_names.push_back("AFC_2_4_F1_AB");
  const int AFC_2_4_F1_AC = VAR++; var_names.push_back("AFC_2_4_F1_AC");
  const int AFC_2_4_F2_AB = VAR++; var_names.push_back("AFC_2_4_F2_AB");
  const int AFC_2_4_F2_AC = VAR++; var_names.push_back("AFC_2_4_F2_AC");
  const int AFC_2_4_F3_AA = VAR++; var_names.push_back("AFC_2_4_F3_AA");
  START_C("A4 - AFC_2_4_LEQ")
    lp.set_a(AFC_2_4_F1_AB, C, 1);
    lp.set_a(AFC_2_4_F1_AC, C, 1);
    lp.set_a(AFC_2_4_F2_AB, C, 1);
    lp.set_a(AFC_2_4_F2_AC, C, 1);
    lp.set_a(AFC_2_4_F3_AA, C, 1);
    lp.set_a(ARM_2_4, C, -1);
    END_C(0)

  START_C("A4 - AFC_2_4_GEQ")
    lp.set_a(AFC_2_4_F1_AB, C, -1);
    lp.set_a(AFC_2_4_F1_AC, C, -1);
    lp.set_a(AFC_2_4_F2_AB, C, -1);
    lp.set_a(AFC_2_4_F2_AC, C, -1);
    lp.set_a(AFC_2_4_F3_AA, C, -1);
    lp.set_a(ARM_2_4, C, 1);
    END_C(0)
  const int AFC_3_3_F1_BB = VAR++; var_names.push_back("AFC_3_3_F1_BB");
  const int AFC_3_3_F1_BC = VAR++; var_names.push_back("AFC_3_3_F1_BC");
  const int AFC_3_3_F1_CC = VAR++; var_names.push_back("AFC_3_3_F1_CC");
  const int AFC_3_3_F2_AB = VAR++; var_names.push_back("AFC_3_3_F2_AB");
  const int AFC_3_3_F2_AC = VAR++; var_names.push_back("AFC_3_3_F2_AC");
  const int AFC_3_3_F3_AA = VAR++; var_names.push_back("AFC_3_3_F3_AA");
  START_C("A4 - AFC_3_3_LEQ")
    lp.set_a(AFC_3_3_F1_BB, C, 1);
    lp.set_a(AFC_3_3_F1_BC, C, 1);
    lp.set_a(AFC_3_3_F1_CC, C, 1);
    lp.set_a(AFC_3_3_F2_AB, C, 1);
    lp.set_a(AFC_3_3_F2_AC, C, 1);
    lp.set_a(AFC_3_3_F3_AA, C, 1);
    lp.set_a(ARM_3_3, C, -1);
    END_C(0)

  START_C("A4 - AFC_3_3_GEQ")
    lp.set_a(AFC_3_3_F1_BB, C, -1);
    lp.set_a(AFC_3_3_F1_BC, C, -1);
    lp.set_a(AFC_3_3_F1_CC, C, -1);
    lp.set_a(AFC_3_3_F2_AB, C, -1);
    lp.set_a(AFC_3_3_F2_AC, C, -1);
    lp.set_a(AFC_3_3_F3_AA, C, -1);
    lp.set_a(ARM_3_3, C, 1);
    END_C(0)
  START_C("A5 - fans in arms - F1_AA_LEQ")
    lp.set_a(AFC_2_2_F1_AA, C, 1);
    lp.set_a(F1_AA, C, -1);
    END_C(0)
  START_C("A5 - fans in arms - F1_AB_LEQ")
    lp.set_a(AFC_0_2_F1_AB, C, 1);
    lp.set_a(AFC_1_2_F1_AB, C, 1);
    lp.set_a(AFC_2_3_F1_AB, C, 1);
    lp.set_a(AFC_2_4_F1_AB, C, 1);
    lp.set_a(F1_AB, C, -1);
    END_C(0)
  START_C("A5 - fans in arms - F1_AC_LEQ")
    lp.set_a(AFC_0_2_F1_AC, C, 1);
    lp.set_a(AFC_1_2_F1_AC, C, 1);
    lp.set_a(AFC_2_3_F1_AC, C, 1);
    lp.set_a(AFC_2_4_F1_AC, C, 1);
    lp.set_a(F1_AC, C, -1);
    END_C(0)
  START_C("A5 - fans in arms - F1_BB_LEQ")
    lp.set_a(AFC_0_3_F1_BB, C, 1);
    lp.set_a(AFC_3_3_F1_BB, C, 1);
    lp.set_a(F1_BB, C, -1);
    END_C(0)
  START_C("A5 - fans in arms - F1_BC_LEQ")
    lp.set_a(AFC_0_3_F1_BC, C, 1);
    lp.set_a(AFC_3_3_F1_BC, C, 1);
    lp.set_a(F1_BC, C, -1);
    END_C(0)
  START_C("A5 - fans in arms - F1_CC_LEQ")
    lp.set_a(AFC_0_3_F1_CC, C, 1);
    lp.set_a(AFC_3_3_F1_CC, C, 1);
    lp.set_a(F1_CC, C, -1);
    END_C(0)
  START_C("A5 - fans in arms - F2_AA_LEQ")
    lp.set_a(AFC_2_3_F2_AA, C, 1);
    lp.set_a(F2_AA, C, -1);
    END_C(0)
  START_C("A5 - fans in arms - F2_AB_LEQ")
    lp.set_a(AFC_0_3_F2_AB, C, 1);
    lp.set_a(AFC_1_2_F2_AB, C, 1);
    lp.set_a(AFC_2_4_F2_AB, C, 1);
    lp.set_a(AFC_3_3_F2_AB, C, 1);
    lp.set_a(F2_AB, C, -1);
    END_C(0)
  START_C("A5 - fans in arms - F2_AC_LEQ")
    lp.set_a(AFC_0_3_F2_AC, C, 1);
    lp.set_a(AFC_1_2_F2_AC, C, 1);
    lp.set_a(AFC_2_4_F2_AC, C, 1);
    lp.set_a(AFC_3_3_F2_AC, C, 1);
    lp.set_a(F2_AC, C, -1);
    END_C(0)
  START_C("A5 - fans in arms - F3_AA_LEQ")
    lp.set_a(AFC_2_4_F3_AA, C, 1);
    lp.set_a(AFC_3_3_F3_AA, C, 1);
    lp.set_a(F3_AA, C, -1);
    END_C(0)
