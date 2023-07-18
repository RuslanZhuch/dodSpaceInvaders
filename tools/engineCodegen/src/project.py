import loader
import types_manager

import os
import contexts
import executors
import diff
import runtime

from os import path

class ProjectDesc:
    def __init__(self):
        self.project_dest_folder = ""
        self.application_context_path = ""
        self.types_paths = []
        self.contexts_paths = []
        self.executors_paths = []
        self.project_root_src_folder = ""
        self.project_root_src_types = ""
        self.project_contexts_src_folder = ""
        self.project_executors_src_folder = ""

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

def _generate_diff_for_contexts(contexts_folder : str, context_paths : list[str]):
    contexts_folder_prev = contexts_folder + "/prev"
    
    context_names = [path.basename(context_path) for context_path in context_paths]
    diff_list = diff.generate_list(contexts_folder, contexts_folder_prev, context_names)

    diff.replace(contexts_folder, contexts_folder_prev, diff_list)
    
    return diff_list

def _generate_diff_for_executors(executors_folder : str, executors_paths : list[str]):
    executors_folder_prev = executors_folder + "/prev"
    
    context_names = [path.basename(executor_path) for executor_path in executors_paths]
    diff_list = diff.generate_list(executors_folder, executors_folder_prev, context_names)

    diff.replace(executors_folder, executors_folder_prev, diff_list)
    
    return diff_list

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
    project_desc.project_root_src_folder = _get_value(desc_file_data, "project_root_src_folder")
    project_desc.project_root_src_types = _get_value(desc_file_data, "project_root_src_types")
    project_desc.project_contexts_src_folder = _get_value(desc_file_data, "project_contexts_src_folder")
    project_desc.project_executors_src_folder = _get_value(desc_file_data, "project_executors_src_folder")
    
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
    
    contexts_target_path = target_path + "/contexts"
    if not os.path.exists(contexts_target_path):
        os.makedirs(contexts_target_path)
    
    executors_target_path = target_path + "/executors"
    if not os.path.exists(executors_target_path):
        os.makedirs(executors_target_path)
        
    _generate_diff_for_contexts(project_desc.project_contexts_src_folder, project_desc.contexts_paths)
    _generate_diff_for_executors(project_desc.project_executors_src_folder, project_desc.executors_paths)
    
    types_data = [loader.load_file_data(project_desc.project_root_src_types + "/" + types_path) for types_path in project_desc.types_paths]
    types_cache = types_manager.cache_types(types_data)
        
    for context_file in project_desc.contexts_paths:
        full_path = project_desc.project_contexts_src_folder + "/" + context_file
        contexts.generate_context_def(contexts_target_path, full_path, types_cache)
    
    for context_file in project_desc.contexts_paths:
        full_path = project_desc.project_contexts_src_folder + "/" + context_file
        contexts.generate_context_impl(contexts_target_path, full_path)
                
    executor_full_paths = [project_desc.project_executors_src_folder + "/" + executor_path for executor_path in project_desc.executors_paths]
    executors_data = executors.load(executor_full_paths)
    for data in executors_data:
        executors.gen_header(executors_target_path, data)
        executors.gen_source(executors_target_path, data)
        executors.gen_implementation(executors_target_path, data)
        
    application_context_path = project_desc.project_root_src_folder + "/" + project_desc.application_context_path
    
    contexts_full_paths = [project_desc.project_contexts_src_folder + "/" + context_path for context_path in project_desc.contexts_paths]
    contexts_data = contexts.load_contexts(contexts_full_paths)
    runtime.generate(target_path, executors_data, application_context_path, contexts_data)
    