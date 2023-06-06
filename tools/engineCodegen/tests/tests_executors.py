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

EXPECT_NUM_OF_EXECUTORS = 2

def load_executors():
    return executors.load("assets")

def create_target_file():
    return generator.generate_runtime_file("dest")

class TestExecutors(unittest.TestCase):
    def test_load_executors(self):
        executors_data = load_executors()
        self.assertEqual(len(executors_data), EXPECT_NUM_OF_EXECUTORS)
        
    def test_get_executor_name(self):
        executors_data = load_executors()
        self.assertEqual(len(executors_data), EXPECT_NUM_OF_EXECUTORS)
        self.assertEqual(executors.get_name(executors_data[0]), "executor1")
        self.assertEqual(executors.get_name(executors_data[1]), "executor2")
        
    def test_gen_executor_init(self):
        executors_data = load_executors()
        self.assertEqual(len(executors_data), EXPECT_NUM_OF_EXECUTORS)
        
        handler = create_target_file()
        self.assertIsNotNone(handler)
        
        executors.gen_inits(handler, executors_data)
        
        handler.close()
        
        descriptor_file = open("dest/runtime.cpp", "r")
        file_data = descriptor_file.read()
        self.assertEqual(file_data, "Game::ExecutionBlock::Executor1 executor1;\nGame::ExecutionBlock::Executor2 executor2;\n")
        