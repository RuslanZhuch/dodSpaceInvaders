
import loader
import generator

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
    for usage in content["usage"]:
        usage_data.append(ContextUsage(usage["contextName"], usage["instanceName"]))
    return usage_data
    
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
