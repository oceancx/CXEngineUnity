using UnityEngine;
using UnityEditor;
using UnityEditorInternal;

namespace ImGuiNET.Unity.Editor
{
    [CustomEditor(typeof(FontAtlasConfigAsset))]
    class FontAtlasConfigAssetEditor : UnityEditor.Editor
    {
        static class Styles
        {
            public static GUIContent rasterizer = new GUIContent("Rasterizer", "Build font atlases using a different rasterizer.");
            public static GUIContent rasterizerFlags = new GUIContent("Rasterizer Flags", "Settings for custom font rasterizer. Forces flags on all fonts.");
            public static GUIContent fonts = new GUIContent("Fonts", "Fonts to pack into the atlas texture.");
        }

        SerializedProperty _rasterizer;
        SerializedProperty _rasterizerFlags;
        SerializedProperty _fonts;

        ReorderableList _fontsList;

        void OnEnable()
        {
            _rasterizer = serializedObject.FindProperty(nameof(FontAtlasConfigAsset.Rasterizer));
            _rasterizerFlags = serializedObject.FindProperty(nameof(FontAtlasConfigAsset.RasterizerFlags));
            _fonts = serializedObject.FindProperty(nameof(FontAtlasConfigAsset.Fonts));

            _fontsList = new ReorderableList(serializedObject, _fonts, true, true, true, true)
            {
                elementHeightCallback = (i) => EditorGUI.GetPropertyHeight(_fontsList.serializedProperty.GetArrayElementAtIndex(i)) + EditorGUIUtility.standardVerticalSpacing,
                drawHeaderCallback = (Rect rect) => EditorGUI.LabelField(rect, Styles.fonts),
                drawElementCallback = (Rect rect, int i, bool isActive, bool isFocused) =>
                {
                    if (i % 2 != 0)
                        EditorGUI.DrawRect(new Rect(rect.x - 19f, rect.y, rect.width + 23f, rect.height), new Color(0, 0, 0, 0.1f));
                    EditorGUI.PropertyField(rect, _fontsList.serializedProperty.GetArrayElementAtIndex(i));
                },
                onAddCallback = (li) =>
                {
                    var index = li.index >= 0 && li.index < li.count ? li.index : li.count;
                    li.serializedProperty.InsertArrayElementAtIndex(index);
                    // HACK to get default values for the new font config
                    serializedObject.ApplyModifiedProperties();
                    if (serializedObject.targetObject is FontAtlasConfigAsset atlas)
                        atlas.Fonts[index].Config.SetDefaults();
                    serializedObject.Update();
                },
            };
        }

        public override void OnInspectorGUI()
        {
            serializedObject.Update();

            EditorGUILayout.PropertyField(_rasterizer, Styles.rasterizer);
            DrawRasterizerFlagsProperty();
            _fontsList.DoLayoutList();

            serializedObject.ApplyModifiedProperties();
        }

        void DrawRasterizerFlagsProperty()
        {
#if IMGUI_FEATURE_FREETYPE
            if (_rasterizer.intValue == (int)FontRasterizerType.FreeType)
            {
                EditorGUI.BeginChangeCheck();
                var value = (ImFreetype.RasterizerFlags)EditorGUILayout.EnumFlagsField(Styles.rasterizerFlags, (ImFreetype.RasterizerFlags)_rasterizerFlags.intValue);
                if (EditorGUI.EndChangeCheck())
                    _rasterizerFlags.intValue = (int)value;
            }
            else
            {
                EditorGUILayout.PropertyField(_rasterizerFlags, Styles.rasterizerFlags);
            }
#else
            EditorGUILayout.PropertyField(_rasterizerFlags, Styles.rasterizerFlags);
#endif
        }
    }
}
