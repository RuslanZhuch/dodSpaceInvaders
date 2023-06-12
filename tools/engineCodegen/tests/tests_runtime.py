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

import utils

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
        
        utils.assert_files(self, "dest/runtime.cpp", "assets/expected/runtime.cpp")
