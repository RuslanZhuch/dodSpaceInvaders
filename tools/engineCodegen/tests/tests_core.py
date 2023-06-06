import unittest

import sys
sys.path.append("../src")

import loader
import generator

class TestCore(unittest.TestCase):
    def test_load_data_executor1(self):
        data = loader.load_descriptor("assets/executor1.json")
        self.assertEqual(data.get("whoIAm"), "Executor")
        self.assertEqual(data.get("version"), 1)
        self.assertEqual(data.get("name"), "executor1")
        
    def test_load_data_executor2(self):
        data = loader.load_descriptor("assets/executor2.json")
        self.assertEqual(data.get("whoIAm"), "Executor_2")
        self.assertEqual(data.get("version"), 2)
        self.assertEqual(data.get("name"), "executor2")
        
    def test_load_data_from_folder(self):
        data = loader.load_descriptors("assets")
        self.assertEqual(len(data), 2)
        
        self.assertEqual(data[0].get("whoIAm"), "Executor")
        self.assertEqual(data[0].get("version"), 1)
        self.assertEqual(data[0].get("name"), "executor1")
        
        self.assertEqual(data[1].get("whoIAm"), "Executor_2")
        self.assertEqual(data[1].get("version"), 2)
        self.assertEqual(data[1].get("name"), "executor2")
        
    def test_generate_runtime_file(self):
        handler = generator.generate_runtime_file("dest")
        self.assertIsNotNone(handler)
        descriptor_file = open("dest/runtime.cpp")
        self.assertIsNotNone(descriptor_file)
        