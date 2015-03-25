namespace GSignond {
    [CCode (cheader_filename = "gsignond/gsignond.h")]
    public class Dictionary : GLib.HashTable<string, GLib.Variant> {
        [CCode (has_construct_function = false)]
        public Dictionary ();
        public Dictionary.new_from_variant (GLib.Variant variant);
        public GLib.Variant to_variant ();
        public unowned GLib.VariantBuilder to_variant_builder ();
        public unowned GLib.Variant get (string key);
        public bool set (string key, GLib.Variant value);
        public bool get_boolean (string key, out bool value);
        public bool set_boolean (string key, bool value);
        public bool get_int32 (string key, out int value);
        public bool set_int32 (string key, int value);
        public bool get_uint32 (string key, out uint value);
        public bool set_uint32 (string key, uint32 value);
        public bool get_int64 (string key, out int64 value);
        public bool set_int64 (string key, int64 value);
        public bool get_uint64 (string key, out uint64 value);
        public bool set_uint64 (string key, uint64 value);
        public unowned string get_string (string key);
        public bool set_string (string key, string value);
        public bool remove (string key);
        public bool contains (string key);
    }

    [CCode (cheader_filename = "gsignond/gsignond.h", free_function = "gsignond_security_context_list_free")]
    public class SecurityContextList : GLib.List<GSignond.SecurityContext> {
        public SecurityContextList.from_variant (GLib.Variant variant);
        public GLib.Variant to_variant ();
        public SecurityContextList copy ();
    }
}
