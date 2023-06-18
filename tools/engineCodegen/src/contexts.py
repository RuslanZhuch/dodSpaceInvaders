
import loader
import generator

class SharedMerge:
    def __init__(self, executor_name, executor_scontext):
        self.executor_name = executor_name
        self.executor_scontext = executor_scontext

def _to_double_camel_case(text):
    s = text.replace("-", " ").replace("_", " ")
    s = s.split()
    if len(text) <= 1:
        return text
    if len(s) <= 1:
        return text[0].capitalize() + text[1:]
    out = ''.join(i.capitalize() for i in s[:])
    return out

def _to_class_name(name):
    return _to_double_camel_case(name)

def load_shared(folder):
    return loader.load_shared_contexts(folder)

class ContextUsage:
    def __init__(self, context_name, instance_name):
        self.context_name = context_name
        self.instance_name = instance_name

def load_shared_context_instances(file):
    content = loader.load_shared_contexts_usage(file)
    
    usage_data = []
    for usage in content["sharedContextsInstances"]:
        usage_data.append(ContextUsage(usage["contextName"], usage["instanceName"]))
    return usage_data
    
def load_shared_context_to_flush(workspace_data):
    return workspace_data["sharedContextsInstancesToFlush"]
    
def generate_shared_flush(handler, workspace_data):
    flush_data = load_shared_context_to_flush(workspace_data)
    for context in flush_data:
        generator.generate_line(handler, "Dod::SharedContext::flush(&{}Context);".format(context))
    
def load_shared_context_merge(workspace_data):    
    output = dict()
    
    merge_data_full = workspace_data["sharedContextsUsage"]
    if merge_data_full is None:
        return output
    
    for element in merge_data_full:
        executor_name = element["executorName"]
        shared_instance = element["instanceName"]
        executor_scontext = element["executorSharedName"]
        if output.get(shared_instance) is None:
            output[shared_instance] = []    
        output[shared_instance].append(SharedMerge(executor_name, executor_scontext))
        
    return output
    
def get_validated_shared_context_instances(shared_contexts_data, shared_contexts_instances):
    loaded_names = [el.get("name") for el in shared_contexts_data]
    
    validated = []
    for instance in shared_contexts_instances:
        if loaded_names.count(instance.context_name) == 0:
            continue
        validated.append(instance)
        
    return validated
    
def generate_shared_init(handler, contexts_data):
    for instance in contexts_data:
        class_name = _to_class_name(instance.context_name)
        generator.generate_line(handler, "Dod::SharedContext::Controller<Game::Context::{}::Shared> {}Context;".format(class_name, instance.instance_name))
    
def generate_shared_merge(handler, workspace_data):
    merge_data = load_shared_context_merge(workspace_data)
    if merge_data is None:
        return
    
    for instance_name in merge_data:
        merge_context_full = merge_data[instance_name]
        for merge_context in merge_context_full:
            executor_name = merge_context.executor_name
            executor_scontext = merge_context.executor_scontext
            generator.generate_line(handler, "Dod::SharedContext::merge(&{}Context, {}.{}Context);".format(instance_name, executor_name, executor_scontext))
