#include <libiw4x/imgui.hxx>

#include <dbghelp.h>

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx9.h>

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

namespace iw4x
{
  namespace
  {
    IDirect3D9* WINAPI
    create (UINT sdk_version)
    {
      // ...
    }

    void
    intercept_d3d9 ()
    {
      // This will hold the address of the IAT slot we want to overwrite.
      //
      void* direct3dcreate9_slot = nullptr;

      // Passing nullptr to GetModuleHandle means "give me the handle for the
      // file that started this process", in practice, this is the image base
      // address in memory.
      //
      HMODULE m (GetModuleHandle (nullptr));

      // ImageNtHeader() takes a pointer to the DOS header and finds
      // the NT header using the e_lfanew offset. From here on, we have
      // a full view of section headers and data directories.
      //
      PIMAGE_NT_HEADERS n (
        ImageNtHeader (reinterpret_cast<PIMAGE_DOS_HEADER> (m)));

      // For a loaded PE image in memory, RVAs can be simply converted to
      // pointers by adding them to the module base address, since the loader
      // has already mapped the sections to their virtual addresses.
      //
      auto rva_to_ptr = [m] (DWORD rva) -> void *
      {
        return reinterpret_cast<char*>(m) + rva;
      };

      // The OptionalHeader's DataDirectory table has an entry for the import
      // table (IMAGE_DIRECTORY_ENTRY_IMPORT). Its VirtualAddress is an RVA
      // pointing to the first IMAGE_IMPORT_DESCRIPTOR in the array. Each
      // descriptor represents one DLL that this module imports from.
      //
      PIMAGE_IMPORT_DESCRIPTOR imp (
        static_cast<PIMAGE_IMPORT_DESCRIPTOR> (rva_to_ptr (
          n->OptionalHeader.DataDirectory [IMAGE_DIRECTORY_ENTRY_IMPORT]
            .VirtualAddress)));

      // Walk each imported DLL until we find d3d9.dll.
      //
      for (; imp->Name != 0; ++imp)
      {
        if (auto n = static_cast<char *> (rva_to_ptr (imp->Name)))
        {
          // Case-insensitive match because the import name may differ in
          // case between the linker and the loader.
          //
          if (_stricmp (n, "d3d9.dll") == 0)
          {
            // Once we have the right DLL, we need both the name table
            // (OriginalFirstThunk) and the address table (FirstThunk).
            //
            // OFT is the lookup table the loader uses to find the right address
            // at load time, it points to IMAGE_IMPORT_BY_NAME structures that
            // store the function names.
            //
            // FT is the table that actually gets patched by the loader with the
            // resolved addresses from the target DLL, this is the IAT we're
            // going to patch.
            //
            IMAGE_THUNK_DATA *oft (static_cast<IMAGE_THUNK_DATA *> (
              rva_to_ptr (imp->OriginalFirstThunk)));

            IMAGE_THUNK_DATA *ft (static_cast<IMAGE_THUNK_DATA *> (
              rva_to_ptr (imp->FirstThunk)));

            // We stop when AddressOfData is zero (end of import list). For
            // ordinals (high bit set), there's no name to check, so we skip
            // those.
            //
            for (; (oft != nullptr) && oft->u1.AddressOfData != 0; ++oft, ++ft)
            {
              if ((oft->u1.Ordinal & IMAGE_ORDINAL_FLAG) == 0U)
              {
                // Translate AddressOfData to an IMAGE_IMPORT_BY_NAME.
                //
                IMAGE_IMPORT_BY_NAME *ibn (static_cast<IMAGE_IMPORT_BY_NAME *> (
                  rva_to_ptr (oft->u1.AddressOfData)));

                // Check for the symbol we want, this must match exactly,
                // because IAT slots are per-function.
                //
                if ((ibn != nullptr) &&
                    strcmp (reinterpret_cast<const char *> (ibn->Name),
                            "Direct3DCreate9") == 0)
                {
                  // Found it, store the address of the live function pointer
                  // slot. This is where the loader wrote the real address,
                  // and where we'll write ours.
                  //
                  direct3dcreate9_slot = static_cast<void *> (&ft->u1.Function);
                }
              }
            }
          }
        }
      }

      // The IAT is mapped read-only, so we temporarily change the page
      // protection to PAGE_READWRITE, store our hook function address, then restore
      // the old protection. If we didn't restore it, we'd risk leaving writable
      // imports around
      //
      DWORD o (0);
      VirtualProtect (direct3dcreate9_slot, sizeof (void*), PAGE_READWRITE, &o);
      *static_cast<void **> (direct3dcreate9_slot) =
        reinterpret_cast<void *> (create);
      VirtualProtect (direct3dcreate9_slot, sizeof (void *), o, &o);
    }
  }

  imgui::imgui () = default;
}
