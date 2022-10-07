using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;
using System;

public class Game : MonoBehaviour
{
    [DllImport("CXEngine")]
    private static extern void OnGameUpdate(float t);

    [DllImport("CXEngine")]
    private static extern IntPtr GetRenderEventFunc();

    [DllImport("CXEngine")]
    private static extern void OnGameStart();
    [DllImport("CXEngine")]
    private static extern void OnGameEnd();

    // Start is called before the first frame update
    IEnumerator Start()
    {
        OnGameStart();
        yield return StartCoroutine("CallPluginAtEndOfFrames");
    }

    private IEnumerator CallPluginAtEndOfFrames()
    {
        while (true)
        {
            // Wait until all frame rendering is done
            yield return new WaitForEndOfFrame();

            // Set time for the plugin
            OnGameUpdate(Time.timeSinceLevelLoad);

            // Issue a plugin event with arbitrary integer identifier.
            // The plugin can distinguish between different
            // things it needs to do based on this ID.
            // For our simple plugin, it does not matter which ID we pass here.
            GL.IssuePluginEvent(GetRenderEventFunc(), 1);
        }
    }

    private void OnDestroy()
    {
        OnGameEnd();
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
