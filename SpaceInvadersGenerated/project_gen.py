from pathlib import Path
root_dir = Path.cwd()

import sys
sys.path.append(root_dir)
sys.path.append("../tools/engineCodegen/src")
sys.path.append("../tools/thirdparty/code_generator/src")

import project

if __name__ == "__main__":
    project_desc = project.load("workspace/project.json")
    
    if project_desc:
        project.generate(project_desc)
    