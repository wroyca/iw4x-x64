#include <libiw4x/imgui.hxx>

#include <dbghelp.h>

#include <mutex>
#include <stdexcept>

using namespace std;

namespace iw4x
{
  // Static member initialization
  //
  // This static instance is required for Win32/D3D9 API callbacks. This is not
  // a design choice but a technical necessity imposed by the platform APIs
  // we're intercepting.
  //
  imgui* imgui::instance_ (nullptr);

  imgui::imgui ()
  {
    instance_ = this;

    //
    //
    intercept_d3d9 ();
  }

  void
  imgui::intercept_d3d9 ()
  {
    void* direct3d_create9_slot (nullptr);

    // Passing nullptr to GetModuleHandle means "give me the handle for the
    // file that started this process", in practice, this is the image base
    // address in memory.
    //
    HMODULE handle (GetModuleHandle (nullptr));

    // ImageNtHeader() takes a pointer to the DOS header and finds
    // the NT header using the e_lfanew offset. From here on, we have
    // a full view of section headers and data directories.
    //
    PIMAGE_NT_HEADERS nt_headers (
      ImageNtHeader (reinterpret_cast<PIMAGE_DOS_HEADER> (handle)));

    // For a loaded PE image in memory, RVAs can be simply converted to
    // pointers by adding them to the module base address, since the loader
    // has already mapped the sections to their virtual addresses.
    //
    auto rva_to_pointer = [handle] (unsigned long rva) -> void*
    {
      return reinterpret_cast<char*> (handle) + rva;
    };

    // The OptionalHeader's DataDirectory table has an entry for the import
    // table (IMAGE_DIRECTORY_ENTRY_IMPORT). Its VirtualAddress is an RVA
    // pointing to the first IMAGE_IMPORT_DESCRIPTOR in the array. Each
    // descriptor represents one DLL that this module imports from.
    //
    PIMAGE_IMPORT_DESCRIPTOR import_descriptor (
      static_cast<PIMAGE_IMPORT_DESCRIPTOR> (rva_to_pointer (
        nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT]
          .VirtualAddress)));

    // Walk each imported DLL until we find d3d9.dll.
    //
    for (; import_descriptor->Name != 0; ++import_descriptor)
    {
      char *dll_name (
        static_cast<char *> (rva_to_pointer (import_descriptor->Name)));

      // Case-insensitive match because the import name may differ in
      // case between the linker and the loader.
      //
      if (_stricmp (dll_name, "d3d9.dll") == 0)
      {
        // Once we have the right DLL, we need both the name table
        // (OriginalFirstThunk) and the address table (FirstThunk).
        //
        // OriginalFirstThunk is the lookup table the loader uses to find the
        // right address at load time, it points to IMAGE_IMPORT_BY_NAME
        // structures that store the function names.
        //
        // FirstThunk is the table that actually gets patched by the loader with
        // the resolved addresses from the target DLL, this is the IAT we're
        // going to patch.
        //
        IMAGE_THUNK_DATA* original_first_thunk (
          static_cast<IMAGE_THUNK_DATA*> (
            rva_to_pointer (import_descriptor->OriginalFirstThunk)));

        IMAGE_THUNK_DATA* first_thunk (
          static_cast<IMAGE_THUNK_DATA*> (
            rva_to_pointer (import_descriptor->FirstThunk)));

        // We stop when AddressOfData is zero (end of import list). For
        // ordinals (high bit set), there's no name to check, so we skip
        // those.
        //
        for (; original_first_thunk->u1.AddressOfData != 0;
             ++original_first_thunk, ++first_thunk)
        {
          if ((original_first_thunk->u1.Ordinal & IMAGE_ORDINAL_FLAG) == 0U)
          {
            // Translate AddressOfData to an IMAGE_IMPORT_BY_NAME.
            //
            IMAGE_IMPORT_BY_NAME* import_by_name (
              static_cast<IMAGE_IMPORT_BY_NAME*> (
                rva_to_pointer (original_first_thunk->u1.AddressOfData)));

            // Check for the symbol we want, this must match exactly,
            // because IAT slots are per-function.
            //
            if (strcmp (reinterpret_cast<const char*> (import_by_name->Name),
                       "Direct3DCreate9") == 0)
            {
              // Found it, store the address of the live function pointer
              // slot. This is where the loader wrote the real address,
              // and where we'll write ours.
              //
              direct3d_create9_slot = &first_thunk->u1.Function;
              break;
            }
          }
        }
        break;
      }
    }

    if (direct3d_create9_slot == nullptr)
      throw runtime_error ("unable to locate Direct3DCreate9 slot");

    // The IAT is mapped read-only, so we temporarily change the page
    // protection to PAGE_READWRITE, store our hook function address, then
    // restore the old protection. If we didn't restore it, we'd risk leaving
    // writable imports around
    //
    unsigned long o (0);
    VirtualProtect (direct3d_create9_slot, sizeof (void *), PAGE_READWRITE, &o);
    *static_cast<void **> (direct3d_create9_slot) =
      reinterpret_cast<void *> (d3d9_create);
    VirtualProtect (direct3d_create9_slot, sizeof (void *), o, &o);
  }

  IDirect3D9* WINAPI
  imgui::d3d9_create (unsigned int sdk_version)
  {
    // ...
  }
}
