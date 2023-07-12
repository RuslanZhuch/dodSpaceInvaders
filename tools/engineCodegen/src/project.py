import loader
import types_manager

import os
import contexts
import executors
import runtime

class ProjectDesc:
    def __init__(self):
        self.project_dest_folder = ""
        self.application_context_path = ""
        self.types_paths = []
        self.contexts_paths = []
        self.executors_paths = []

def _get_value(data, field):
    value = data.get(field)
    if value is not None:
        return value
    return ""

def _get_array(data, field):
    array_data = data.get(field)
    if array_data is not None:
        return array_data
    
    return []

def load(project_desc_file):
    desc_file_data = loader.load_file_data(project_desc_file)
    
    project_desc = ProjectDesc()
    if desc_file_data is None:
        return project_desc
    
    project_desc.project_dest_folder = _get_value(desc_file_data, "project_dest_folder")
    project_desc.application_context_path = _get_value(desc_file_data, "application_context_path")
    project_desc.types_paths = _get_array(desc_file_data, "types_paths")
    project_desc.contexts_paths = _get_array(desc_file_data, "contexts_paths")
    project_desc.executors_paths = _get_array(desc_file_data, "executors_paths")
    
    return project_desc
    
def generate(project_desc : ProjectDesc):
    if project_desc is None:
        return
    
    target_path = project_desc.project_dest_folder
    if target_path == "":
        return
    
    #os.mkdir(target_path)
    if not os.path.exists(target_path):
        os.makedirs(target_path)
    
    types_data = [loader.load_file_data(types_path) for types_path in project_desc.types_paths]
    types_cache = types_manager.cache_types(types_data)
    
    contexts_target_path = target_path + "/contexts"
    if not os.path.exists(contexts_target_path):
        os.makedirs(contexts_target_path)
    
    for context_file in project_desc.contexts_paths:
        contexts.generate_context_def(contexts_target_path, context_file, types_cache)
    
    for context_file in project_desc.contexts_paths:
        contexts.generate_context_impl(contexts_target_path, context_file)
        
    executors_target_path = target_path + "/executors"
    if not os.path.exists(executors_target_path):
        os.makedirs(executors_target_path)
        
    executors_data = executors.load(project_desc.executors_paths)
    for data in executors_data:
        executors.gen_header(executors_target_path, data)
        executors.gen_source(executors_target_path, data)
        executors.gen_implementation(executors_target_path, data)
        
    application_context_path = project_desc.application_context_path
    
    contexts_data = contexts.load_contexts(project_desc.contexts_paths)
    runtime.generate(target_path, executors_data, application_context_path, contexts_data)
    