#ifndef cesources_h_
#define cesources_h_

// Macro for declaring the unsigned char array and array size variables for the given resource
// embedded by cesources.
#define cesources_declare_var(resource_name)    \
    extern const unsigned char resource_name[]; \
    extern const int resource_name##_size

#endif  // cesources_h_
