
class TypesCache:
    def __init__(self, type_names, paths):
        self.type_names = type_names
        self.paths = paths
        
    def get_path(self, type_name):
        id = self.type_names.index(type_name)
        if id < 0:
            return "None"
        
        return self.paths[id]
        
def cache_types(types_data):
    cache = dict()
    
    for type_data in types_data:
        data = type_data["data"]
        for type_name in data:
            cache[type_name] = data[type_name]

    type_names = []
    paths = []
    for type_name in cache:
        type_names.append(type_name)
        paths.append(cache[type_name])
    
    return TypesCache(type_names, paths)
