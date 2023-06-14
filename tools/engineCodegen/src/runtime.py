import generator
import executors
import contexts

def generate_runtime_file(folder):
    return generator.generate_file(folder, 'runtime.cpp')

def _generate_share_contexts_init(handler, workspace_shared_contexts_file):
    contexts_data = contexts.load_shared("assets/contexts/shared")
    instances_data = contexts.load_shared_context_instances(workspace_shared_contexts_file)
    validated_instances = contexts.get_validated_shared_context_instances(contexts_data, instances_data)
    
    contexts.generate_shared_init(handler, validated_instances)

def generate(folder, file_name, workspace_shared_contexts_file):
    handler = generate_runtime_file("dest")
    generator.generate_line(handler, "#include <chrono>")
    handler.newline(1)
    
    def fill_function(self, handler):
        _generate_share_contexts_init(handler, workspace_shared_contexts_file)
        handler.newline(1)
        
        executors_data = executors.load("assets/executors")
        executors.gen_inits(handler, executors_data)
    
        def cycle_function(handler):
            generator.generate_line(handler, "const auto start{ std::chrono::high_resolution_clock::now() };")
            handler.newline(1)
            
            executors.gen_updates(handler, executors_data)
            handler.newline(1)
            
            executors.gen_flush(handler, executors_data)
            handler.newline(1)
            
            generator.generate_line(handler, "const auto end{ std::chrono::high_resolution_clock::now() };")
            generator.generate_line(handler, "deltaTime = static_cast<float>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) / 1'000'000'000.f;")
        
        handler.newline(1)
        
        generator.generate_variable(handler, "float", "deltaTime", 0)
        generator.generate_block(handler, "while(true)", cycle_function)
        
    generator.generate_function(handler, "runtime", fill_function)
    handler.close()
    