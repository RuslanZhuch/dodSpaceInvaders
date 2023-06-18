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

def _generate_commands(handler):
#        for (int32_t cmdId{}; cmdId < Dod::BufferUtils::getNumFilledElements(sApplication.context.commands); ++cmdId)
#        {
#            if (Dod::BufferUtils::get(sApplication.context.commands, 0) == 1)
#            {
#                return;
#            }
#        }

    def cycle_body(handler):
        def exit_body(handler):
            generator.generate_line(handler, "return;")
        generator.generate_block(handler, "if (Dod::BufferUtils::get(sApplication.context.commands, 0) == 1)", exit_body)
        
    generator.generate_block(handler, "for (int32_t cmdId{}; cmdId < Dod::BufferUtils::getNumFilledElements(sApplication.context.commands); ++cmdId)", cycle_body)

def generate(folder, file_name, workspace_shared_contexts_file, workspace_data):
    handler = generate_runtime_file("dest")
    generator.generate_line(handler, "#include <chrono>")
    handler.newline(1)
    
    def fill_function(self, handler):
        _generate_share_contexts_init(handler, workspace_shared_contexts_file)
        handler.newline(1)
        
        executors_data = executors.load("assets/executors")
        executors.gen_inits(handler, executors_data, workspace_data)
    
        def cycle_function(handler):
            generator.generate_line(handler, "const auto start{ std::chrono::high_resolution_clock::now() };")
            handler.newline(1)
            
            executors.gen_updates(handler, executors_data)
            handler.newline(1)
            
            contexts.generate_shared_flush(handler, workspace_data)
            handler.newline(1)
            
            executors.gen_flush(handler, executors_data)
            handler.newline(1)
            
            contexts.generate_shared_merge(handler, workspace_data)
            handler.newline(1)
        
            _generate_commands(handler)
            handler.newline(1)
            
            generator.generate_line(handler, "const auto end{ std::chrono::high_resolution_clock::now() };")
            generator.generate_line(handler, "deltaTime = static_cast<float>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) / 1'000'000'000.f;")
        
        handler.newline(1)
        
        generator.generate_variable(handler, "float", "deltaTime", 0)
        generator.generate_block(handler, "while(true)", cycle_function)
        
    generator.generate_function(handler, "runtime", fill_function)
    handler.close()
    