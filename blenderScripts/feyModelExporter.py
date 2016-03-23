bl_info = { "name": "Export Fey Model (.fey.model)",
            "author": "Nathaniel Lane",
            "version": (0, 1),
            "blender": (2, 7, 7),
            "location": "File > Export",
            "description": "Export a 3D model for use in the Fey game engine",
            "category": "Import-Export"}

import bpy
from bpy_extras.io_utils import ExportHelper

def polygonToTriangles(polygon):
    index_list = []
    for v in polygon.vertices:
        if len(index_list) >= 3:
            index_list.append(index_list[0])
            index_list.append(index_list[-2])          
        index_list.append(v)
        
    return index_list

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
                    f.write(str(v.co.x) + " " + str(v.co.y) + " " + str(v.co.z))
                    f.write('\n')
                   
                triangleVertexIndices = []
                for p in d.polygons:
                    triangleVertexIndices = triangleVertexIndices + polygonToTriangles(p)
                    
                f.write(str(len(triangleVertexIndices)) + "\n")
                for v in triangleVertexIndices:
                    f.write(str(v) + "\n")
                    
                f.write(str(len(d.uv_layers)) + "\n")
                for i, l in enumerate(d.uv_layers):
                    f.write(d.uv_textures[i].data[0].image.filepath + "\n")
                    seen_verts = set()
                    for index, loop in enumerate(l.data):
                        vert_idx = d.loops[index].vertex_index
                        if (vert_idx not in seen_verts):
                            f.write(str(vert_idx) + " ")
                            f.write(str(loop.uv.x) + " ")
                            f.write(str(loop.uv.y) + "\n")
                            seen_verts.add(vert_idx)
        
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