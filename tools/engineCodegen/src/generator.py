import sys
sys.path.append("../../thirdparty/code_generator/src")

from code_generation.core import code_generator
from code_generation.cpp import cpp_variable

def generate_runtime_file(folder):
    return code_generator.CodeFile(folder + "/" + 'runtime.cpp')

def generate_variable(handler, var_type, var_name):
    variable = cpp_variable.CppVariable(
        name=var_name,
        type=var_type
    )
    variable.render_to_string(handler)
    