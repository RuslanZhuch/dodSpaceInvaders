import loader
import generator

def _to_camel_case(text):
    s = text.replace("-", " ").replace("_", " ")
    s = s.split()
    if len(text) == 0:
        return text
    return s[0] + ''.join(i.capitalize() for i in s[1:])

def _to_double_camel_case(text):
    camel = _to_camel_case(text)
    return camel.capitalize()

def load(folder):
    return loader.load_descriptors(folder)

def get_name(data):
    return data.get("name")

def gen_inits(handler, executors_data):
    for data in executors_data:
        name = get_name(data)
        class_name = "Game::ExecutionBlock::" + _to_double_camel_case(name)
        generator.generate_variable(handler, class_name, name)
        