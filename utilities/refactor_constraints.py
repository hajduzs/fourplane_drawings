import re
import os
import sys

# Macros to be added to fourplane.cpp
MACROS = """
#include <set>
#include <fstream>
#include <string>

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
"""

def refactor_content(content):
    # Pattern to match:
    # 1. A block of lines starting with lp.set_a or // (comments)
    # 2. followed by lp.set_b(C++, <val>);
    # 3. followed by constr_names.push_back("<name>");
    
    # We use [^\n]* to match the rest of the line and \n to match the newline.
    # This ensures we capture the entire line even if it has multiple lp.set_a calls.
    pattern = re.compile(r'((?:^\s*(?:lp\.set_a|//)[^\n]*\n)+)\s*lp\.set_b\(C\+\+,\s*([^)]+)\);\s*constr_names\.push_back\("([^"]+)"\);', re.MULTILINE)
    
    def replace_func(match):
        block = match.group(1).rstrip()
        b_val = match.group(2).strip()
        name = match.group(3).strip()
        
        # Split into lines, strip them, and re-indent
        lines = block.split("\n")
        indented_lines = []
        for line in lines:
            if line.strip():
                indented_lines.append("    " + line.strip())
        
        indented_block = "\n".join(indented_lines)
        
        return f'START_C("{name}")\n{indented_block}\n    END_C({b_val})'

    return pattern.sub(replace_func, content)

def process_file(file_path):
    print(f"Processing {file_path}...")
    with open(file_path, 'r') as f:
        content = f.read()
    
    new_content = refactor_content(content)
    
    # Special case for fourplane.cpp: add macros and init call
    if file_path.endswith('fourplane.cpp'):
        # Insert macros before main
        if 'struct ConstraintManager' not in new_content:
            new_content = new_content.replace('int main()', MACROS + '\nint main()')
        
        # Insert init call after variable declarations
        init_call = '    ConstraintManager::init(lp, C, constr_names);'
        if init_call not in new_content:
            new_content = new_content.replace('Program lp (CGAL::SMALLER, true, 0, false, 0);', 
                                              'Program lp (CGAL::SMALLER, true, 0, false, 0);\n' + init_call)

    with open(file_path + '.refactored', 'w') as f:
        f.write(new_content)
    print(f"Created {file_path}.refactored")

if __name__ == "__main__":
    # List of files to process
    files = ['fourplane.cpp']
    for root, dirs, f_names in os.walk('include4'):
        for f in f_names:
            if f.endswith('.inc'):
                files.append(os.path.join(root, f))
    
    for f in files:
        if os.path.exists(f):
            process_file(f)
    
    print("\nRefactoring complete. Review the .refactored files.")
    print("If they look good, you can overwrite the originals.")
