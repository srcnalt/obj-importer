# obj-importer
DLL for importing .obj files with material and texture.

The project is based on [Unity's ObjImporter](http://wiki.unity3d.com/index.php/ObjImporter), the code is converted into C++, simplified and cleaned.
In addition to .obj loading, material loading and texture address extracing added to the DLL. 

## Instructions
- Place the [ObjLoader.dll](https://github.com/srcnalt/obj-importer/blob/master/Build/ObjImporter.dll) & [ModelLoader.cs](https://github.com/srcnalt/obj-importer/blob/master/Loader/ModelLoader.cs) files into your Unity project.
- Create an `Empty Game Object` in the scene and attach a `MeshFilter` and a `MeshRenderer` to it.
- Attach the `ModelLoader` script to this game object.
- Call the `LoadModel` method from this script using the path of the model as a string parameter.
