import re
import os
import sys

# Macros to be added to cpp files
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
    pattern = re.compile(r'((?:^\s*(?:lp\.set_a|//)[^\n]*\n)+)\s*lp\.set_b\(C\+\+,\s*([^)]+)\);\s*constr_names\.push_back\("([^"]+)"\);', re.MULTILINE)
    
    def replace_func(match):
        block = match.group(1).rstrip()
        b_val = match.group(2).strip()
        name = match.group(3).strip()
        
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
    
    if file_path.endswith('.cpp'):
        if 'struct ConstraintManager' not in new_content:
            new_content = new_content.replace('int main()', MACROS + '\nint main()')
        
        init_call = '    ConstraintManager::init(lp, C, constr_names);'
        if init_call not in new_content:
            new_content = new_content.replace('Program lp (CGAL::SMALLER, true, 0, false, 0);', 
                                              'Program lp (CGAL::SMALLER, true, 0, false, 0);\n' + init_call)

    with open(file_path + '.refactored', 'w') as f:
        f.write(new_content)
    print(f"Created {file_path}.refactored")

if __name__ == "__main__":
    # Detect local files
    files = []
    for f in os.listdir('..'):
        if f.endswith('.cpp'):
            files.append(os.path.join('..', f))
    
    # include dirs
    for d in os.listdir('..'):
        if d.startswith('include') and os.path.isdir(os.path.join('..', d)):
            for root, dirs, f_names in os.walk(os.path.join('..', d)):
                for f in f_names:
                    if f.endswith('.inc'):
                        files.append(os.path.join(root, f))
    
    for f in files:
        process_file(f)
    
    print("\nRefactoring complete. Review the .refactored files.")
