import sys
sys.path.append("../../thirdparty/code_generator/src")

from code_generation.core import code_generator
from code_generation.cpp import cpp_variable
from code_generation.cpp import cpp_function

def generate_file(folder, file_name):
    return code_generator.CodeFile(folder + "/" + file_name)

def generate_variable(handler, var_type, var_name, initial = None):
    variable = cpp_variable.CppVariable(
        name=var_name,
        type=var_type,
        initialization_value = initial
    )
    variable.render_to_string(handler)
    
def generate_line(handler, line):
    handler(line)
    
def generate_function(hander, function_name, function_data):
    func = cpp_function.CppFunction(name=function_name,
        ret_type='void',
        implementation_handle=function_data)
    func.render_to_string(hander)
    
def generate_block(hander, block_header, block_data):
    with hander.block(block_header):
        block_data(hander)
    