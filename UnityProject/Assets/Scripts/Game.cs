using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;
using System;
using SLua;
using ImGuiNET;
using UnityEngine.Assertions.Must;
using System.IO;
public class Game : MonoBehaviour
{
    [DllImport("CXEngine")]
    private static extern IntPtr GetRenderEventFunc();
    [DllImport("CXEngine")]
    private static extern void script_system_init(IntPtr L);
    [DllImport("CXEngine")]
    private static extern void script_system_update(float t);
    [DllImport("CXEngine")]
    private static extern void script_system_deinit();
    [DllImport("CXEngine")]
    private static extern void script_system_draw_ui();
    public static float mouseX;
    public static  float mouseY;
    LuaSvr luaS;
    // Start is called before the first frame update
    IEnumerator Start()
    {
        luaS = new LuaSvr();
        LuaSvr.mainState.loaderDelegate = (string fn, ref string absoluteFn) =>
        {
            byte[] bytes = File.ReadAllBytes(fn);
            return bytes;
        };
        luaS.init(null, () => { });
        script_system_init(LuaSvr.mainState.L);
        
        ImGuiUn.Layout += OnImGuiLayout;
        yield return StartCoroutine("CallPluginAtEndOfFrames"); 
    }
    void OnImGuiLayout()
    {
        mouseX = ImGui.GetMousePos().x;
        mouseY = ImGui.GetMousePos().y;
        //ImGui.ShowDemoWindow();
        //script_system_draw_ui();
    } 
    private IEnumerator CallPluginAtEndOfFrames()
    {
        while (true)
        {
            // Wait until all frame rendering is done
            yield return new WaitForEndOfFrame();
            // Set time for the plugin
            // OnGameUpdate(Time.timeSinceLevelLoad);
            script_system_update(Time.timeSinceLevelLoad);
            // Issue a plugin event with arbitrary integer identifier.
            // The plugin can distinguish between different
            // things it needs to do based on this ID.
            // For our simple plugin, it does not matter which ID we pass here.
            GL.IssuePluginEvent(GetRenderEventFunc(), 1);
        }
    }

    private void OnDestroy()
    {
        script_system_deinit();
        ImGuiUn.Layout -= OnImGuiLayout;
    }

    // Update is called once per frame
    void Update()
    {
       
    }
}

