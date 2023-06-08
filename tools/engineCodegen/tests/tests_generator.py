import unittest

import json

from os import path
import sys
 
from pathlib import Path
root_dir = Path.cwd()
sys.path.append(root_dir)

import sys
sys.path.append("../src")

import executors
import generator
import runtime

EXPECT_NUM_OF_EXECUTORS = 2

def load_executors():
    return executors.load("assets/executors")

def create_target_file():
    return generator.generate_file("dest", "gen.cpp")

class TestGenerators(unittest.TestCase):
    def test_generate_file(self):
        self.assertIsNotNone(generator.generate_file("dest", "file1.cpp"))
        open("dest/file1.cpp")
        self.assertIsNotNone(generator.generate_file("dest", "file2.cpp"))
        open("dest/file2.cpp")
        
    def test_generate_line(self):
        handler = create_target_file()
        self.assertIsNotNone(handler)
        
        generator.generate_line(handler, "Generated line")
        
        handler.close()
        
        descriptor_file = open("dest/gen.cpp", "r")
        file_data = descriptor_file.read()
        
        self.assertEqual(file_data, "Generated line\n")
        
    def test_generate_empty_line(self):
        handler = generator.generate_file("dest", "gen_newLine.cpp")
        self.assertIsNotNone(handler)
        
        generator.generate_line(handler, "Generated line")
        generator.generate_empty(handler)
        generator.generate_line(handler, "Generated line2")
        generator.generate_empty(handler, 2)
        generator.generate_line(handler, "Generated line3")
        
        def block_data(handler):
            generator.generate_empty(handler)
            generator.generate_line(handler, "Generated line4")
            
        generator.generate_block(handler, "", block_data)
        
        handler.close()
        
        descriptor_file = open("dest/gen_newLine.cpp", "r")
        file_data = descriptor_file.read()
        
        expected_file = open("assets/expected/newLine.cpp", "r")
        expected_file_data = expected_file.read()
        
        self.assertEqual(file_data, expected_file_data)
        
    def test_generate_variable(self):
        handler = create_target_file()
        self.assertIsNotNone(handler)
        
        generator.generate_variable(handler, "Var_type", "var_name")
        
        handler.close()
        
        descriptor_file = open("dest/gen.cpp", "r")
        file_data = descriptor_file.read()
        
        self.assertEqual(file_data, "Var_type var_name;\n")
        
    def test_generate_variable_initial(self):
        handler = create_target_file()
        self.assertIsNotNone(handler)
        
        generator.generate_variable(handler, "Var_type", "var_name", 42)
        
        handler.close()
        
        descriptor_file = open("dest/gen.cpp", "r")
        file_data = descriptor_file.read()
        
        self.assertEqual(file_data, "Var_type var_name{ 42 };\n")
        
    def test_generate_variable_initial_zero(self):
        handler = create_target_file()
        self.assertIsNotNone(handler)
        
        generator.generate_variable(handler, "Var_type", "var_name", 0)
        
        handler.close()
        
        descriptor_file = open("dest/gen.cpp", "r")
        file_data = descriptor_file.read()
        
        self.assertEqual(file_data, "Var_type var_name{};\n")
                
    def test_generate_function(self):
        handler = create_target_file()
        self.assertIsNotNone(handler)
        
        
        def func_data(self, handler):
            generator.generate_line(handler, "Line1")
        
        generator.generate_function(handler, "func1", func_data)
        
        handler.close()
        
        descriptor_file = open("dest/gen.cpp", "r")
        file_data = descriptor_file.read()
        
        self.assertEqual(file_data, "void func1()\n{\n    Line1\n}\n")
        
    def test_generate_block(self):
        handler = create_target_file()
        self.assertIsNotNone(handler)
        
        
        def block_data(handler):
            generator.generate_line(handler, "Block line")
        
        generator.generate_block(handler, "block1", block_data)
        
        handler.close()
        
        descriptor_file = open("dest/gen.cpp", "r")
        file_data = descriptor_file.read()
        
        self.assertEqual(file_data, "block1\n{\n    Block line\n}\n")
        
    def test_generate_empty_class(self):
        handler = generator.generate_file("dest", "gen_empty_class.cpp")
        self.assertIsNotNone(handler)
        
        def class_data(class_handler):
            pass
        generator.generate_class(handler, "Test1", class_data)
        
        handler.close()
        
        descriptor_file = open("dest/gen_empty_class.cpp", "r")
        file_data = descriptor_file.read()
        
        expected_file = open("assets/expected/classDeclarationEmpty.cpp", "r")
        expected_file_data = expected_file.read()
        
        self.assertEqual(file_data, expected_file_data)
        
    def test_generate_class(self):
        handler = generator.generate_file("dest", "gen_class.cpp")
        self.assertIsNotNone(handler)
        
        def class_data(class_handler):
            generator.generate_class_public_method(class_handler, "publicMethod1", "float", [], True)
            generator.generate_class_public_method(class_handler, "publicMethod2", "void", ['float dt'], False)
            generator.generate_class_private_method(class_handler, "privateMethod1", "int", ['int n'], True)
            generator.generate_class_variable(class_handler, "int", "var1", 0)
            generator.generate_class_variable(class_handler, "Dod::MemPool", "memory")
            
        generator.generate_class(handler, "Test2", class_data)

        handler.close()
        
        descriptor_file = open("dest/gen_class.cpp", "r")
        file_data = descriptor_file.read()
        
        expected_file = open("assets/expected/classDeclaration.cpp", "r")
        expected_file_data = expected_file.read()
        
        self.assertEqual(file_data, expected_file_data)
        