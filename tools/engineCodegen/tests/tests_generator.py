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
        
        self.assertEqual(file_data, "void func1()\n{\n\tLine1\n}\n")
        
    def test_generate_block(self):
        handler = create_target_file()
        self.assertIsNotNone(handler)
        
        
        def block_data(handler):
            generator.generate_line(handler, "Block line")
        
        generator.generate_block(handler, "block1", block_data)
        
        handler.close()
        
        descriptor_file = open("dest/gen.cpp", "r")
        file_data = descriptor_file.read()
        
        self.assertEqual(file_data, "block1\n{\n\tBlock line\n}\n")
        