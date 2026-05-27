INNER_SEGMENTS = {
    "c3_tri": 3,
    "c4_tri": 1,
    "c4_qua": 4,
    "c5_qua": 2,
    "c5_pen": 5
}

def write_vardef(varname, f):
    f.write(f'  const int {varname} = VAR++; var_names.push_back("{varname}");\n')

def write_equality(var_value_list, constrname, f):
    write_constraint(var_value_list, False, constrname, f)
    f.write("\n")
    write_constraint(var_value_list, True, constrname, f)

def write_constraint(var_value_list, flipsign, constrname, f):
    c = -1 if flipsign else 1
    sign_str = "GEQ" if flipsign else "LEQ"
    full_name = f"{constrname}_{sign_str}"
    f.write(f'  START_C("{full_name}")\n')
    for var, value in var_value_list:
        f.write(f"    lp.set_a({var}, C, {int(c*value)});\n")
    f.write(f"    END_C(0)\n")

def count_cells_by_trails(valid_trails, cell, constrname, f):
    trails_containing_cell = [(str(T), T.countcell(cell)) for T in valid_trails if T.countcell(cell)]
    if len(trails_containing_cell) == 0:
        return
    trails_containing_cell.append((cell, -1* INNER_SEGMENTS[cell]))
    
    write_equality(trails_containing_cell, constrname, f)


