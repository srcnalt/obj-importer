using System;
using System.IO;
using UnityEngine;
using System.Runtime.InteropServices;

public class ModelLoader : MonoBehaviour
{
    [DllImport("ObjImporter")] private static extern void ImportObj(string path);
    [DllImport("ObjImporter")] private static extern IntPtr GetSizes();
    [DllImport("ObjImporter")] private static extern IntPtr GetVertices();
    [DllImport("ObjImporter")] private static extern IntPtr GetNormals();
    [DllImport("ObjImporter")] private static extern IntPtr GetTris();
    [DllImport("ObjImporter")] private static extern IntPtr GetUVs();
    [DllImport("ObjImporter")] private static extern void ImportMat();
    [DllImport("ObjImporter")] private static extern IntPtr GetColor();
    [DllImport("ObjImporter")] private static extern float GetGloss();
    [DllImport("ObjImporter")] private static extern IntPtr GetTexture();

    [StructLayout(LayoutKind.Sequential)]
    struct Point3
    {
        public float x;
        public float y;
        public float z;

        public Vector3 Vector3()
        {
            return new Vector3(x, y, z);
        }

        public static Vector3[] Convert(Point3[] arr)
        {
            Vector3[] vArr = new Vector3[arr.Length];

            for (int i = 0; i < arr.Length; i++)
            {
                vArr[i] = new Vector3(arr[i].x, arr[i].y, arr[i].z);
            }

            return vArr;
        }
    };

    [StructLayout(LayoutKind.Sequential)]
    struct Point2
    {
        public float x;
        public float y;

        public Vector2 Vector3()
        {
            return new Vector2(x, y);
        }

        public static Vector2[] Convert(Point2[] arr)
        {
            Vector2[] vArr = new Vector2[arr.Length];

            for (int i = 0; i < arr.Length; i++)
            {
                vArr[i] = new Vector2(arr[i].x, arr[i].y);
            }

            return vArr;
        }

    };

    [StructLayout(LayoutKind.Sequential)]
    struct Size
    {
        public int vSize;
        public int nSize;
        public int tSize;
        public int uSize;
    }

    private string path;
    private MeshFilter meshFilter;
    private MeshRenderer meshRenderer;

    private void OnEnable()
    {
        meshFilter = GetComponent<MeshFilter>();
        meshRenderer = GetComponent<MeshRenderer>();
    }

    public void LoadModel(string path)
    {
        this.path = path;

        LoadObj();
        LoadMat();
        RepositionObject();
    }

    private void LoadObj()
    {
        ImportObj(path);

        IntPtr sizePtr = GetSizes();
        Size sizes = (Size)Marshal.PtrToStructure(new IntPtr(sizePtr.ToInt32()), typeof(Size));

        IntPtr vertPtr = GetVertices();
        Point3[] vertsT = GetElements<Point3>(vertPtr, sizes.vSize);
        Vector3[] verts = Point3.Convert(vertsT);

        IntPtr normPtr = GetNormals();
        Point3[] normsT = GetElements<Point3>(normPtr, sizes.nSize);
        Vector3[] norms = Point3.Convert(normsT);

        IntPtr uvPtr = GetUVs();
        Point2[] uvsT = GetElements<Point2>(uvPtr, sizes.uSize);
        Vector2[] uvs = Point2.Convert(uvsT);

        IntPtr triPtr = GetTris();
        int[] tris = GetElements<int>(triPtr, sizes.tSize);

        Mesh mesh = new Mesh();

        mesh.vertices = verts;
        mesh.uv = uvs;
        mesh.normals = norms;
        mesh.triangles = tris;
        mesh.RecalculateBounds();

        meshFilter.mesh = mesh;
    }

    private void LoadMat()
    {
        ImportMat();

        IntPtr colorPtr = GetColor();
        Point3 color = (Point3)Marshal.PtrToStructure(new IntPtr(colorPtr.ToInt32()), typeof(Point3));

        float gloss = GetGloss();

        IntPtr texPtr = GetTexture();
        string texture = Marshal.PtrToStringAnsi(texPtr);

        Material material = new Material(Shader.Find("Standard"));
        material.SetColor("_Color", new Color(color.x, color.y, color.z));
        material.SetFloat("_Glossiness", gloss);
        material.mainTexture = Loadtexture(texture);

        meshRenderer.material = material;
    }

    private void RepositionObject()
    {
        Vector3 bounds = meshFilter.mesh.bounds.extents;
        float max = Mathf.Max(new float[] { bounds.x, bounds.y, bounds.z });

        transform.localScale = Vector3.one / max;
        transform.position = Vector3.zero;
        transform.Translate(meshFilter.mesh.bounds.center / max * -1);
    }

    private T[] GetElements<T>(IntPtr address, int arraySize)
    {
        int offset = 0;
        int size = Marshal.SizeOf(typeof(T));
        T[] elems = new T[arraySize];

        for (int i = 0; i < arraySize; i++)
        {
            elems[i] = (T)Marshal.PtrToStructure(new IntPtr(address.ToInt32() + offset), typeof(T));
            offset += size;
        }

        return elems;
    }

    private Texture Loadtexture(string path)
    {
        Texture2D texture = null;
        byte[] data;

        if (File.Exists(path))
        {
            data = File.ReadAllBytes(path);
            texture = new Texture2D(2, 2);
            texture.LoadImage(data);
        }

        return texture;
    }
}
