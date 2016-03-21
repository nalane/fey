bl_info = { "name": "Export Fey Model (.fey.model)",
            "author": "Nathaniel Lane",
            "version": (0, 1),
            "blender": (2, 7, 7),
            "location": "File > Export",
            "description": "Export a 3D model for use in the Fey game engine",
            "category": "Import-Export"}

import bpy
from bpy_extras.io_utils import ExportHelper

class Export_FeyModel(bpy.types.Operator, ExportHelper):
    bl_idname = "export_object.fey_model"
    bl_label = "Export Fey Model (.fey.model)"
    
    filename_ext = ".fey.model"
    
    def execute(self, context):
        props = self.properties
        filepath = self.filepath
        filepath = bpy.path.ensure_ext(filepath, self.filename_ext)
        
        obj_list = []
        for obj in bpy.context.selected_objects:
            if hasattr(obj.data, 'vertices'):
                obj_list.append(obj)
        
        if (len(obj_list) != 1):
            print('Script cannot work with multiple objects. Please select only one object!')
            
        else:
            with open(filepath, 'w') as f:
                d = obj_list[0].data
                f.write(str(len(d.vertices)) + "\n")
                for v in d.vertices:
                    f.write(str(v.x) + " " + str(v.y) + " " + str(v.z))
                    f.write('\n')
        
        return {'FINISHED'}
            
    def invoke(self, context, event):
        wm = context.window_manager
        wm.fileselect_add(self)
        return {'RUNNING_MODAL'}
    
def menu_func(self, context):
    self.layout.operator(Export_FeyModel.bl_idname, text="Fey Model (.fey.model)")

def register():
    bpy.utils.register_class(Export_FeyModel)
    bpy.types.INFO_MT_file_export.append(menu_func)
    
def unregister():
    bpy.utils.unregister_class(Export_FeyModel)
    bpy.types.INFO_MT_file_export.remove(menu_func)
    
if __name__ == "__main__":
    register()