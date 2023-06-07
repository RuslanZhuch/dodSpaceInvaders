import generator
import executors

def generate_runtime_file(folder):
    return generator.generate_file(folder, 'runtime.cpp')

def generate(folder, file_name):
    handler = generate_runtime_file("dest")
    generator.generate_line(handler, "#include <chrono>")
    handler.newline(1)
    
    def fill_function(self, handler):
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
    