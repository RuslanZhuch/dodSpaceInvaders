import unittest

import json

from os import path
import sys
 
from pathlib import Path
root_dir = Path.cwd()
sys.path.append(root_dir)

import sys
sys.path.append("../src")

import runtime

class TestRuntime(unittest.TestCase):
    def __init__(self, methodName: str = "runTest") -> None:
        super().__init__(methodName)
        self.maxDiff = None
        
    def test_generate_runtime_file(self):
        handler = runtime.generate_runtime_file("dest")
        self.assertIsNotNone(handler)
        descriptor_file = open("dest/runtime.cpp")
        
    def test_generate_runtime_function(self):
        runtime.generate("dest", "runtime.cpp")
        
        descriptor_file = open("dest/runtime.cpp", "r")
        file_data = descriptor_file.read()
        
        expected_file = open("assets/expected/runtime.cpp", "r")
        expected_file_data = expected_file.read()
        self.assertEqual(file_data, expected_file_data)
        