import unittest

import json

from os import path
import sys
 
from pathlib import Path
root_dir = Path.cwd()
sys.path.append(root_dir)

import sys
sys.path.append("../src")

import generator
import contexts

import utils

def load_shared_contexts():
    return contexts.load_shared("assets/contexts/shared")

class TestContexts(unittest.TestCase):
    def __init__(self, methodName: str = "runTest") -> None:
        super().__init__(methodName)
        self.maxDiff = None
        
    def test_load_shared_context_instancies(self):
        data = contexts.load_shared_context_instances("assets/workspace/ws_sharedContexts.json")
        self.assertEqual(len(data), 4)
        
        self.assertEqual(data[0].context_name, "sContext1")
        self.assertEqual(data[0].instance_name, "shared1")
        
        self.assertEqual(data[1].context_name, "sContext1")
        self.assertEqual(data[1].instance_name, "shared2")
        
        self.assertEqual(data[2].context_name, "sContext2")
        self.assertEqual(data[2].instance_name, "shared3")
        
        self.assertEqual(data[3].context_name, "sContext3")
        self.assertEqual(data[3].instance_name, "shared4")
        
    def test_get_valid_shared_context_instancies(self):
        contexts_data = load_shared_contexts()
        instances_data = contexts.load_shared_context_instances("assets/workspace/ws_sharedContexts.json")
        
        validated_instances = contexts.get_validated_shared_context_instances(contexts_data, instances_data)
        self.assertEqual(len(validated_instances), 3)
        self.assertEqual(validated_instances[0].context_name, "sContext1")
        self.assertEqual(validated_instances[0].instance_name, "shared1")
        
        self.assertEqual(validated_instances[1].context_name, "sContext1")
        self.assertEqual(validated_instances[1].instance_name, "shared2")
        
        self.assertEqual(validated_instances[2].context_name, "sContext2")
        self.assertEqual(validated_instances[2].instance_name, "shared3")
        
    def test_generate_shared_contexts_init(self):
        instances_data = contexts.load_shared_context_instances("assets/workspace/ws_sharedContexts.json")
        handler = generator.generate_file("dest", "gen_scontexts_init.cpp")
        self.assertIsNotNone(handler)
        contexts.generate_shared_init(handler, instances_data)
        
        handler.close()
        
        utils.assert_files(self, "dest/gen_scontexts_init.cpp", "assets/expected/gen_scontexts_init.cpp")
        