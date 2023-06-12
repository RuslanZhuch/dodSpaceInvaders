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

def _to_class_name(name):
    return _to_double_camel_case(name)

def load(folder):
    return loader.load_descriptors(folder)

def get_name(data):
    return data.get("name")

def gen_inits(handler, executors_data):
    for data in executors_data:
        name = get_name(data)
        class_name = "Game::ExecutionBlock::" + _to_class_name(name)
        generator.generate_variable(handler, class_name, name)
        generator.generate_line(handler, name + ".loadContext();")
        generator.generate_line(handler, name + ".initiate();")
        
def gen_updates(handler, executors_data):
    for data in executors_data:
        name = get_name(data)
        generator.generate_line(handler, name + ".update(deltaTime);")
        
def gen_flush(handler, executors_data):
    for data in executors_data:
        name = get_name(data)
        generator.generate_line(handler, name + ".flushSharedLocalContexts();")
        
def gen_body_flush(handler, executor_data):
    contexts_to_flush = executor_data.get("contextsToFlush")
    if contexts_to_flush is None:
        return
    
    for flush_context in contexts_to_flush:
        generator.generate_line(handler, "this->{}Context.reset();".format(flush_context))
        
def gen_body_init(handler, executor_data):
    contexts_write_to = executor_data.get("contextsWriteTo")
    if contexts_write_to is None:
        return
    
    for init_context in contexts_write_to:
        generator.generate_line(handler, "this->{}Context.init();".format(init_context))
        
def gen_header(folder, executor_data):
    executor_name = get_name(executor_data)
    class_name = _to_class_name(executor_name)
    file_name = class_name + "Executor.h"
    handler = generator.generate_file(folder, file_name)
    
    generator.generate_line(handler, "#pragma once")
    generator.generate_empty(handler)
    
    generator.generate_line(handler, "#include <dod/MemPool.h>")
    generator.generate_empty(handler)
    
    def namespace_block_data(handler):
        def class_data(class_handler):
            generator.generate_class_public_method(class_handler, "loadContext", "void", [], False)
            generator.generate_class_public_method(class_handler, "initiate", "void", [], False)
            generator.generate_class_public_method(class_handler, "update", "void", ['float dt'], False)
            generator.generate_class_public_method(class_handler, "flushSharedLocalContexts", "void", [], False)
            
            generator.generate_class_variable(class_handler, "Dod::MemPool", "memory")
                        
        generator.generate_class(handler, class_name, class_data)
        
    generator.generate_block(handler, "namespace Game::ExecutionBlock", namespace_block_data)
    
def gen_source(folder, executor_data):
    executor_name = get_name(executor_data)
    class_name = _to_class_name(executor_name)
    file_name_header = class_name + "Executor.h"
    file_name_source = class_name + "Executor.cpp"
    handler = generator.generate_file(folder, file_name_source)

    generator.generate_line(handler, "#include \"{}\"".format(file_name_header))
    generator.generate_empty(handler)
    
    def namespace_block_data(handler):
        generator.generate_empty(handler)
        def class_data(class_handler):
            generator.generate_class_public_method(class_handler, "loadContext", "void", [], False)
            
            def init_body(self, handler):
                gen_body_init(handler, executor_data)
            generator.generate_class_public_method(class_handler, "initiate", "void", [], False, init_body)
            generator.generate_class_public_method(class_handler, "update", "void", ['float dt'], False)
            
            def flush_body(self, handler):
                gen_body_flush(handler, executor_data)
            generator.generate_class_public_method(class_handler, "flushSharedLocalContexts", "void", [], False, flush_body)
                        
        generator.generate_class_impl(handler, class_name, class_data)
        
    generator.generate_block(handler, "namespace Game::ExecutionBlock", namespace_block_data)
    