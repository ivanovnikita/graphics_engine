#!/usr/bin/env python3

import argparse
import xml.etree.ElementTree as ET 
from os import path


class Type:
    def __init__(self, base_name, qualifier, is_ptr, array, bitfield):
        self.base_name = base_name
        self.qualifier = qualifier
        self.is_ptr = is_ptr
        self.array = array
        self.bitfield = bitfield

        self.full_name = ''
        if qualifier:
            self.full_name += qualifier + ' '

        self.full_name += base_name

        if is_ptr:
            self.full_name += '*'

class Member:
    def __init__(self, type, name):
        self.type = type
        self.name = name


class Struct:
    def __init__(self, name, members, alias, protect, enum_type):
        self.name = name
        self.members = members
        self.alias = alias
        self.protect = protect
        self.enum_type = enum_type


    def get_cpp_name(self):
        if self.name.startswith('Vk'):
            return self.name[2:]
        else:
            return self.name


class EnumEntry:
    def __init__(self, name, value, is_alias, is_bit, protect):
        self.name = name
        self.value = value
        self.is_alias = is_alias
        self.is_bit = is_bit
        self.protect = protect

    def transform_name_part(self, name_part):
        if name_part.lower() == 'bit':
            return ''

        if name_part.lower() in [
            'ext', 
            'khr', 
            'qcom', 
        ]:
            return name_part.upper()

        result = ''
        is_first = True
        for char in name_part:
            if is_first:
                result += str(char).upper()

                if not str(char).isdigit():
                    is_first = False
            else:
                result += str(char).lower()

        return result


    def get_cpp_name(self, enum_name):
        result = ''

        enum_name_parts = enum_name.lower()

        is_first = True
        name_part = ''
        for char in self.name:
            if is_first:
                is_first = False
                name_part += char
                continue

            if char == '_':
                is_first = True
                if name_part.lower() != 'flag' and enum_name_parts.startswith(name_part.lower()):
                    enum_name_parts = enum_name_parts[len(name_part):]
                elif name_part.lower() != 'flag' and enum_name_parts.endswith(name_part.lower()):
                    enum_name_parts = enum_name_parts[:-len(name_part)]
                else:
                    result += self.transform_name_part(name_part)
                name_part = ''
                continue

            name_part += str(char).lower()

        if name_part.lower() != 'flag' and enum_name_parts.startswith(name_part.lower()):
            enum_name_parts = enum_name_parts[len(name_part):]
        elif name_part.lower() != 'flag' and enum_name_parts.endswith(name_part.lower()):
            enum_name_parts = enum_name_parts[:-len(name_part)]
        else:
            result += self.transform_name_part(name_part)

        result = 'e' + result
        
        return result


class Enum:
    def __init__(self, name, entries, alias, is_bitmask, protect):
        self.name = name
        self.entries = entries
        self.alias = alias
        self.is_bitmask = is_bitmask
        self.protect = protect
        self.bitwidth = 32


    def get_cpp_name(self):
        if self.name.startswith('Vk'):
            return self.name[2:]
        else:
            return self.name


class Bitmask:
    def __init__(self, name, bits_type, alias):
        self.name = name
        self.bits_type = bits_type
        self.alias = alias


class Argument:
    def __init__(self, type, name):
        self.type = type
        self.name = name


class Function:
    def __init__(self, name, params, return_type, success_codes, error_codes, alias, protect):
        self.name = name
        self.params = params
        self.return_type = return_type
        self.success_codes = success_codes
        self.error_codes = error_codes
        self.alias = alias
        self.protect = protect


class VulkanApi:
    def __init__(self, structs, unions, enums, bitmasks, functions):
        self.structs = structs
        self.unions = unions
        self.enums = enums
        self.bitmasks = bitmasks
        self.functions = functions


def get_parser_cla_result():
    parser = argparse.ArgumentParser()

    parser.add_argument('-i', '--input', help = 'Input xml file')
    parser.add_argument('-o', '--output', help = 'Output directory for generated files')

    return parser.parse_args()


def collect_type_categories(root):
    types = set()

    for type_node in root.findall('./types/type'):
        if 'category' in type_node.attrib:
            types.add(type_node.attrib['category'])

    return types


def collect_structs(root):
    structs = {}

    for type_node in root.findall('./types/type[@category="struct"]'):

        alias = None
        if 'alias' in type_node.attrib:
            alias = type_node.attrib['alias']
        
        enum_type = None
        members = []
        for member_node in type_node.findall('member'):
            type_qualifier = None
            if member_node.text:
                type_qualifier = member_node.text.strip()

            member_type_node = member_node.find("type")
            is_ptr = member_type_node.tail and (member_type_node.tail.strip() == '*')

            if member_type_node.text == "VkStructureType" and 'values' in member_node.attrib:
                enum_type = member_node.attrib['values']

            name_node = member_node.find("name")
            array = None
            bitfield = None
            if name_node.tail:
                if name_node.tail == '[':
                    array_size_node = member_node.find("enum")
                    array = f'[{array_size_node.text}]'
                elif name_node.tail.startswith('['):
                    array = name_node.tail
                elif name_node.tail.startswith(':'):
                    bitfield = name_node.tail

            type = Type(member_type_node.text, type_qualifier, is_ptr, array, bitfield)

            member = Member(type, name_node.text)
            members.append(member)

        struct = Struct(type_node.attrib["name"], members, alias, protect = None, enum_type = enum_type)

        structs[struct.name] = struct

    return structs

def collect_enums(root):
    result = {}

    for enum_node in root.findall('./types/type[@category="enum"]'):
        
        alias = None
        if 'alias' in enum_node.attrib:
            alias = enum_node.attrib['alias']
        
        enum = Enum(enum_node.attrib["name"], [], alias, is_bitmask = False, protect = None)
        result[enum.name] = enum

    for enum_node in root.findall('./enums[@type="enum"]'):
        enum = result[enum_node.attrib['name']]

        for enum_value_node in enum_node.findall('./enum'):
            value = ''
            if 'value' in enum_value_node.attrib:
                value = enum_value_node.attrib['value']

            is_alias = False
            if 'alias' in enum_value_node.attrib:
                value = enum_value_node.attrib['alias']
                is_alias = True

            enum_entry = EnumEntry(enum_value_node.attrib["name"], value, is_alias, is_bit = False, protect = None)
            enum.entries.append(enum_entry)

    for enum_node in root.findall('./enums[@type="bitmask"]'):
        enum = result[enum_node.attrib["name"]]
        enum.is_bitmask = True
        
        if 'bitwidth' in enum_node.attrib:
            assert enum_node.attrib['bitwidth'] == '64'
            enum.bitwidth = 64

        for enum_value_node in enum_node.findall('./enum'):
            value = ''
            is_bit = False
            if 'value' in enum_value_node.attrib:
                value = enum_value_node.attrib['value']

            if 'bitpos' in enum_value_node.attrib:
                value = enum_value_node.attrib['bitpos']
                is_bit = True

            is_alias = False
            if 'alias' in enum_value_node.attrib:
                value = enum_value_node.attrib['alias']
                is_alias = True

            enum_entry = EnumEntry(enum_value_node.attrib["name"], value, is_alias, is_bit, protect = None)
            enum.entries.append(enum_entry)

    for enum in result.values():
        enum.entries.sort(key = lambda entry: entry.name)
        unique_entries = []
        for entry in enum.entries:
            if len(unique_entries) == 0 or unique_entries[-1].name != entry.name:
                unique_entries.append(entry)
        enum.entries = unique_entries

    return result


def add_extensions_to_enum(root, enums):

    for feature_node in root.findall('./feature/require/enum[@extends]'):
        enum = enums[feature_node.attrib["extends"]]

        value = None

        is_alias = False
        if 'alias' in feature_node.attrib:
            is_alias = True
            value = feature_node.attrib['alias']

        is_bit = False
        if 'bitpos' in feature_node.attrib:
            value = feature_node.attrib['bitpos']
            is_bit = True

        enum_entry = EnumEntry(feature_node.attrib["name"], value, is_alias, is_bit = is_bit, protect = None)
        enum.entries.append(enum_entry)

    # TODO: process implicit protection (see VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT, protected by win32)
    for extension_node in root.findall('./extensions/extension[@supported!="disabled"]/require/enum[@extends]'):
        enum = enums[extension_node.attrib["extends"]]

        is_alias = False
        if 'alias' in extension_node.attrib:
            is_alias = True
            value = extension_node.attrib['alias']

        protect = None
        if 'protect' in extension_node.attrib:
            protect = extension_node.attrib['protect']

        value = None
        is_bit = False
        if 'bitpos' in extension_node.attrib:
            value = extension_node.attrib['bitpos']
            is_bit = True

        enum_entry = EnumEntry(extension_node.attrib["name"], value, is_alias, is_bit = is_bit, protect = protect)
        enum.entries.append(enum_entry)

    for enum in enums.values():
        enum.entries.sort(key = lambda entry: entry.name)
        unique_entries = []
        for entry in enum.entries:
            if len(unique_entries) == 0 or unique_entries[-1].name != entry.name:
                unique_entries.append(entry)
        enum.entries = unique_entries

    return


def collect_bitmasks(root):
    result = {}

    for bitmask_node in root.findall('./types/type[@category="bitmask"]'):
        name = ''
        alias = None
        if 'alias' in bitmask_node.attrib:
            alias = bitmask_node.attrib['alias']
            name = bitmask_node.attrib['name']
        else:
            name = bitmask_node.find('name').text

        bits_type = None
        if 'requires' in bitmask_node.attrib:
            bits_type = bitmask_node.attrib['requires']

        bitmask = Bitmask(name, bits_type, alias)
        result['name'] = bitmask

    return result


def collect_functions(root):
    result = {}

    for command_node in root.findall('./commands/command'):
        alias = None
        name = ''
        if 'alias' in command_node.attrib:
            alias = command_node.attrib['alias']
            name = command_node.attrib['name']
            function = Function(name, [], None, [], [], alias, protect = None)
            result[name] = function
            continue
        else:
            proto = command_node.find('proto')
            name = proto.find('name').text
            return_type = proto.find('type').text

            params = []
            for param in command_node.findall('param'):
                type_node = param.find('type')

                param_type = type_node.text
                is_ptr = type_node.tail and (type_node.tail.strip() == '*')

                type_qualifier = None
                if param.text:
                    type_qualifier = param.text.strip()

                param_name = param.find('name').text
                
                # TODO: parse array with size (see vkCmdSetFragmentShadingRateEnumNV combinerOps)
                params.append(Member(Type(param_type, type_qualifier, is_ptr, array = None, bitfield = None), param_name))

            success_codes = []
            if 'successcodes' in command_node.attrib:
                success_codes = command_node.attrib['successcodes'].split(',')

            error_codes = []
            if 'errorcodes' in command_node.attrib:
                error_codes = command_node.attrib['errorcodes'].split(',')            

            function = Function(
                name,
                params,
                return_type,
                success_codes,
                error_codes,
                alias = None,
                protect = None
            )
            result[name] = function

    return result


def collect_unions(root):
    result = {}

    for type_node in root.findall('./types/type[@category="union"]'):
        
        members = []
        for member_node in type_node.findall('member'):
            type_qualifier = None
            if member_node.text:
                type_qualifier = member_node.text.strip()

            member_type_node = member_node.find("type")
            is_ptr = member_type_node.tail and (member_type_node.tail.strip() == '*')

            name_node = member_node.find("name")
            array = None
            if name_node.tail:
                if name_node.tail == '[':
                    array_size_node = member_node.find("enum")
                    array = f'[{array_size_node.text}]'
                else:
                    array = name_node.tail

            type = Type(member_type_node.text, type_qualifier, is_ptr, array, bitfield = None)

            member = Member(type, name_node.text)
            members.append(member)

        # TODO: is there enum type for union?
        union = Struct(type_node.attrib["name"], members, alias = None, protect = None, enum_type = None)

        result[union.name] = union

    return result

def add_platform_protection(root, api):
    platforms = {}
    for platform_node in root.findall('./platforms/platform'):
        platforms[platform_node.attrib['name']] = platform_node.attrib['protect']

    for extension_node in root.findall('./extensions/extension[@platform]'):
        platform = extension_node.attrib['platform']
        protect = platforms[platform]

        for enum_node in extension_node.findall('./require/enum'):
            if not 'extends' in enum_node.attrib:
                continue
            enum_name = enum_node.attrib['extends']
            entry_name = enum_node.attrib['name']

            enum = api.enums[enum_name]
            for entry in enum.entries:
                if entry.name == entry_name:
                    entry.protect = protect
                    break

        for type_node in extension_node.findall('./require/type'):
            type_name = type_node.attrib['name']
            if type_name in api.structs:
                struct = api.structs[type_name]
                struct.protect = protect
            if type_name in api.unions:
                union = api.unions[type_name]
                union.protect = protect
            if type_name in api.enums:
                enum = api.enums[type_name]
                enum.protect = protect

        for command_node in extension_node.findall('./require/command'):
            command_name = command_node.attrib['name']
            function = api.functions[command_name]
            function.protect = protect

    api.enums['VkImageFormatConstraintsFlagBitsFUCHSIA'].protect = platforms['fuchsia']

# TODO: handles

def collect_api(root):
    structs = collect_structs(root)
    unions = collect_unions(root)
    
    enums = collect_enums(root)
    add_extensions_to_enum(root, enums)

    bitmasks = collect_bitmasks(root)

    functions = collect_functions(root)

    api = VulkanApi(structs, unions, enums, bitmasks, functions)

    add_platform_protection(root, api)

    return api


def print_struct(struct):
    if struct.alias:
        print(f'using {struct.name} = {struct.alias};\n')
        return

    print(f'struct {struct.name}')
    print('{')
    for member in struct.members:
        array = ''
        if member.type.array:
            array = member.type.array
        print(f'    {member.type.full_name} {member.name}{array};')
    print('};\n')
    if struct.enum_type:
        print(f'VkStructureType = {struct.enum_type}')


def print_union(union):
    print(f'union {union.name}')
    print('{')
    for member in union.members:
        array = ''
        if member.type.array:
            array = member.type.array
        print(f'    {member.type.full_name} {member.name}{array};')
    print('};\n')


def print_enum(enum):
    if enum.alias:
        print(f'using {enum.name} = {enum.alias};\n')
        return

    print(f'enum class {enum.name}')
    print('{')
    for enum_value in enum.entries:
        value = enum_value.value
        if enum_value.is_bit:
            value = f'1 << {enum_value.value}'
        print(f'    {enum_value.name} = {value},{" // protect: " + enum_value.protect if enum_value.protect else ""}')
    print('};\n')


def print_bitmask(bitmask):
    if bitmask.alias:
        print(f'using {bitmask.name} = {bitmask.alias};')
    elif bitmask.bits_type:
        print(f'using {bitmask.name} = vk::Flags<{bitmask.bits_type}>;')
    else:
        print(f'using {bitmask.name} = vk::Flags<>;')


def print_function(function):
    if function.alias:
        print(f'using {function.name} = {function.alias};')
        return

    print(f'{function.return_type} {function.name}')
    print('(')
    for param in function.params:
        print(f'    {param.type.full_name} {param.name},')
    print(');')

    if function.return_type == 'VkResult':
        print(f' -> {function.success_codes} {function.error_codes}')

    print('')


def print_api(api):
    for struct in api.structs.values():
        print_struct(struct)

    for union in api.unions.values():
        print_union(union)

    for enum in api.enums.values():
        print_enum(enum)

    for bitmask in api.bitmasks.values():
        print_bitmask(bitmask)
    
    for function in api.functions.values():
        print_function(function)


def generate_reflection_structs(structs):
    result = ''

    for struct in structs.values():
        if struct.alias:
            continue

        if struct.protect:
            result += f'#ifdef {struct.protect}\n\n'

        result += f'    export constexpr auto register_members(const {struct.get_cpp_name()}*) noexcept\n'
        result += '    {\n'
        result += '        using namespace ge;\n'
        result += '        return std::tuple\n'
        result += '        {\n'
        
        for member in struct.members:
            if member.type.bitfield:
                continue
            result += f'            Member<&{struct.get_cpp_name()}::{member.name}>{{"{member.name}"}},\n'
         
        # remove last comma
        if len(struct.members) > 0:
            result = result[:-2]
            result += '\n'
        
        result += '        };\n'
        result += '    }\n\n'

        if struct.protect:
            result += '#endif\n\n'

    return result


def generate_reflection(api, module_name):
    result = ''

    result += 'module;\n\n'

    result += '#include <vulkan/vulkan.hpp>\n\n'

    result += f'export module {module_name};\n\n'

    result += 'export import member;\n\n'

    result += 'namespace vk\n'
    result += '{\n'    

    result += generate_reflection_structs(api.structs)
    result += generate_reflection_structs(api.unions)

    result += '}\n'

    return result


def generate_enum_to_string_view_decl(enums, module_name):
    result = ''

    result += 'module;\n\n'

    result += '#include <string_view>\n\n'

    result += '#include <vulkan/vulkan.hpp>\n\n'

    result += f'export module {module_name};'

    result += 'namespace vk\n'
    result += '{\n'

    for enum in enums.values():
        if enum.alias:
            continue

        if enum.protect:
            result += f'#ifdef {enum.protect}\n\n'

        result += f'    export std::string_view to_string_view({enum.get_cpp_name()}) noexcept;\n'

        if enum.protect:
            result += '#endif\n\n'

    result += '}\n'

    return result

def generate_enum_to_string_view_def(enums, module_name):
    result = ''

    result += 'module;\n\n'

    result += '#include <string_view>\n\n'

    result += '#include <vulkan/vulkan.hpp>\n\n'

    result += f'module {module_name};\n\n'

    result += 'namespace vk\n'
    result += '{\n'

    # TODO: add defines for protected entries
    for enum in enums.values():
        if enum.alias:
            continue

        if enum.protect:
            result += f'#ifdef {enum.protect}\n\n'

        enum_name = enum.get_cpp_name()

        if len(enum.entries) == 0:
            result += f'    std::string_view to_string_view({enum_name}) noexcept\n'
            result += '    {\n'
            result += f'        return "unknown value of {enum_name}";\n'
        else:
            result += f'    std::string_view to_string_view(const {enum_name} flags) noexcept\n'
            result += '    {\n'
            result += f'        const {enum.name} c_flags = static_cast<{enum.name}>(flags);\n'
            result += '        switch (c_flags)\n'
            result += '        {\n'

            for entry in enum.entries:
                if entry.is_alias:
                    continue

                if entry.protect:
                    result += f'#ifdef {entry.protect}\n\n'

                entry_name = entry.get_cpp_name(enum.name)

                result += f'            case {entry.name}: return "{entry_name}";\n'

                if entry.protect:
                    result += f'#endif\n\n'
            
            result += f'            default: return "unknown value of {enum_name}";\n'
            result += '        }\n'
            result += '        __builtin_unreachable();\n'
        result += '    }\n\n'

        if enum.protect:
            result += '#endif\n\n'  

    result += '}\n'

    return result


def generate_fwds(api, module_name):
    result = ''

    result += 'module;\n\n'

    result += '#include <cstdint>\n\n'

    result += f'export module {module_name};'

    result += 'namespace vk\n'
    result += '{\n'

    for struct in api.structs.values():
        if struct.alias:
            continue

        if struct.protect:
            result += f'#ifdef {struct.protect}\n\n'

        result += f'    export struct {struct.get_cpp_name()};\n'

        if struct.protect:
            result += '#endif\n\n'

    for enum in api.enums.values():
        if enum.alias:
            continue

        if enum.protect:
            result += f'#ifdef {enum.protect}\n\n'

        underlying_type = ''
        if enum.is_bitmask:
            bitwidth = 'uint32_t'
            if enum.bitwidth == 64:
                bitwidth = 'uint64_t'
            underlying_type = f' : {bitwidth}'

        result += f'    export enum class {enum.get_cpp_name()}{underlying_type};\n'

        if enum.protect:
            result += '#endif\n\n'

    result += '}\n'

    return result


def generate_invoke_for_downcasted(api, module_name):
    result = ''

    result += 'module;\n\n'

    result += '#include <string_view>\n\n'

    result += '#include <vulkan/vulkan.hpp>\n\n'

    result += f'export module {module_name};'

    result += 'namespace ge\n'
    result += '{\n'

    enum_structure_type = api.enums['VkStructureType']
    enum_structure_type_entries = {}
    for entry in enum_structure_type.entries:
        enum_structure_type_entries[entry.name] = entry

    result += '    export template <typename F>\n'
    result += '    void invoke_for_downcasted(const vk::BaseInStructure& in, F&& func)\n'
    result += '    {\n'
    result += '        const VkStructureType c_type = static_cast<VkStructureType>(in.sType);\n'
    result += '        switch (c_type)\n'
    result += '        {\n'

    for struct in api.structs.values():
        if not struct.enum_type:
            continue;

        enum_type_entry = None
        if struct.enum_type in enum_structure_type_entries:
            enum_type_entry = enum_structure_type_entries[struct.enum_type]
            enum_structure_type_entries.pop(struct.enum_type)

        if not enum_type_entry:
            #print(f'enum entry not found for {struct.enum_type}')
            # perhaps it is absent because extension support is disabled
            continue

        if enum_type_entry.is_alias:
            continue

        if enum_type_entry.protect and enum_type_entry.protect == 'VK_ENABLE_BETA_EXTENSIONS':
            result += f'#ifdef {enum_type_entry.protect}\n\n'

        result += f'        case {enum_type_entry.name}:\n'
        result += '        {\n'

        if enum_type_entry.protect:
            result += f'#ifdef {enum_type_entry.protect}\n\n'
        
        result += f'            const vk::{struct.get_cpp_name()}& downcasted = reinterpret_cast<const vk::{struct.get_cpp_name()}&>(in);\n'
        result += '            func(downcasted);\n'

        if enum_type_entry.protect:
            result += f'#endif\n\n'

        result += '            break;\n'
        result += '        }\n'

        if enum_type_entry.protect and enum_type_entry.protect == 'VK_ENABLE_BETA_EXTENSIONS':
            result += f'#endif\n\n'

    for entry in enum_structure_type_entries.values():
        if entry.is_alias:
            continue

        result += f'        case {entry.name}:\n'

    result += '        case VK_STRUCTURE_TYPE_MAX_ENUM:\n'
    result += '        {\n'
    result += '            break;\n'
    result += '        }\n'


    result += '        }\n'
    result += '    }\n'

    result += '}\n'

    return result


def write_file(text, dirpath, filename):
    with open(path.join(dirpath, filename), 'w') as file:
        file.write(text)


def generate_code(input_xml_file_path, output_dir):
    tree = ET.parse(input_xml_file_path)
    root = tree.getroot()

    #print(collect_type_categories(root))

    api = collect_api(root)
    #print_api(api)

    refl = generate_reflection(api, 'vulkan_refl')
    write_file(refl, output_dir, 'vulkan_refl.ixx')

    str_view_decl = generate_enum_to_string_view_decl(api.enums, 'to_string_view_enum')
    write_file(str_view_decl, output_dir, 'to_string_view_enum.ixx')

    str_view_def = generate_enum_to_string_view_def(api.enums, 'to_string_view_enum')
    write_file(str_view_def, output_dir, 'to_string_view_enum.cxx')

    fwds = generate_fwds(api, 'vulkan_fwd')
    write_file(fwds, output_dir, 'vulkan_fwds.ixx')

    downcasted = generate_invoke_for_downcasted(api, 'invoke_for_downcasted')
    write_file(downcasted, output_dir, 'invoke_for_downcasted.ixx')


if __name__ == '__main__':
    settings = get_parser_cla_result()
    generate_code(settings.input, settings.output)
