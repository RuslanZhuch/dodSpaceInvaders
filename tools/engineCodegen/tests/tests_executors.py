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

import utils

EXPECT_NUM_OF_EXECUTORS = 2

def load_executors():
    return executors.load("assets/executors")

def load_executors_for_class():
    return executors.load("assets/executors/classGeneration")

def create_target_file():
    return runtime.generate_runtime_file("dest")

class TestExecutors(unittest.TestCase):
    def __init__(self, methodName: str = "runTest") -> None:
        super().__init__(methodName)
        self.maxDiff = None
        
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
        
        utils.assert_files(self, "dest/runtime.cpp", "assets/expected/executorsGeneration.cpp")
        
    def test_gen_executor_update(self):
        executors_data = load_executors()
        self.assertEqual(len(executors_data), EXPECT_NUM_OF_EXECUTORS)
        
        handler = create_target_file()
        self.assertIsNotNone(handler)
        
        executors.gen_updates(handler, executors_data)
        
        handler.close()
        
        utils.assert_files(self, "dest/runtime.cpp", "assets/expected/executorsUpdates.cpp")
        
    def test_gen_executor_flush(self):
        executors_data = load_executors()
        self.assertEqual(len(executors_data), EXPECT_NUM_OF_EXECUTORS)
        
        handler = create_target_file()
        self.assertIsNotNone(handler)
        
        executors.gen_flush(handler, executors_data)
        
        handler.close()
        
        utils.assert_files(self, "dest/runtime.cpp", "assets/expected/executorsFlush.cpp")
        
    def test_gen_executor_1_body_flush(self):
        executors_data = load_executors()
        self.assertEqual(len(executors_data), EXPECT_NUM_OF_EXECUTORS)
        
        handler = generator.generate_file("dest", "gen_executor_1_body_flush.cpp")
        self.assertIsNotNone(handler)
        
        executors.gen_body_flush(handler, executors_data[0])
        handler.close()
        
        descriptor_file = open("dest/gen_executor_1_body_flush.cpp", "r")
        file_data = descriptor_file.read()
        self.assertEqual(file_data, "")
        
    def test_gen_executor_2_body_flush(self):
        executors_data = load_executors()
        self.assertEqual(len(executors_data), EXPECT_NUM_OF_EXECUTORS)
        
        handler = generator.generate_file("dest", "gen_executor_2_body_flush.cpp")
        self.assertIsNotNone(handler)
        
        executors.gen_body_flush(handler, executors_data[1])
        handler.close()
        
        utils.assert_files(self, "dest/gen_executor_2_body_flush.cpp", "assets/expected/gen_executor_2_body_flush.cpp")
        
    def test_gen_executor_1_body_init(self):
        executors_data = load_executors()
        self.assertEqual(len(executors_data), EXPECT_NUM_OF_EXECUTORS)
        
        handler = generator.generate_file("dest", "gen_executor_1_body_init.cpp")
        self.assertIsNotNone(handler)
        
        executors.gen_body_init(handler, executors_data[0])
        handler.close()
        
        descriptor_file = open("dest/gen_executor_1_body_init.cpp", "r")
        file_data = descriptor_file.read()
        self.assertEqual(file_data, "")
        
    def test_gen_executor_2_body_init(self):
        executors_data = load_executors()
        self.assertEqual(len(executors_data), EXPECT_NUM_OF_EXECUTORS)
        
        handler = generator.generate_file("dest", "gen_executor_2_body_init.cpp")
        self.assertIsNotNone(handler)
        
        executors.gen_body_init(handler, executors_data[1])
        handler.close()
        
        utils.assert_files(self, "dest/gen_executor_2_body_init.cpp", "assets/expected/gen_executor_2_body_init.cpp")
        
    def test_gen_executor_test1_header(self):
        executors_data = load_executors_for_class()
        self.assertEqual(len(executors_data), 1)
        
        executors.gen_header("dest", executors_data[0])
        
        utils.assert_files(self, "dest/Test1Executor.h", "assets/expected/Test1Executor.h")

    def test_gen_executor_test1_source(self):
        executors_data = load_executors_for_class()
        self.assertEqual(len(executors_data), 1)
        
        executors.gen_source("dest", executors_data[0])
        
        utils.assert_files(self, "dest/Test1Executor.cpp", "assets/expected/Test1Executor.cpp")
