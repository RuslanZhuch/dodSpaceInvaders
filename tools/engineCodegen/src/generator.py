import sys
sys.path.append("../../thirdparty/code_generator/src")

from code_generation.core import code_generator
from code_generation.cpp import cpp_variable
from code_generation.cpp import cpp_function
from code_generation.cpp import cpp_class

def generate_file(folder, file_name):
    return code_generator.CppFile(folder + "/" + file_name)

def generate_variable(handler, var_type, var_name, initial = None):
    variable = cpp_variable.CppVariable(
        name=var_name,
        type=var_type,
        initialization_value = initial
    )
    variable.render_to_string(handler)
    
def generate_line(handler, line):
    handler(line)
    
def generate_empty(handler, num_of_lines = 1):
    handler.newline(num_of_lines)
    
def generate_function(hander, function_name, function_data):
    func = cpp_function.CppFunction(name=function_name,
        ret_type='void',
        implementation_handle=function_data)
    func.render_to_string(hander)
    
def generate_block(hander, block_header, block_data):
    with hander.block(block_header):
        block_data(hander)
        
def _generate_class_method(method_name, return_type, arguments, is_const):
    method_handler = cpp_class.CppClass.CppMethod(name = method_name, ret_type = return_type, is_const=is_const)
    for argument in arguments:
        method_handler.add_argument(argument)
    return method_handler
    
def generate_class_private_method(class_handler, method_name, return_type, arguments, is_const):
    class_handler.add_private_method(_generate_class_method(method_name, return_type, arguments, is_const))
    
def generate_class_public_method(class_handler, method_name, return_type, arguments, is_const):
    class_handler.add_method(_generate_class_method(method_name, return_type, arguments, is_const))
    
def generate_class_variable(class_handler, variable_type, variable_name, initial = None):
    class_handler.add_variable(cpp_variable.CppVariable(
        name=variable_name,
        type=variable_type,
        initialization_value = initial
    ))
    
def generate_class(handler, class_name, class_data):
    class_handler = cpp_class.CppClass(name = class_name, is_struct = False)
    class_data(class_handler)
    class_handler.render_to_string_declaration(handler)
