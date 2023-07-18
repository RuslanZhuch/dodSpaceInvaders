import unittest

import os
from os import path
import sys
 
from pathlib import Path
root_dir = Path.cwd()
sys.path.append(root_dir)

import sys
sys.path.append("../src")

import shutil

import utils
import project

def get_file_modification_time(filename : str):
    return path.getmtime(filename)
    
class TestProject(unittest.TestCase):
    def __init__(self, methodName: str = "runTest") -> None:
        super().__init__(methodName)
        self.maxDiff = None
    
    def test_decode_project1_file(self):
        project_desc = project.load("assets/workspace/project1.json")
        
        self.assertIsNotNone(project_desc)
        
        self.assertEqual(project_desc.project_dest_folder, "dest/projects/project1")
        
        self.assertEqual(project_desc.project_root_src_folder, "assets/workspace")
        self.assertEqual(project_desc.project_root_src_types, "assets/workspace")
        self.assertEqual(project_desc.project_contexts_src_folder, "assets/contexts")
        self.assertEqual(project_desc.project_executors_src_folder, "assets/executors")
        
        self.assertEqual(project_desc.application_context_path, "ws_applicationContext.json")
        
        self.assertEqual(len(project_desc.types_paths), 2)
        self.assertEqual(project_desc.types_paths[0], "types_default.json")
        self.assertEqual(project_desc.types_paths[1], "types_contexts_data.json")
        
        self.assertEqual(len(project_desc.contexts_paths), 5)
        self.assertEqual(project_desc.contexts_paths[0], "lContext1.json")
        self.assertEqual(project_desc.contexts_paths[1], "lContext2.json")
        self.assertEqual(project_desc.contexts_paths[2], "lContext3.json")
        self.assertEqual(project_desc.contexts_paths[3], "sContext1.json")
        self.assertEqual(project_desc.contexts_paths[4], "sContext2.json")
        
        self.assertEqual(len(project_desc.executors_paths), 3)
        self.assertEqual(project_desc.executors_paths[0], "executor1.json")
        self.assertEqual(project_desc.executors_paths[1], "executor2.json")
        self.assertEqual(project_desc.executors_paths[2], "executor3.json")
        
    def test_gen_project1(self):
        project_desc = project.load("assets/workspace/project1.json")
        self.assertIsNotNone(project_desc)
        
        project.generate(project_desc)
        
        utils.assert_files(self, "dest/projects/project1/contexts/LContext1Context.h", "assets/expected/lContext1Context.h")
        utils.assert_files(self, "dest/projects/project1/contexts/LContext1Context.cpp", "assets/expected/lContext1Context.cpp")
        utils.assert_files(self, "dest/projects/project1/contexts/LContext2Context.h", "assets/expected/lContext2Context.h")
        utils.assert_files(self, "dest/projects/project1/contexts/LContext2Context.cpp", "assets/expected/lContext2Context.cpp")
        utils.assert_files(self, "dest/projects/project1/contexts/LContext3Context.h", "assets/expected/lContext3Context.h")
        utils.assert_files(self, "dest/projects/project1/contexts/LContext3Context.cpp", "assets/expected/lContext3Context.cpp")
        utils.assert_files(self, "dest/projects/project1/contexts/SContext1Context.h", "assets/expected/sContext1Context.h")
        utils.assert_files(self, "dest/projects/project1/contexts/SContext1Context.cpp", "assets/expected/sContext1Context.cpp")
        utils.assert_files(self, "dest/projects/project1/contexts/SContext2Context.h", "assets/expected/sContext2Context.h")
        utils.assert_files(self, "dest/projects/project1/contexts/SContext2Context.cpp", "assets/expected/sContext2Context.cpp")
        
        utils.assert_files(self, "dest/projects/project1/executors/Executor1Executor.h", "assets/expected/Executor1Executor.h")
        utils.assert_files(self, "dest/projects/project1/executors/Executor1Executor.cpp", "assets/expected/Executor1Executor.cpp")
        utils.assert_files(self, "dest/projects/project1/executors/Executor2Executor.h", "assets/expected/Executor2Executor.h")
        utils.assert_files(self, "dest/projects/project1/executors/Executor2Executor.cpp", "assets/expected/Executor2Executor.cpp")
        utils.assert_files(self, "dest/projects/project1/executors/Executor3Executor.h", "assets/expected/Executor3Executor.h")
        utils.assert_files(self, "dest/projects/project1/executors/Executor3Executor.cpp", "assets/expected/Executor3Executor.cpp")
        
        utils.assert_files(self, "dest/projects/project1/executors/Executor1ExecutorImpl.cpp", "assets/expected/Executor1ExecutorImpl.cpp")
        utils.assert_files(self, "dest/projects/project1/executors/Executor2ExecutorImpl.cpp", "assets/expected/Executor2ExecutorImpl.cpp")
        utils.assert_files(self, "dest/projects/project1/executors/Executor3ExecutorImpl.cpp", "assets/expected/Executor3ExecutorImpl.cpp")
        
        utils.assert_files(self, "dest/projects/project1/runtime.cpp", "assets/expected/runtime.cpp")
                
    def test_gen_project2_with_diff(self):
        project_desc = project.load("assets/project2/project2.json")
        self.assertIsNotNone(project_desc)
        
        contexts_path = "dest/projects/project2/dest/contexts"
        prev_contexts_path = contexts_path + "/prev"
        if not os.path.exists(contexts_path):
            os.makedirs(contexts_path)
        if not os.path.exists(prev_contexts_path):
            os.makedirs(prev_contexts_path)
            
        executors_path = "dest/projects/project2/dest/contexts"
        prev_executors_path = executors_path + "/prev"
        if not os.path.exists(executors_path):
            os.makedirs(executors_path)
        if not os.path.exists(prev_executors_path):
            os.makedirs(prev_executors_path)
        
        shutil.copyfile("assets/expected/project2/prev/lcontext2.json", prev_contexts_path + "/lcontext2.json")
        shutil.copyfile("assets/expected/project2/prev/executor1.json", prev_executors_path + "/executor1.json")
        
        shutil.copyfile("assets/expected/project2/prev/lcontext2.json", prev_contexts_path + "/lcontext2.json")
        shutil.copyfile("assets/expected/project2/prev/executor1.json", prev_executors_path + "/executor1.json")
        
        prev_context_mod_time = get_file_modification_time("assets/expected/project2/prev/lcontext2.json")
        prev_executor_mod_time = get_file_modification_time("assets/expected/project2/prev/executor1.json")
        
        project.generate(project_desc)
        
        utils.assert_files(self, "dest/projects/project2/contexts/LContext1Context.h", "assets/expected/project2/contexts/lContext1Context.h")
        utils.assert_files(self, "dest/projects/project2/contexts/LContext1Context.cpp", "assets/expected/project2/contexts/lContext1Context.cpp")
        utils.assert_files(self, "dest/projects/project2/contexts/LContext2Context.h", "assets/expected/project2/contexts/lContext2Context.h")
        utils.assert_files(self, "dest/projects/project2/contexts/LContext2Context.cpp", "assets/expected/project2/contexts/lContext2Context.cpp")
        
        utils.assert_files(self, "dest/projects/project2/executors/Executor1Executor.h", "assets/expected/project2/executors/Executor1Executor.h")
        utils.assert_files(self, "dest/projects/project2/executors/Executor1Executor.cpp", "assets/expected/project2/executors/Executor1Executor.cpp")
        utils.assert_files(self, "dest/projects/project2/executors/Executor2Executor.h", "assets/expected/project2/executors/Executor2Executor.h")
        utils.assert_files(self, "dest/projects/project2/executors/Executor2Executor.cpp", "assets/expected/project2/executors/Executor2Executor.cpp")
        
        utils.assert_files(self, "dest/projects/project2/executors/Executor1ExecutorImpl.cpp", "assets/expected/project2/executors/Executor1ExecutorImpl.cpp")
        utils.assert_files(self, "dest/projects/project2/executors/Executor2ExecutorImpl.cpp", "assets/expected/project2/executors/Executor2ExecutorImpl.cpp")
        
        utils.assert_files(self, "dest/projects/project2/runtime.cpp", "assets/expected/project2/runtime.cpp")
        
        print(prev_context_mod_time, get_file_modification_time("assets/expected/project2/prev/lcontext2.json"))
        self.assertNotEqual(prev_context_mod_time, get_file_modification_time("assets/expected/project2/prev/lcontext2.json"))
        self.assertNotEqual(prev_executor_mod_time, get_file_modification_time("assets/expected/project2/prev/executor1.json"))
        