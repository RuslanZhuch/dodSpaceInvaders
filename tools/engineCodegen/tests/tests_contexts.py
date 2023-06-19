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
import loader

import utils

def load_shared_contexts():
    return contexts.load_shared("assets/contexts/shared")

class TestContexts(unittest.TestCase):
    def __init__(self, methodName: str = "runTest") -> None:
        super().__init__(methodName)
        self.maxDiff = None
    
    def test_load_context_data(self):
        context_raw_data = loader.load_file_data("assets/contexts/local/lContext1.json")
        context_data = contexts.load_data(context_raw_data)
        
        self.assertEqual(len(context_data.objects_data), 3)
        self.assertEqual(len(context_data.buffers_data), 2)
        
        self.assertEqual(context_data.objects_data[0].name, "var1")
        self.assertEqual(context_data.objects_data[1].name, "var2")
        self.assertEqual(context_data.objects_data[2].name, "var3")
        
        self.assertEqual(context_data.objects_data[0].data_type, "int32_t")
        self.assertEqual(context_data.objects_data[1].data_type, "float")
        self.assertEqual(context_data.objects_data[2].data_type, "CustomData1")
        
        self.assertEqual(context_data.objects_data[0].initial, 1)
        self.assertEqual(context_data.objects_data[1].initial, 1.1)
        self.assertEqual(context_data.objects_data[2].initial, None)
        
        self.assertEqual(context_data.buffers_data[0].name, "dbvar1")
        self.assertEqual(context_data.buffers_data[1].name, "dbvar2")
        
        self.assertEqual(context_data.buffers_data[0].data_type, "float")
        self.assertEqual(context_data.buffers_data[1].data_type, "int64_t")
        
        self.assertEqual(context_data.buffers_data[0].capacity, 512)
        self.assertEqual(context_data.buffers_data[1].capacity, 1024)
        
    def test_gen_context1_data(self):
        context_raw_data = loader.load_file_data("assets/contexts/local/lContext1.json")
        
        handler = generator.generate_file("dest", "gen_lContext1_data.cpp")
        self.assertIsNotNone(handler)
        contexts.generate_context_data(handler, context_raw_data)
        
        handler.close()
        
        utils.assert_files(self, "dest/gen_lContext1_data.cpp", "assets/expected/gen_lContext1_data.cpp")
        
    def test_load_shared_context_instancies(self):
        data = contexts.load_shared_context_instances("assets/workspace/ws_applicationContext.json")
        self.assertEqual(len(data), 5)
        
        self.assertEqual(data[0].context_name, "sContext1")
        self.assertEqual(data[0].instance_name, "sharedInst1")
        
        self.assertEqual(data[1].context_name, "sContext1")
        self.assertEqual(data[1].instance_name, "sharedInst2")
        
        self.assertEqual(data[2].context_name, "sContext2")
        self.assertEqual(data[2].instance_name, "sharedInst3")
        
        self.assertEqual(data[3].context_name, "sContext2")
        self.assertEqual(data[3].instance_name, "sharedInst4")
        
        self.assertEqual(data[4].context_name, "sContext3")
        self.assertEqual(data[4].instance_name, "sharedInst5")
        
    def test_get_valid_shared_context_instancies(self):
        contexts_data = load_shared_contexts()
        instances_data = contexts.load_shared_context_instances("assets/workspace/ws_applicationContext.json")
        
        validated_instances = contexts.get_validated_shared_context_instances(contexts_data, instances_data)
        self.assertEqual(len(validated_instances), 4)
        self.assertEqual(validated_instances[0].context_name, "sContext1")
        self.assertEqual(validated_instances[0].instance_name, "sharedInst1")
        
        self.assertEqual(validated_instances[1].context_name, "sContext1")
        self.assertEqual(validated_instances[1].instance_name, "sharedInst2")
        
        self.assertEqual(validated_instances[2].context_name, "sContext2")
        self.assertEqual(validated_instances[2].instance_name, "sharedInst3")
        
        self.assertEqual(validated_instances[3].context_name, "sContext2")
        self.assertEqual(validated_instances[3].instance_name, "sharedInst4")
        
    def test_generate_shared_contexts_init(self):
        instances_data = contexts.load_shared_context_instances("assets/workspace/ws_applicationContext.json")
        handler = generator.generate_file("dest", "gen_scontexts_init.cpp")
        self.assertIsNotNone(handler)
        contexts.generate_shared_init(handler, instances_data)
        
        handler.close()
        
        utils.assert_files(self, "dest/gen_scontexts_init.cpp", "assets/expected/gen_scontexts_init.cpp")
        
    def test_get_shared_contexts_flush(self):
        workspace_data = loader.load_application_context_data("assets/workspace/ws_applicationContext.json")
        flush_data = contexts.load_shared_context_to_flush(workspace_data)
        self.assertEqual(len(flush_data), 3)
        self.assertEqual(flush_data[0], "sharedInst1")
        self.assertEqual(flush_data[1], "sharedInst2")
        self.assertEqual(flush_data[2], "sharedInst4")
        
    def test_generate_shared_contexts_flush(self):
        handler = generator.generate_file("dest", "gen_shared_contexts_flush.cpp")
        self.assertIsNotNone(handler)
        workspace_data = loader.load_application_context_data("assets/workspace/ws_applicationContext.json")
        contexts.generate_shared_flush(handler, workspace_data)
        
        handler.close()
        
        utils.assert_files(self, "dest/gen_shared_contexts_flush.cpp", "assets/expected/gen_shared_contexts_flush.cpp")
       
    def test_load_shared_contexts_merge(self):
        workspace_data = loader.load_application_context_data("assets/workspace/ws_applicationContext.json")
        
        merge_data = contexts.load_shared_context_merge(workspace_data)
        self.assertEqual(len(merge_data), 4)
        
        self.assertIsNotNone(merge_data["sharedInst1"])
        self.assertEqual(len(merge_data["sharedInst1"]), 2)
        self.assertEqual(merge_data["sharedInst1"][0].executor_name, "executor2")
        self.assertEqual(merge_data["sharedInst1"][0].executor_scontext, "shared1")
        self.assertEqual(merge_data["sharedInst1"][1].executor_name, "executor3")
        self.assertEqual(merge_data["sharedInst1"][1].executor_scontext, "shared1_2")
        
        self.assertIsNotNone(merge_data["sharedInst2"])
        self.assertEqual(len(merge_data["sharedInst2"]), 1)
        self.assertEqual(merge_data["sharedInst2"][0].executor_name, "executor2")
        self.assertEqual(merge_data["sharedInst2"][0].executor_scontext, "shared2")
        
        self.assertIsNotNone(merge_data["sharedInst3"])
        self.assertEqual(len(merge_data["sharedInst3"]), 2)
        self.assertEqual(merge_data["sharedInst3"][0].executor_name, "executor2")
        self.assertEqual(merge_data["sharedInst3"][0].executor_scontext, "shared3")
        self.assertEqual(merge_data["sharedInst3"][1].executor_name, "executor3")
        self.assertEqual(merge_data["sharedInst3"][1].executor_scontext, "shared2_2")
        
        self.assertIsNotNone(merge_data["sharedInst4"])
        self.assertEqual(len(merge_data["sharedInst4"]), 1)
        self.assertEqual(merge_data["sharedInst4"][0].executor_name, "executor2")
        self.assertEqual(merge_data["sharedInst4"][0].executor_scontext, "shared4")
        
    def test_generate_shared_contexts_merge(self):
        handler = generator.generate_file("dest", "gen_shared_contexts_merge.cpp")
        self.assertIsNotNone(handler)
        workspace_data = loader.load_application_context_data("assets/workspace/ws_applicationContext.json")
        contexts.generate_shared_merge(handler, workspace_data)
        
        handler.close()
        
        utils.assert_files(self, "dest/gen_shared_contexts_merge.cpp", "assets/expected/gen_shared_contexts_merge.cpp")
       