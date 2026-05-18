#include <iostream>
#include <cassert>
#include <vector>
 
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
 
// choose exact integral type
#include <CGAL/Gmpz.h>
typedef CGAL::Gmpz ET;
 
// program and solution types
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


void save_var_to_file(const std::vector<std::string>& var, const std::string& filename){
    std::ofstream out(filename);

    out << "[";
    for (size_t i = 0; i < var.size(); ++i) {
        out << "\"" << var[i] << "\"";
        if (i != var.size() - 1) {
            out << ", ";
        }
    }
    out << "]";
}

void save_c_to_file(const Solution& s, const std::string& filename) {
    std::ofstream out(filename);
    
    out << "[";
    
    auto it = s.optimality_certificate_begin();
    auto end = s.optimality_certificate_end();
    
    bool first = true;
    for (; it != end; ++it) {
        if (!first) {
            out << ","; 
        }
        out << "fr(" << it->numerator() << "," << it->denominator() << ")";
        first = false;
    }
    out << "]" << std::endl;
}

void save_matrix_to_file(const Program& lp, const std::string& filename) {
    std::ofstream out(filename);
    
    int m = lp.get_m(); 
    int n = lp.get_n();

    out << "[";
    for (int i = 0; i < m; ++i) { //constraints
        out << "[";
        for (int j = 0; j < n; ++j) { // variables
            auto it = (*(lp.get_a()+j))+i; 
            out << (int) *it;
            if (j < n - 1) out << ", ";
        }
        out << "]";
        if (i < m - 1) out << ",\n";
    }
    out << "]";
    out.close();
}

void save_primal_to_file(const Solution& s, const std::string& filename) {
    std::ofstream out(filename);
    if (!out.is_open()) {
        std::cerr << "Error: Could not write primal to " << filename << "\n";
        return;
    }
    
    out << "[";
    auto it = s.variable_values_begin();
    auto end = s.variable_values_end();
    
    bool first = true;
    for (; it != end; ++it) {
        if (!first) out << ","; 
        out << "fr(" << it->numerator() << "," << it->denominator() << ")";
        first = false;
    }
    out << "]\n";
}
 
int main() {
  
    Program lp (CGAL::SMALLER, true, 0, false, 0);
    // Variable counter, and names. 
    int VAR = 0; std::vector<std::string> var_names;
    // inequality counter and names for easier debugging
    int C = 0;   std::vector<std::string> constr_names;

    // -------------------------
    // ------ Variables --------
    // -------------------------

    // Edges, Crossings 
    const int E       = VAR++; var_names.push_back("E");
    const int E_0     = VAR++; var_names.push_back("E_0");
    const int E_1     = VAR++; var_names.push_back("E_1");
    const int E_2     = VAR++; var_names.push_back("E_2");
    const int E_3     = VAR++; var_names.push_back("E_3"); 
    const int E_4     = VAR++; var_names.push_back("E_4");
    const int E_x     = VAR++; var_names.push_back("E_x");
    const int X       = VAR++; var_names.push_back("X");

    // Singular cells 
    const int c3_tri  = VAR++; var_names.push_back("c3_tri");
    const int c4_qua  = VAR++; var_names.push_back("c4_qua");
    const int c4_tri  = VAR++; var_names.push_back("c4_tri");
    const int c5_pen  = VAR++; var_names.push_back("c5_pen");
    const int c5_qua  = VAR++; var_names.push_back("c5_qua");
    const int c5_tri  = VAR++; var_names.push_back("c5_tri");
    const int c6_qua  = VAR++; var_names.push_back("c6_qua");
    const int c7_pen  = VAR++; var_names.push_back("c7_pen");

    // Cell size sums
    const int SUM_LARGE = VAR++; var_names.push_back("SUM_LARGE");
    const int LC_INCIDENCE = VAR++; var_names.push_back("LC_INCIDENCE");    // large cell incidences along outer edge segments, not along specified large cells.

    // ##############################
    // Trail variable definitions
    #include "include4/traildefs.inc"
    // ##############################

    // ##############################
    // STAR definitions
    #include "include4/stardefs.inc"
    const int SUM_STAR  = VAR++; var_names.push_back("SUM_STAR");
    // ##############################

    // ##############################
    // FAN and ARM variable definitions
    #include "include4/arm_fan_definitions.inc"
    // ANTENNAS (named as Q_ something to be easier to read)
    #include "include4/q_definitions.inc"
    // ##############################

    // ##############################
    // c5qua/c7pen cells around stars
    const int mysterycell = VAR++; var_names.push_back("mysterycell");
    const int MC_5_qua = VAR++; var_names.push_back("MC_5_qua");
    const int MC_7_pen = VAR++; var_names.push_back("MC_7_pen");
    // ##############################
    
    // ##############################
    // Special trails "extensions" obtained alongside outer edge segments 
    const int T4_1_empty = VAR++; var_names.push_back("T4_1_empty");
    const int T4_1_c4tri = VAR++; var_names.push_back("T4_1_c4tri");
    const int T4_1_c5qua = VAR++; var_names.push_back("T4_1_c5qua");
    const int T4_1_large = VAR++; var_names.push_back("T4_1_large");

    const int T3_0_empty = VAR++; var_names.push_back("T3_0_empty");
    const int T3_0_c4tri = VAR++; var_names.push_back("T3_0_c4tri");
    const int T3_0_c5qua = VAR++; var_names.push_back("T3_0_c5qua");
    const int T3_0_large = VAR++; var_names.push_back("T3_0_large");

    const int T5_0_empty = VAR++; var_names.push_back("T5_0_empty");
    const int T5_0_c4tri = VAR++; var_names.push_back("T5_0_c4tri");
    const int T5_0_c5qua = VAR++; var_names.push_back("T5_0_c5qua");
    const int T5_0_large = VAR++; var_names.push_back("T5_0_large");

    const int T3_1_c4tri = VAR++; var_names.push_back("T3_1_c4tri");
    const int T4_2_c4tri = VAR++; var_names.push_back("T4_2_c4tri");
    const int T5_1_c4tri = VAR++; var_names.push_back("T5_1_c4tri");
    // ##############################

    /// NEW STUFF TO TEST and iron out the system
    const int E_zero_large = VAR++; var_names.push_back("E_zero_large");



    // -----------------------------------
    // ------ Constraints --------
    // -----------------------------------

    // 1.A -- density formula with t = 6
    lp.set_a(SUM_LARGE, C, 1); lp.set_a(E, C, 4); lp.set_a(X, C, 4); 
    lp.set_a(c3_tri, C, -9); lp.set_a(c4_tri, C, -4); lp.set_a(c4_qua, C, -4);
    lp.set_a(c5_pen, C, 1); lp.set_a(c5_qua, C, 1); lp.set_a(c5_tri, C, 1);
    lp.set_b(C++, 24);
    constr_names.push_back("1.A / Density Formula");

    // 1.B -- lower bounding the number of large cells
    lp.set_a(t3_c4_tri_large, C, 1); lp.set_a(t3_large_large, C, 2); 
    lp.set_a(t2_c3_tri_large, C, 1); lp.set_a(t2_c4_tri_large, C, 1); lp.set_a(t2_c5_qua_large, C, 1); lp.set_a(t2_c5_pen_large, C, 1); lp.set_a(t2_large_large, C, 2); 
    lp.set_a(t1_c3_tri_large, C, 1); lp.set_a(t1_c4_tri_large, C, 1); lp.set_a(t1_c5_qua_large, C, 1); lp.set_a(t1_c5_pen_large, C, 1); lp.set_a(t0_large_large, C, 2); 
    lp.set_a(t0_c3_tri_large, C, 1); lp.set_a(t0_c4_tri_large, C, 1); lp.set_a(t0_c5_qua_large, C, 1); lp.set_a(t0_c5_pen_large, C, 1); lp.set_a(t1_large_large, C, 2); 
    //lp.set_a(c6_qua, C, 3); lp.set_a(c7_pen, C, 3); 
    lp.set_a(c6_qua, C, 2); lp.set_a(c7_pen, C, 2); 
    lp.set_a(E_zero_large, C, 1);
    lp.set_a(LC_INCIDENCE, C, 1);
    lp.set_a(SUM_LARGE, C, -1);
    lp.set_b(C++, 0);
    constr_names.push_back("1.B / Large_cell_LB");

    // 2.A -- counting edges, crossed and uncrossed 
    lp.set_a(E_0, C, 1); lp.set_a(E_x, C, 1); 
    lp.set_a(E, C, -1);
    lp.set_b(C++, 0);
    constr_names.push_back("2.A = EO EX");
    
    lp.set_a(E, C, 1);
    lp.set_a(E_0, C, -1); lp.set_a(E_x, C, -1); 
    lp.set_b(C++, 0);
    constr_names.push_back("2.A = EO EX");

    // 2.B -- counting crossed edges
    lp.set_a(E_1, C, 1); lp.set_a(E_2, C, 1); lp.set_a(E_3, C, 1); lp.set_a(E_4, C, 1);
    lp.set_a(E_x, C, -1);
    lp.set_b(C++, 0);
    constr_names.push_back("2.B = EX E1->4");

    lp.set_a(E_x, C, 1);
    lp.set_a(E_1, C, -1); lp.set_a(E_2, C, -1); lp.set_a(E_3, C, -1); lp.set_a(E_4, C, -1); 
    lp.set_b(C++, 0);
    constr_names.push_back("2.B = EX E1->4");

    // 2.C  -- counting crossings
    lp.set_a(E_1, C, 1); lp.set_a(E_2, C, 2); lp.set_a(E_3, C, 3); lp.set_a(E_4, C, 4); 
    lp.set_a(X, C, -2);
    lp.set_b(C++, 0);
    constr_names.push_back("2.C = EX X");
    
    lp.set_a(X, C, 2);
    lp.set_a(E_1, C, -1); lp.set_a(E_2, C, -2); lp.set_a(E_3, C, -3); lp.set_a(E_4, C, -4); 
    lp.set_b(C++, 0);
    constr_names.push_back("2.C = EX X");

    // 3.A -- uncrossed edges
    //lp.set_a(c5_tri, C, 1); lp.set_a(c6_qua, C, 1); lp.set_a(c7_pen, C, 1); 
    lp.set_a(c5_tri, C, 1); lp.set_a(E_zero_large, C, 1);  
    lp.set_a(E_0, C, -2);
    lp.set_b(C++, 0);
    constr_names.push_back("3.A cell->E_0");

    lp.set_a(c5_tri, C, -1); lp.set_a(E_zero_large, C, -1);  
    lp.set_a(E_0, C, 2);
    lp.set_b(C++, 0);
    constr_names.push_back("3.A cell->E_0");
    
    // 3.B  -- outer edge segments
    lp.set_a(c4_tri, C, 2); lp.set_a(c5_qua, C, 2); lp.set_a(c5_tri, C, 2); lp.set_a(c6_qua, C, 2); lp.set_a(c7_pen, C, 2);
    lp.set_a(E_x, C, -4); 
    lp.set_b(C++, 0);
    constr_names.push_back("3.B cell->E_outer");

    // 3.C -- inner edge segments 
    lp.set_a(c3_tri, C, 3); lp.set_a(c4_qua, C, 4); lp.set_a(c4_tri, C ,1); lp.set_a(c5_qua, C, 2); lp.set_a(c5_pen, C, 5); lp.set_a(c6_qua, C, 1); lp.set_a(c7_pen, C, 2);
    lp.set_a(t3_c4_tri_large, C, 1); lp.set_a(t3_large_large, C, 2); 
    lp.set_a(t2_c3_tri_large, C, 1); lp.set_a(t2_c4_tri_large, C, 1); lp.set_a(t2_c5_qua_large, C, 1); lp.set_a(t2_c5_pen_large, C, 1); lp.set_a(t2_large_large, C, 2); 
    lp.set_a(t1_c3_tri_large, C, 1); lp.set_a(t1_c4_tri_large, C, 1); lp.set_a(t1_c5_qua_large, C, 1); lp.set_a(t1_c5_pen_large, C, 1); lp.set_a(t0_large_large, C, 2); 
    lp.set_a(t0_c3_tri_large, C, 1); lp.set_a(t0_c4_tri_large, C, 1); lp.set_a(t0_c5_qua_large, C, 1); lp.set_a(t0_c5_pen_large, C, 1); lp.set_a(t1_large_large, C, 2); 
    lp.set_a(E_2, C, -2); lp.set_a(E_3, C, -4); lp.set_a(E_4, C, -6);
    lp.set_b(C++, 0);
   constr_names.push_back("3.C cell->E_inner"); 

    // ##############################
    // 4.A-E  -- Trail cell counting
    #include "include4/trailcounts.inc"
    // ##############################

    // ##############################
    // 5.A  -- Fan cell counting
    #include "include4/fan_c4_cells.inc"
    // ##############################

    // 6.A 
    lp.set_a(T3_0_empty, C, 1); lp.set_a(T3_0_c4tri, C , 1); lp.set_a(T3_0_c5qua, C , 1); lp.set_a(T3_0_large, C , 1);
    lp.set_a(t0_c3_tri_c5_qua, C, -1);
    lp.set_b(C++, 0);
    constr_names.push_back("6.A - T3 cumstom conf "); 

    lp.set_a(t0_c3_tri_c5_qua, C, 1);
    lp.set_a(T3_0_empty, C ,-1); lp.set_a(T3_0_c4tri, C ,-1); lp.set_a(T3_0_c5qua, C ,-1); lp.set_a(T3_0_large, C ,-1);
    lp.set_b(C++, 0);
    constr_names.push_back("6.A - T3 cumstom conf "); 

    // 6.B
    lp.set_a(T4_1_empty, C, 1); lp.set_a(T4_1_c4tri, C , 1); lp.set_a(T4_1_c5qua, C , 1); lp.set_a(T4_1_large, C , 1);
    lp.set_a(t1_c4_tri_c5_qua, C, -1);
    lp.set_b(C++, 0);
    constr_names.push_back("6.b - T4 cumstom conf "); 

    lp.set_a(t1_c4_tri_c5_qua, C, 1);
    lp.set_a(T4_1_empty, C ,-1); lp.set_a(T4_1_c4tri, C ,-1); lp.set_a(T4_1_c5qua, C ,-1); lp.set_a(T4_1_large, C ,-1);
    lp.set_b(C++, 0);
    constr_names.push_back("6.b - T4 cumstom conf "); 

    // 6.C
    lp.set_a(T5_0_empty, C, 1); lp.set_a(T5_0_c4tri, C , 1); lp.set_a(T5_0_c5qua, C , 1); lp.set_a(T5_0_large, C , 1);
    lp.set_a(t0_c5_pen_c5_qua, C, -1);
    lp.set_b(C++, 0);
    constr_names.push_back("6.C - T5 cumstom conf "); 

    lp.set_a(t0_c5_pen_c5_qua, C, 1);
    lp.set_a(T5_0_empty, C ,-1); lp.set_a(T5_0_c4tri, C ,-1); lp.set_a(T5_0_c5qua, C ,-1); lp.set_a(T5_0_large, C ,-1);
    lp.set_b(C++, 0);
    constr_names.push_back("6.C - T5 cumstom conf "); 

    // 6. D
    lp.set_a(T3_0_c4tri, C, 1); lp.set_a(T4_1_c4tri, C, 1); lp.set_a(T5_0_c4tri, C, 1);
    lp.set_a(F1_AB, C, -1);
    lp.set_b(C++, 0);
    constr_names.push_back("6.D - fans in custom confs "); 

    // 6.E
    lp.set_a(T3_0_c5qua, C, 1);
    lp.set_a(t0_c4_tri_c5_qua, C, -1); lp.set_a(t0_c5_qua_c5_qua, C, -1); lp.set_a(t0_c5_qua_large, C, -1);
    lp.set_b(C++, 0);
    constr_names.push_back("6.E - trails in custom confs "); 

    // 7.A
    lp.set_a(t1_c3_tri_c5_qua, C, -1); lp.set_a(T3_1_c4tri, C, 1); 
    lp.set_a(t1_c3_tri_c5_qua, C, 1); lp.set_a(T3_1_c4tri, C, -1); 
    lp.set_b(C++, 0);
    constr_names.push_back("7.A1 - what are even these "); 

    // 7.A
    lp.set_a(t2_c4_tri_c5_qua, C, -1); lp.set_a(T4_2_c4tri, C, 1); 
    lp.set_a(t2_c4_tri_c5_qua, C, 1); lp.set_a(T4_2_c4tri, C, -1); 
    lp.set_b(C++, 0);
    constr_names.push_back("7.A2 - what are even these "); 

    // 7.A
    lp.set_a(t1_c5_pen_c5_qua, C, -1); lp.set_a(T5_1_c4tri, C, 1); 
    lp.set_a(t1_c5_pen_c5_qua, C, 1); lp.set_a(T5_1_c4tri, C, -1); 
    lp.set_b(C++, 0);
    constr_names.push_back("7.A3 - what are even these "); 
    

    // ##############################
    // 8.A
    #include "include4/star_sum_equality.inc"       
    // ##############################

    // 8.B 
    lp.set_a(c5_pen, C, 1);
    lp.set_a(t2_c5_pen_large, C, -1); 
    lp.set_a(t1_c5_pen_large, C, -1);
    lp.set_a(t1_c5_pen_c5_qua, C, -1); 
    lp.set_a(t0_c5_pen_large, C, -1);   
    lp.set_a(t0_c5_pen_c5_qua, C, -1); 
    lp.set_a(SUM_STAR, C, -1);
    lp.set_b(C++, 0);
    constr_names.push_back("8.B - nonstar bound"); 

    // 9.A
    lp.set_a(t0_c4_tri_c5_qua, C, 1);
    lp.set_a(E_1, C, -2);
    lp.set_b(C++, 0);
    constr_names.push_back("9.A - Edges "); 

    // ##############################
    // 9.B-C
    #include "include4/edge_sums.inc"
    // ##############################


    // 10.A
    lp.set_a(T3_1_c4tri, C, 1); 
    lp.set_a(T4_2_c4tri, C, 1); 
    lp.set_a(T5_1_c4tri, C, 1);
    
    lp.set_a(T3_0_empty, C, 1); lp.set_a(T4_1_empty, C, 1); lp.set_a(T5_0_empty, C, 1);
    
    lp.set_a(F1_AA, C, 2); lp.set_a(F2_AA, C, 2); lp.set_a(F3_AA, C, 2);
    lp.set_a(F2_AB, C, 1); lp.set_a(F2_AC, C, 1);
    lp.set_a(F1_AB, C, 1); lp.set_a(F1_AC, C, 1);
    lp.set_a(c5_tri, C, -2);
    lp.set_b(C++, 0); 

    constr_names.push_back("10.A - c5_tri incid "); 

    // 10.B
    lp.set_a(T3_0_c5qua, C, 1); lp.set_a(T4_1_c5qua, C, 1); lp.set_a(T5_0_c5qua, C, 1);
    lp.set_a(F1_AB, C, 1); lp.set_a(F2_AB, C, 1);
    lp.set_a(F1_BC, C, 1); lp.set_a(F1_BB, C, 2);
    lp.set_a(c5_qua, C, -2);
    lp.set_b(C++, 0); 
    constr_names.push_back("10.B - c5_qua incid "); 

    // 10.C
    lp.set_a(T3_0_large, C, 1); lp.set_a(T4_1_large, C, 1); lp.set_a(T5_0_large, C, 1);
    lp.set_a(F1_CC, C, 2);
    lp.set_a(F1_BC, C, 1);
    lp.set_a(F1_AC, C, 1);
    lp.set_a(F2_AC, C, 1);
    lp.set_a(LC_INCIDENCE, C, -1);                                      
    lp.set_b(C++, 0);
    constr_names.push_back("10.C - LC incid "); 



    // ##############################
    // A.1.1-7
    #include "include4/star_sum_lbs.inc"            
    // ##############################

    // ##############################
    // A.2.1-7
    #include "include4/star_arms.inc"               
    // ##############################

    // ##############################
    // A.2.8-11
    #include "include4/star_qs.inc"
    // ##############################

    // ##############################
    // A.3.1
    #include "include4/star_corners.inc"
    // ##############################

    // A.3.2
    lp.set_a(MC_5_qua, C, 1); lp.set_a(MC_7_pen, C, 1);
    lp.set_a(mysterycell, C, -1); 
    lp.set_b(C++, 0);
    constr_names.push_back("A3.2 - mystery 1 "); 

    lp.set_a(mysterycell, C, 1);
    lp.set_a(MC_5_qua, C, -1); lp.set_a(MC_7_pen, C, -1);
    lp.set_b(C++, 0);
    constr_names.push_back("A3.2 - mystery 1 "); 

    lp.set_a(MC_5_qua, C, 2);
    lp.set_a(t0_c3_tri_c5_qua, C, -1);
    lp.set_b(C++, 0);
    constr_names.push_back("A3.2 - mystery 2 "); 

    lp.set_a(MC_7_pen, C, 1);
    lp.set_a(c7_pen, C, -1);
    lp.set_b(C++, 0);
    constr_names.push_back("A3.2 - mystery 2 "); 


    // ##############################
    // A.4.1-7
    // A.5.1-10
    #include "include4/arm_fan_containments.inc"
    // ##############################

    // ##############################
    // A.6.1-4
    // A.7.1-x 
     #include "include4/Q_Trail_containments.inc"
    // ##############################

    // ------------------------------------
    // ----- Playground for testing -------
    // ------------------------------------

    // #include "_testing/turn_off_the_sky.inc"
    // this one breaks the whole thing lol 

    //lp.set_a(SUM_LARGE, C, 1);
    //lp.set_b(C++, 0);
    // this one does nothing. 

    /*
    lp.set_a(STAR_83, C, 1);
    lp.set_a(STAR_88, C, 1);
    lp.set_a(STAR_94, C, 1);
    lp.set_a(STAR_165, C, 1);
    lp.set_a(STAR_23, C, 1);
    lp.set_a(STAR_27, C, 1);
    lp.set_a(STAR_8, C, 1);
    lp.set_a(STAR_3, C, 1);
    // --- this is the point where we get to with Q_trail_containmnets
    lp.set_a(STAR_1, C, 1);
    lp.set_a(STAR_17, C, 1);
    lp.set_a(STAR_25, C, 1);
    // --- and this is the point where it breaks again. (no longer! it broke when the E_0 safeguard was not set up)
    lp.set_b(C++, 0);
    constr_names.push_back("test "); 
    
    */
        // breaks it 

    // lets try to just forbit STAR_! under the "simple" assumptions: 

    lp.set_a(STAR_1, C, 1);
    lp.set_a(c7_pen, C, 5);
    lp.set_b(C++, 0);
    constr_names.push_back("test_Star1_special_simple"); 

    // ------------------------------------
    // ------ Set Target and solve  -------
    // ------------------------------------
    bool edge = false;
    if(edge){
        lp.set_c(E , -1);
        std::cout << " ** Optimizing for |E| \n";
    }else{
        lp.set_c(X , -1);
        std::cout << " ** Optimizing for |X| \n";
    }

    save_matrix_to_file(lp, "./verification/M.txt");
    std::cout << "Saved M \n";

    save_var_to_file(var_names, "./verification/V.txt");
    std::cout << "Saved Variable names \n";

    save_var_to_file(constr_names, "./verification/C_names.txt");
    std::cout << "Saved Constraint names \n";

    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_quadratic_program(lp, ET());
    if (s.is_infeasible()) {
        std::cout << "LP Infeasible" << std::endl;
    } 
    else if (s.is_unbounded()) {
        std::cout << "LP Unbounded" << std::endl;
    } 
    else if (s.is_optimal()) {
        std::cout << "Status: Optimal" << std::endl;
        std::cout << "Value:" << s.objective_value() << "\n";
        auto it = s.variable_values_begin(); 
        // PRINT variable
        std::cout << "# constraints:" << C << "\n";
        std::cout << "# variables:" << VAR << "\n -------- \n";
        std::cout << "Non-zero variables:" << "\n";
        int zeros = 0;
        for (std::size_t i = 0; i < var_names.size(); ++i, ++it) {
            double val = CGAL::to_double(*it);
            if (val == 0.0) {zeros++; continue;}
            std::cout << var_names[i] << "\t" << val << std::endl;
        }
        std::cout << " \n --------- \n # Non-zero variables: " << VAR - zeros << "\n";
        if(edge){
            save_c_to_file(s, "./verification/ce.txt");
            save_primal_to_file(s, "./verification/pe.txt");
            std::cout << "Saved ce and pe \n";
        }else{
            save_c_to_file(s, "./verification/cx.txt");
            save_primal_to_file(s, "./verification/px.txt"); 
            std::cout << "Saved cx and px \n";
        }
    }
    return 0;
}
