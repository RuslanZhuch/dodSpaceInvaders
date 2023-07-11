import unittest

from os import path
import sys
 
from pathlib import Path
root_dir = Path.cwd()
sys.path.append(root_dir)

import sys
sys.path.append("../src")

import utils
import generator
import loader
import types_manager
import project

class TestProject(unittest.TestCase):
    def __init__(self, methodName: str = "runTest") -> None:
        super().__init__(methodName)
        self.maxDiff = None
    
    def test_decode_project1_file(self):
        project_desc = project.load("assets/workspace/project1.json")
        
        self.assertIsNotNone(project_desc)
        
        self.assertEqual(project_desc.project_dest_folder, "dest/projects/project1")
        
        self.assertEqual(project_desc.application_context_path, "assets/workspace/ws_applicationContext.json")
        
        self.assertEqual(len(project_desc.types_paths), 2)
        self.assertEqual(project_desc.types_paths[0], "assets/workspace/types_default.json")
        self.assertEqual(project_desc.types_paths[1], "assets/workspace/types_contexts_data.json")
        
        self.assertEqual(len(project_desc.contexts_paths), 5)
        self.assertEqual(project_desc.contexts_paths[0], "assets/contexts/local/lContext1.json")
        self.assertEqual(project_desc.contexts_paths[1], "assets/contexts/local/lContext2.json")
        self.assertEqual(project_desc.contexts_paths[2], "assets/contexts/local/lContext3.json")
        self.assertEqual(project_desc.contexts_paths[3], "assets/contexts/shared/sContext1.json")
        self.assertEqual(project_desc.contexts_paths[4], "assets/contexts/shared/sContext2.json")
        
        self.assertEqual(len(project_desc.executors_paths), 3)
        self.assertEqual(project_desc.executors_paths[0], "assets/executors/executor1.json")
        self.assertEqual(project_desc.executors_paths[1], "assets/executors/executor2.json")
        self.assertEqual(project_desc.executors_paths[2], "assets/executors/executor3.json")
        
    def test_gen_project1_(self):
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
        