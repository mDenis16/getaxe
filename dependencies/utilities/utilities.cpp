#include "utilities.hpp"
#include "../utilities/csgo.hpp"
#include <psapi.h>

std::uint8_t* utilities::pattern_scan(const char* module_name, const char* signature) noexcept {
    const auto module_handle = GetModuleHandleA(module_name);

    if (!module_handle)
        return nullptr;

    static auto pattern_to_byte = [](const char* pattern) {
        auto bytes = std::vector<int>{};
        auto start = const_cast<char*>(pattern);
        auto end = const_cast<char*>(pattern) + std::strlen(pattern);

        for (auto current = start; current < end; ++current) {
            if (*current == '?') {
                ++current;

                if (*current == '?')
                    ++current;

                bytes.push_back(-1);
            }
            else {
                bytes.push_back(std::strtoul(current, &current, 16));
            }
        }
        return bytes;
    };

    auto dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(module_handle);
    auto nt_headers =
        reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<std::uint8_t*>(module_handle) + dos_header->e_lfanew);

    auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
    auto pattern_bytes = pattern_to_byte(signature);
    auto scan_bytes = reinterpret_cast<std::uint8_t*>(module_handle);

    auto s = pattern_bytes.size();
    auto d = pattern_bytes.data();

    for (auto i = 0ul; i < size_of_image - s; ++i) {
        bool found = true;

        for (auto j = 0ul; j < s; ++j) {
            if (scan_bytes[i + j] != d[j] && d[j] != -1) {
                found = false;
                break;
            }
        }
        if (found)
            return &scan_bytes[i];
    }

    return nullptr;
}

std::string utilities::unicode_to_ascii( const std::wstring& unicode ) {
    std::string ascii_str( unicode.begin( ), unicode.end( ) );
    return ascii_str;
}
void* utilities::grab_interface( const char* module_name, const char* interf, bool bruteforce ) {
    auto create_interface = ( void* ( * )( const char*, int ) ) GetProcAddress( GetModuleHandleA( module_name ), "CreateInterface" );

    std::string str_interface, str_interface_version = "0";

    if ( bruteforce ) {
        for ( auto i = 0; i <= 99; i++ ) {
            str_interface = interf;
            str_interface += str_interface_version;
            str_interface += std::to_string( i );

            void* func_ptr = create_interface( str_interface.c_str( ), 0 );

            if ( func_ptr )
                return func_ptr;

            if ( i == 99 && str_interface_version == "0" ) {
                str_interface_version = "00";
                i = 0;
            }
            else if ( i == 99 && str_interface_version == "00" )
                return nullptr;
        }
    }
    else {
        void* ptr = create_interface( interf, 0 );

        if ( ptr )
            return ptr;
        else
            return nullptr;
    }

    return nullptr;
}
float utilities::get_distance( vec3_t src, vec3_t dest ) {
    vec3_t delta = src - dest;

    float fl_dist = ::sqrtf( ( delta.length( ) ) );

    if ( fl_dist < 1.0f )
        return 1.0f;

    return fl_dist;
}
