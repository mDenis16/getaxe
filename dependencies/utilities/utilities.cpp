#include "utilities.hpp"
#include "../utilities/csgo.hpp"
#include <psapi.h>

template <typename T>
 constexpr auto utilities::relativeToAbsolute ( uintptr_t address ) noexcept {
    return ( T ) ( address + 4 + *reinterpret_cast< std::int32_t * >( address ) );
}

static std::pair<void *, std::size_t> getModuleInformation ( const char * name ) noexcept {

    if ( HMODULE handle = GetModuleHandleA ( name ) ) {
        if ( MODULEINFO moduleInfo; GetModuleInformation ( GetCurrentProcess ( ), handle, &moduleInfo, sizeof ( moduleInfo ) ) )
            return std::make_pair ( moduleInfo.lpBaseOfDll, moduleInfo.SizeOfImage );
    }
    return {};

}

[[nodiscard]] static auto generateBadCharTable ( std::string_view pattern ) noexcept {
    assert ( !pattern.empty ( ) );

    std::array<std::size_t, ( std::numeric_limits<std::uint8_t>::max )( ) + 1> table;

    auto lastWildcard = pattern.rfind ( '?' );
    if ( lastWildcard == std::string_view::npos )
        lastWildcard = 0;

    const auto defaultShift = ( std::max ) ( std::size_t ( 1 ), pattern.length ( ) - 1 - lastWildcard );
    table.fill ( defaultShift );

    for ( auto i = lastWildcard; i < pattern.length ( ) - 1; ++i )
        table [ static_cast< std::uint8_t >( pattern [ i ] ) ] = pattern.length ( ) - 1 - i;

    return table;
}

std::uintptr_t utilities::code_style_pattern ( const char * moduleName, std::string_view pattern, bool reportNotFound  ) noexcept {
    static auto id = 0;
    ++id;

    const auto [moduleBase, moduleSize] = getModuleInformation ( moduleName );

    if ( moduleBase && moduleSize ) {
        const auto lastIdx = pattern.length ( ) - 1;
        const auto badCharTable = generateBadCharTable ( pattern );

        auto start = static_cast< const char * >( moduleBase );
        const auto end = start + moduleSize - pattern.length ( );

        while ( start <= end ) {
            int i = lastIdx;
            while ( i >= 0 && ( pattern [ i ] == '?' || start [ i ] == pattern [ i ] ) )
                --i;

            if ( i < 0 )
                return reinterpret_cast< std::uintptr_t >( start );

            start += badCharTable [ static_cast< std::uint8_t >( start [ lastIdx ] ) ];
        }
    }

    return 0;
}
#define strcat_(x, y) x ## y
#define strcat(x, y) strcat_(x, y)
#define PRINT_VALUE(x) template <int> struct strcat(strcat(value_of_, x), _is); static_assert(strcat(strcat(value_of_, x), _is)<x>::x, "");


std::uint32_t utilities::pattern_scan_rel_function(const char* _module, const char* signature, int offsetStartRelAddr)
{
    uint32_t* pattern = (uint32_t*)(utilities::pattern_scan(_module, signature) + offsetStartRelAddr);
    return (uint32_t)pattern + (uint32_t)*pattern + 4;
}
std::uint32_t* utilities::pattern_scan32(const char* module_name, const char* signature) noexcept
{
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
    auto scan_bytes = reinterpret_cast<std::uint32_t*>(module_handle);

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
