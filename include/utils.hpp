


template<typename Class, typename Param, void(Class::*funct)(Param)>
struct GtkProxy {
    static void callback(Param p, gpointer c) {
        (static_cast<Class*>(c)->*funct)(p);
    }
};