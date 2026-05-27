#include <iostream>
#include <cassert>
#include <vector>
#include <set>
#include <fstream>
#include <string>
 
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

struct ConstraintManager {
    static Program* lp_ptr;
    static int* C_ptr;
    static std::vector<std::string>* names_ptr;
    static std::set<std::string> disabled;
    static std::string current_name;

    static void init(Program& lp, int& C, std::vector<std::string>& names) {
        lp_ptr = &lp; C_ptr = &C; names_ptr = &names;
        std::ifstream in("verification/disabled_constraints.txt");
        std::string line;
        while (std::getline(in, line)) {
            if (!line.empty()) disabled.insert(line);
        }
    }

    static bool start(const std::string& name) {
        if (disabled.count(name)) return false;
        current_name = name;
        return true;
    }

    static void finish(int b) {
        lp_ptr->set_b((*C_ptr)++, b);
        names_ptr->push_back(current_name);
    }
};

Program* ConstraintManager::lp_ptr = nullptr;
int* ConstraintManager::C_ptr = nullptr;
std::vector<std::string>* ConstraintManager::names_ptr = nullptr;
std::set<std::string> ConstraintManager::disabled;
std::string ConstraintManager::current_name;

#define START_C(name) if (ConstraintManager::start(name)) {
#define END_C(b) ConstraintManager::finish(b); }
 
int main() {
  
    Program lp (CGAL::SMALLER, true, 0, false, 0);
    // Variable counter, and names. 
    int VAR = 0; std::vector<std::string> var_names;
    // inequality counter and names for easier debugging
    int C = 0;   std::vector<std::string> constr_names;

    ConstraintManager::init(lp, C, constr_names);

        // Edges, Crossings 
    const int E       = VAR++; var_names.push_back("E");
    const int E_0     = VAR++; var_names.push_back("E_0");
    const int E_1     = VAR++; var_names.push_back("E_1");
    const int E_2     = VAR++; var_names.push_back("E_2");
    const int E_3     = VAR++; var_names.push_back("E_3"); 
    const int E_4     = VAR++; var_names.push_back("E_4");
    const int E_5     = VAR++; var_names.push_back("E_5");
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
    #include "include5/traildefs.inc"
    // ##############################

    // ##############################
    // FAN and ARM variable definitions
    #include "include5/arm_fan_definitions.inc"

    // ##############################
    // Trail cell counting
    #include "include5/trailcounts.inc"
    // ##############################
    // ##############################
    // Fac cell counting
    #include "include5/fan_c4_cells.inc"
    // ##############################

    const int T3_0_empty = VAR++; var_names.push_back("T3_0_empty");
    const int T3_0_c5qua = VAR++; var_names.push_back("T3_0_c5qua");
    const int T3_0_large = VAR++; var_names.push_back("T3_0_large");
    const int T3_0_L1AB = VAR++; var_names.push_back("T3_0_L1AB");
    const int T3_0_L1BC = VAR++; var_names.push_back("T3_0_L1BC");
    const int T3_0_L1BB = VAR++; var_names.push_back("T3_0_L1BB");
    const int T3_0_L2AB = VAR++; var_names.push_back("T3_0_L2AB");

    START_C("6.A - T3 custom conf GEQ")
        lp.set_a(T3_0_empty, C, 1); lp.set_a(T3_0_L1AB, C , 1); lp.set_a(T3_0_c5qua, C , 1); lp.set_a(T3_0_large, C , 1);
        lp.set_a(T3_0_L1BC, C, 1);
        lp.set_a(T3_0_L1BB, C, 1);
        lp.set_a(T3_0_L2AB, C, 1);
        lp.set_a(t0_c3_tri_c5_qua, C, -1);
    END_C(0)

    START_C("6.A - T3 custom conf LEQ")
        lp.set_a(t0_c3_tri_c5_qua, C, 1);
        lp.set_a(T3_0_empty, C ,-1); lp.set_a(T3_0_L1AB, C ,-1); lp.set_a(T3_0_c5qua, C ,-1); lp.set_a(T3_0_large, C ,-1);
        lp.set_a(T3_0_L1BC, C, -1);
        lp.set_a(T3_0_L1BB, C, -1);
        lp.set_a(T3_0_L2AB, C, -1);
    END_C(0)

    START_C("6.A - T3 L1AB")
        lp.set_a(T3_0_L1AB, C, 1); lp.set_a(F1_AB, C, -1);
    END_C(0)

    START_C("6.A - T3 L1BB")
        lp.set_a(T3_0_L1BB, C, 1); lp.set_a(F1_BB, C, -1);
    END_C(0)

    START_C("6.A - T3 L1BC")
        lp.set_a(T3_0_L1BC, C, 1); lp.set_a(F1_BC, C, -1);
    END_C(0)

    START_C("6.A - T3 L2AB")
        lp.set_a(T3_0_L2AB, C, 1); lp.set_a(F2_AB, C, -1);
    END_C(0)

    // ##############################
    // Edge lower bound constraints 
    // E_1
    START_C("9.A - Edges E1")
        lp.set_a(t0_c4_tri_c5_qua, C, 1);
        lp.set_a(E_1, C, -2);
    END_C(0)
    // E_2 an E_3
    // ##############################

        // 2.A -- counting edges, crossed and uncrossed 
    START_C("2.A = EO EX 1")
        lp.set_a(E_0, C, 1); lp.set_a(E_x, C, 1); 
        lp.set_a(E, C, -1);
    END_C(0)
    
    START_C("2.A = EO EX 2")
        lp.set_a(E, C, 1);
        lp.set_a(E_0, C, -1); lp.set_a(E_x, C, -1); 
    END_C(0)

    // 2.B -- counting crossed edges
    START_C("2.B = EX E1->5 1")
        lp.set_a(E_1, C, 1); lp.set_a(E_2, C, 1); lp.set_a(E_3, C, 1); lp.set_a(E_4, C, 1);
        lp.set_a(E_5, C, 1);
        lp.set_a(E_x, C, -1);
    END_C(0)

    START_C("2.B = EX E1->5 2")
        lp.set_a(E_x, C, 1);
        lp.set_a(E_1, C, -1); lp.set_a(E_2, C, -1); lp.set_a(E_3, C, -1); lp.set_a(E_4, C, -1); 
        lp.set_a(E_5, C, -1);
    END_C(0)

    // 2.C  -- counting crossings
    START_C("2.C = EX X 1")
        lp.set_a(E_1, C, 1); lp.set_a(E_2, C, 2); lp.set_a(E_3, C, 3); lp.set_a(E_4, C, 4);
        lp.set_a(E_5, C, 5); 
        lp.set_a(X, C, -2);
    END_C(0)
    
    START_C("2.C = EX X 2")
        lp.set_a(X, C, 2);
        lp.set_a(E_1, C, -1); lp.set_a(E_2, C, -2); lp.set_a(E_3, C, -3); lp.set_a(E_4, C, -4);
        lp.set_a(E_5, C, -5); 
    END_C(0)

    // 3.A -- uncrossed edges
    START_C("3.A cell->E_0")
        lp.set_a(c5_tri, C, 1); lp.set_a(c6_qua, C, 1); lp.set_a(c7_pen, C, 1); 
        lp.set_a(E_0, C, -2);
    END_C(0)
    
    // 3.B  -- outer edge segments
    START_C("3.B cell->E_outer")
        lp.set_a(c4_tri, C, 2); lp.set_a(c5_qua, C, 2); lp.set_a(c5_tri, C, 2); lp.set_a(c6_qua, C, 2); lp.set_a(c7_pen, C, 2);
        lp.set_a(E_x, C, -4); 
    END_C(0)

    // 3.C -- inner edge segments 
    START_C("3.C cell->E_inner")
        lp.set_a(c3_tri, C, 3); lp.set_a(c4_qua, C, 4); lp.set_a(c4_tri, C ,1); lp.set_a(c5_qua, C, 2); lp.set_a(c5_pen, C, 5); lp.set_a(c6_qua, C, 1); lp.set_a(c7_pen, C, 2);
        lp.set_a(t4_c4_tri_large, C, 1); lp.set_a(t4_large_large, C, 2); 
        lp.set_a(t3_c3_tri_large, C, 1); lp.set_a(t3_c4_tri_large, C, 1); lp.set_a(t3_c5_qua_large, C, 1); lp.set_a(t3_c5_pen_large, C, 1); lp.set_a(t3_large_large, C, 2); 
        lp.set_a(t2_c3_tri_large, C, 1); lp.set_a(t2_c4_tri_large, C, 1); lp.set_a(t2_c5_qua_large, C, 1); lp.set_a(t2_c5_pen_large, C, 1); lp.set_a(t2_large_large, C, 2); 
        lp.set_a(t1_c3_tri_large, C, 1); lp.set_a(t1_c4_tri_large, C, 1); lp.set_a(t1_c5_qua_large, C, 1); lp.set_a(t1_c5_pen_large, C, 1); lp.set_a(t0_large_large, C, 2); 
        lp.set_a(t0_c3_tri_large, C, 1); lp.set_a(t0_c4_tri_large, C, 1); lp.set_a(t0_c5_qua_large, C, 1); lp.set_a(t0_c5_pen_large, C, 1); lp.set_a(t1_large_large, C, 2); 
        lp.set_a(E_2, C, -2); lp.set_a(E_3, C, -4); lp.set_a(E_4, C, -6);
        lp.set_a(E_5, C, -8);
    END_C(0)


    /*
    // Ddensity formula with t = 6
    lp.set_a(SUM_LARGE, C, 1); lp.set_a(E, C, 4); lp.set_a(X, C, 4); 
    lp.set_a(c3_tri, C, -9); lp.set_a(c4_tri, C, -4); lp.set_a(c4_qua, C, -4);
    lp.set_a(c5_pen, C, 1); lp.set_a(c5_qua, C, 1); lp.set_a(c5_tri, C, 1);
    lp.set_b(C++, 24);
    */

    
    // Density formula wit  t = 7.5
    START_C("1.A / Density Formula")
        lp.set_a(SUM_LARGE, C, 3); lp.set_a(E, C, 8); lp.set_a(X, C, 8); 
        lp.set_a(c3_tri, C, -21); lp.set_a(c4_tri, C, -8); lp.set_a(c4_qua, C, -8);
        lp.set_a(c5_pen, C, 5); lp.set_a(c5_qua, C, 5); lp.set_a(c5_tri, C, 5);
    END_C(60)

    // 1.B -- lower bounding the number of large cells
    START_C("1.B / Large_cell_LB")
        lp.set_a(t4_c4_tri_large, C, 1); lp.set_a(t4_large_large, C, 2); 
        lp.set_a(t3_c3_tri_large, C, 1); lp.set_a(t3_c4_tri_large, C, 1); lp.set_a(t3_c5_qua_large, C, 1); lp.set_a(t3_c5_pen_large, C, 1); lp.set_a(t3_large_large, C, 2); 
        lp.set_a(t2_c3_tri_large, C, 1); lp.set_a(t2_c4_tri_large, C, 1); lp.set_a(t2_c5_qua_large, C, 1); lp.set_a(t2_c5_pen_large, C, 1); lp.set_a(t2_large_large, C, 2); 
        lp.set_a(t1_c3_tri_large, C, 1); lp.set_a(t1_c4_tri_large, C, 1); lp.set_a(t1_c5_qua_large, C, 1); lp.set_a(t1_c5_pen_large, C, 1); lp.set_a(t0_large_large, C, 2); 
        lp.set_a(t0_c3_tri_large, C, 1); lp.set_a(t0_c4_tri_large, C, 1); lp.set_a(t0_c5_qua_large, C, 1); lp.set_a(t0_c5_pen_large, C, 1); lp.set_a(t1_large_large, C, 2); 
        lp.set_a(c6_qua, C, 3); lp.set_a(c7_pen, C, 3); 
        lp.set_a(LC_INCIDENCE, C, 1);
        lp.set_a(SUM_LARGE, C, -1);
    END_C(0)


        // ----------------------------------------------------------------
    // ------ Constraints concerning the ||c|| >= 5 incidences --------
    // ----------------------------------------------------------------
    // Y.A------- c5_tri outer edge segments  -------
    // complete trails
    START_C("10.A - c5_tri incid")
        lp.set_a(t2_c4_tri_c5_qua, C, 1); lp.set_a(t2_c5_qua_c5_qua, C, 2); lp.set_a(t2_c5_qua_large, C, 1);  // TODO these contribute tho
        lp.set_a(t1_c3_tri_c5_qua, C, 1); 
        lp.set_a(t1_c5_pen_c5_qua, C, 1);
        // smaller trails that do not force it, but might have adjacencies in the empty and c6 case
        lp.set_a(T3_0_empty, C, 1); //lp.set_a(T4_1_empty, C, 1); lp.set_a(T5_0_empty, C, 1);
        // Fans
        lp.set_a(F1_AA, C, 2); lp.set_a(F2_AA, C, 2); lp.set_a(F3_AA, C, 2); lp.set_a(F4_AA, C, 2);
        lp.set_a(F3_AB, C, 1); lp.set_a(F3_AC, C, 1);
        lp.set_a(F2_AB, C, 1); lp.set_a(F2_AC, C, 1);
        lp.set_a(F1_AB, C, 1); lp.set_a(F1_AC, C, 1);
        lp.set_a(c5_tri, C, -2);
    END_C(0)

    
    // Y.B ------- c5_qua outer edge segments  -------
    START_C("10.B - c5_qua incid")
        lp.set_a(T3_0_c5qua, C, 1); //lp.set_a(T4_1_c5qua, C, 1); lp.set_a(T5_0_c5qua, C, 1);
        lp.set_a(F1_AB, C, 1); 
        lp.set_a(F1_BB, C, 2);
        lp.set_a(F1_BC, C, 1); 
        lp.set_a(F2_AB, C, 1); 
        lp.set_a(F2_BB, C, 2);
        lp.set_a(F2_BC, C, 1); 
        lp.set_a(F3_AB, C, 1);  
        lp.set_a(c5_qua, C, -2);
    END_C(0)


    // ------- some large cell outer edge segments  -------
    START_C("10.C - LC incid")
        lp.set_a(T3_0_large, C, 1); //lp.set_a(T4_1_large, C, 1); lp.set_a(T5_0_large, C, 1);
        lp.set_a(F1_AC, C, 1); 
        lp.set_a(F1_CC, C, 2);
        lp.set_a(F1_CC, C, 1); 
        lp.set_a(F2_AC, C, 1); 
        lp.set_a(F2_CC, C, 2);
        lp.set_a(F2_BC, C, 1); 
        lp.set_a(F3_AC, C, 1);  
        lp.set_a(LC_INCIDENCE, C, -1);
    END_C(0)


    // testing ground 
    START_C("test_c3_tri")
        lp.set_a(c3_tri, C, 1);
    END_C(1)


    // ------------------------------------
    // ------ Set Target and solve  -------
    // ------------------------------------
    lp.set_c(E, -1);

    save_matrix_to_file(lp, "./verification/M.txt");
    save_var_to_file(var_names, "./verification/V.txt");
    save_var_to_file(constr_names, "./verification/C_names.txt");

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
        std::cout << "# variables:" << VAR << "\n";
        std::cout << "Non-zero variables:" << "\n";
        int zeros = 0;
        for (std::size_t i = 0; i < var_names.size(); ++i, ++it) {
            double val = CGAL::to_double(*it);
            if (val == 0.0) {zeros++; continue;}
            std::cout << var_names[i] << "\t" << val << std::endl;
        }
        std::cout << " # Non-zero variables: " << VAR - zeros << "\n";

        save_c_to_file(s, "./verification/ce.txt");
        save_primal_to_file(s, "./verification/pe.txt");
    }
    return 0;
}
