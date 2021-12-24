#!/usr/bin/env python3

import argparse
import xml.etree.ElementTree as ET 


class Type:
    def __init__(self, base_name, qualifier, is_ptr, array):
        self.base_name = base_name
        self.qualifier = qualifier
        self.is_ptr = is_ptr
        self.array = array

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
    def __init__(self, name, members):
        self.name = name
        self.members = members


class EnumEntry:
    def __init__(self, name, value, is_alias, is_bit, protect):
        self.name = name
        self.value = value
        self.is_alias = is_alias
        self.is_bit = is_bit
        self.protect = protect


class Enum:
    def __init__(self, name, entries, alias, is_bitmask):
        self.name = name
        self.entries = entries
        self.alias = alias
        self.is_bitmask = is_bitmask


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
    def __init__(self, name, params, return_type, success_codes, error_codes, alias):
        self.name = name
        self.params = params
        self.return_type = return_type
        self.success_codes = success_codes
        self.error_codes = error_codes
        self.alias = alias


def get_parser_cla_result():
    parser = argparse.ArgumentParser()

    parser.add_argument('-i', '--input', help = 'Input xml file')

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

            type = Type(member_type_node.text, type_qualifier, is_ptr, array)

            member = Member(type, name_node.text)
            members.append(member)

        struct = Struct(type_node.attrib["name"], members)

        structs[struct.name] = struct

    return structs

def collect_enums(root):
    result = {}

    for enum_node in root.findall('./types/type[@category="enum"]'):
        
        alias = None
        if 'alias' in enum_node.attrib:
            alias = enum_node.attrib['alias']
        
        enum = Enum(enum_node.attrib["name"], [], alias, False)
        result[enum.name] = enum

    for enum_node in root.findall('./enums[@type="enum"]'):
        enum = result[enum_node.attrib["name"]]
        
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

    return result


def add_extensions_to_enum(root, enums):

    for feature_node in root.findall('./feature/require/enum[@extends]'):
        enum = enums[feature_node.attrib["extends"]]

        value = None
        is_bit = False
        if 'bitpos' in feature_node.attrib:
            value = feature_node.attrib['bitpos']
            is_bit = True

        enum_entry = EnumEntry(feature_node.attrib["name"], value, is_alias = False, is_bit = is_bit, protect = None)
        enum.entries.append(enum_entry)

    for extension_node in root.findall('./extensions/extension/require/enum[@extends]'):
        enum = enums[extension_node.attrib["extends"]]

        protect = None
        if 'protect' in extension_node.attrib:
            protect = extension_node.attrib['protect']

        value = None
        is_bit = False
        if 'bitpos' in extension_node.attrib:
            value = extension_node.attrib['bitpos']
            is_bit = True

        enum_entry = EnumEntry(extension_node.attrib["name"], value, is_alias = False, is_bit = is_bit, protect = protect)
        enum.entries.append(enum_entry)

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

        print_bitmask(bitmask)

    return result


def collect_functions(root):
    result = {}

    for command_node in root.findall('./commands/command'):
        alias = None
        name = ''
        if 'alias' in command_node.attrib:
            alias = command_node.attrib['alias']
            name = command_node.attrib['name']
            function = Function(name, [], None, [], [], alias)
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
                params.append(Member(Type(param_type, type_qualifier, is_ptr, array = None), param_name))

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
                None
            )
            result[name] = function

    for func_name, function in result.items():
        print_function(function)

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

            type = Type(member_type_node.text, type_qualifier, is_ptr, array)

            member = Member(type, name_node.text)
            members.append(member)

        union = Struct(type_node.attrib["name"], members)

        result[union.name] = union

    return result

# TODO: handles

def print_struct(struct):
    print(f'struct {struct.name}')
    print('{')
    for member in struct.members:
        array = ''
        if member.type.array:
            array = member.type.array
        print(f'    {member.type.full_name} {member.name}{array};')
    print('};\n')


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


def parse_xml_file(input_xml_file_path):
    tree = ET.parse(input_xml_file_path)
    root = tree.getroot()

    #print(collect_type_categories(root))

    #structs = collect_structs(root)
    
    #enums = collect_enums(root)
    #add_extensions_to_enum(root, enums)

    #for enum_name, enum in enums.items():
    #    print_enum(enum)

    #bitmasks = collect_bitmasks(root)
    #functions = collect_functions(root)

    unions = collect_unions(root)

    for union_name, union in unions.items():
        print_union(union)


if __name__ == '__main__':
    settings = get_parser_cla_result()
    zones = parse_xml_file(settings.input)