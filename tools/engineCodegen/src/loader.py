import json

from os import path
from os import walk
import ntpath

def _path_leaf(path):
    head, tail = ntpath.split(path)
    return tail or ntpath.basename(head)

def load_descriptor(file_path):
    descriptor_file = open(file_path)
    data = json.load(descriptor_file)
    data["name"] = path.splitext(_path_leaf(file_path))[0]
    return data

def load_descriptors(folder_path):
    data = []
    for (dir_path, dir_names, file_names) in walk(folder_path):
        for file_name in file_names:
            if dir_path == folder_path:
                data.append(load_descriptor(dir_path + "/" + file_name))
            
    return data
