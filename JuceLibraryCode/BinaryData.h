/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   gs_bold_ttf;
    const int            gs_bold_ttfSize = 62492;

    extern const char*   gs_italic_ttf;
    const int            gs_italic_ttfSize = 78304;

    extern const char*   gs_light_ttf;
    const int            gs_light_ttfSize = 99540;

    extern const char*   gs_regular_ttf;
    const int            gs_regular_ttfSize = 71788;

    extern const char*   pcd_ttf;
    const int            pcd_ttfSize = 15044;

    extern const char*   DavidSuLogo_png;
    const int            DavidSuLogo_pngSize = 287997;

    extern const char*   MelodrumaticTitle_png;
    const int            MelodrumaticTitle_pngSize = 69028;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 7;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
